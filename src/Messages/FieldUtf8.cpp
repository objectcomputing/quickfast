// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldUtf8.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldUtf8::FieldUtf8(const std::string & value)
  : Field(Field::UTF8, true)
  , value_(value)
{
}

FieldUtf8::FieldUtf8(const uchar * buffer, size_t length)
  : Field(Field::UTF8, true)
  , value_(std::string(reinterpret_cast<const char *>(buffer), length))
{
}


FieldUtf8::FieldUtf8()
  : Field(Field::UTF8, false)
  , value_()
{
}

FieldUtf8::~FieldUtf8()
{
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
