// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldUtf8.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldUtf8::FieldUtf8(const std::string & value)
  : Field(true)
  , value_(value)
{
}

FieldUtf8::FieldUtf8(const uchar * buffer, size_t length)
  : Field(true)
  , value_(std::string(reinterpret_cast<const char *>(buffer), length))
{
}


FieldUtf8::FieldUtf8()
  : Field(false)
  , value_()
{
}

void
FieldUtf8::setValue(const uchar * buffer, size_t length)
{
  value_ = std::string(reinterpret_cast<const char *>(buffer), length);
  valid_ = true;
}

FieldUtf8::~FieldUtf8()
{
}

Field::FieldType
FieldUtf8::getType()const
{
  return Field::UTF8;
}

bool
FieldUtf8::isString() const
{
  return true;
}

const std::string &
FieldUtf8::toString() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return value_;
}

void
FieldUtf8::setValue(const std::string & value)
{
  value_ = value;
  valid_ = true;
}

const std::string &
FieldUtf8::toUtf8() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return value_;
}

FieldCPtr
FieldUtf8::create(const std::string & value)
{
  return new FieldUtf8(value);
}

FieldCPtr
FieldUtf8::create(const uchar * buffer, size_t length)
{
  return new FieldUtf8(buffer, length);
}

FieldCPtr
FieldUtf8::createNull()
{
  return new FieldUtf8;
}
