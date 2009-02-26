// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldSet.h"
#include <Messages/Field.h>
#include <Common/Exceptions.h>

#include <Common/Profiler.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldSet::FieldSet(size_t res)
: fields_(reinterpret_cast<MessageField *>(new unsigned char[sizeof(MessageField) * res]))
, capacity_(res)
, used_(0)
{
  memset(fields_, 0, sizeof(sizeof(MessageField) * capacity_));
}

FieldSet::FieldSet(const FieldSet & rhs)
: fields_(reinterpret_cast<MessageField *>(new unsigned char[sizeof(MessageField) * rhs.used_]))
, capacity_(rhs.capacity_)
, used_(rhs.used_)
{
  memset(fields_, 0, sizeof(sizeof(MessageField) * capacity_));
  for(size_t nField = 0; nField < used_; ++nField)
  {
    new (&fields_[nField]) MessageField(rhs.fields_[nField]);
  }
}

FieldSet::~FieldSet()
{
  clear();
  delete [] reinterpret_cast<unsigned char *>(fields_);
}

void
FieldSet::reserve(size_t capacity)
{
  if(capacity > capacity_)
  {
    MessageField * buffer = reinterpret_cast<MessageField *>(new unsigned char[sizeof(MessageField) * capacity]);
    memset(buffer, 0, sizeof(sizeof(MessageField) * capacity_));
    for(size_t nField = 0; nField < used_; ++nField)
    {
      new(&buffer[nField]) MessageField(fields_[nField]);
    }
    unsigned char * oldBuffer = reinterpret_cast<unsigned char *>(fields_);
    fields_ = buffer;
    capacity_ = capacity;
    delete[] oldBuffer;
  }
}

void
FieldSet::clear(size_t capacity)
{
  while(used_ > 0)
  {
    --used_;
    fields_[used_].~MessageField();
  }
  if(capacity > capacity_)
  {
    reserve(capacity);
  }
  memset(fields_, 0, sizeof(sizeof(MessageField) * capacity_));
}


bool
FieldSet::isPresent(const std::string & name) const
{
  for(size_t index = 0; index < used_; ++index)
  {
    if(name == fields_[index].name())
    {
      return fields_[index].getField()->isDefined();
    }
  }
  return false;
}

void
FieldSet::addField(const FieldIdentityCPtr & identity, const FieldCPtr & value)
{
  PROFILE_POINT("FieldSet::addField");
  if(used_ >= capacity_)
  {
    PROFILE_POINT("FieldSet::grow");
    // todo complain.  This should not happen
    reserve((used_ * 3) / 2);
  }
  new (fields_ + used_) MessageField(identity, value);
  ++used_;
}

bool
FieldSet::getField(const std::string &name, FieldCPtr & value) const
{
  PROFILE_POINT("FieldSet::getField");
  for(size_t index = 0; index < used_; ++index)
  {
    if(name == fields_[index].name())
    {
      value = fields_[index].getField();
      return value->isDefined();
    }
  }
  return false;
}

bool
FieldSet::getIdentity(const std::string &name, FieldIdentityCPtr & identity) const
{
  for(size_t index = 0; index < used_; ++index)
  {
    if(name == fields_[index].name())
    {
      identity = fields_[index].getIdentity();
      return true;
    }
  }
  return false;
}
