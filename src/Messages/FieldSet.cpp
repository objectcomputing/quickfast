// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldSet.h"
#include <Messages/Sequence.h>
#include <Messages/Group.h>
#include <Common/Exceptions.h>
#include <Common/Profiler.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldSet::FieldSet(size_t res)
: fields_(reinterpret_cast<MessageField *>(new unsigned char[sizeof(MessageField) * res]))
, capacity_(res)
, used_(0)
{
  memset(fields_, 0, sizeof(MessageField) * capacity_);
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
    memset(buffer, 0, sizeof(MessageField) * capacity_);
    for(size_t nField = 0; nField < used_; ++nField)
    {
      new(&buffer[nField]) MessageField(fields_[nField]);
    }

    MessageField * oldBuffer = fields_;
    size_t oldUsed = used_;
    fields_ = buffer;
    capacity_ = capacity;

    while (oldUsed > 0)
    {
      --oldUsed;
      oldBuffer[oldUsed].~MessageField();
    }
    delete[] reinterpret_cast<unsigned char *>(oldBuffer);
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
  memset(fields_, 0, sizeof(MessageField) * capacity_);
}

const MessageField &
FieldSet::operator[](size_t index)const
{
  if(index >= used_)
  {
    throw UsageError("Coding Error", "Accessing FieldSet entry: index out of range.");
  }
  return fields_[index];
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
    reserve(((used_ + 1) * 3) / 2);
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

void
FieldSet::getFieldInfo(size_t index, std::string & name, ValueType::Type & type, FieldCPtr & fieldPtr)const
{
  name = fields_[index].name();
  type = fields_[index].getField()->getType();
  fieldPtr = fields_[index].getField();
}

bool
FieldSet::equals (const FieldSet & rhs, std::ostream & reason) const
{
  if(used_ != rhs.used_)
  {
    reason << "Field counts: " << used_ << " != " << rhs.used_;
    return false;
  }
  // application type "any" matches anything.
  if(applicationType_ != "any" && rhs.applicationType_ != "any")
  {
    if(applicationType_ != rhs.applicationType_)
    {
      reason << "Application types: " << applicationType_ << " != " << rhs.applicationType_;
      return false;
    }
    if(!applicationTypeNs_.empty() && !rhs.applicationTypeNs_.empty() && applicationTypeNs_ != rhs.applicationTypeNs_)
    {
      reason << "Application type namespaces: " << applicationTypeNs_ << " != " << rhs.applicationTypeNs_;
      return false;
    }
  }
  for(size_t nField = 0; nField < used_; ++nField)
  {
    if (fields_[nField].name() != rhs.fields_[nField].name())
    {
      reason << "Field[" << nField << "] names: " << fields_[nField].name() << " != " << rhs.fields_[nField].name();
      return false;
    }
    Messages::FieldCPtr f1 = fields_[nField].getField();
    Messages::FieldCPtr f2 = fields_[nField].getField();
    if(*f1 != *f2)
    {
      reason << "Field[" << nField << "] "<< fields_[nField].name() << "values: " << f1->displayString() << " != " << f2->displayString();
      return false;
    }
  }
  return true;
}

