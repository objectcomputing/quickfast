// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldAscii.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldAscii::FieldAscii(const std::string & value)
  : Field(true)
  , value_(value)
{
}

FieldAscii::FieldAscii(const uchar * value, size_t length)
  : Field(true)
  , value_(reinterpret_cast<const char *>(value), length)
{
}

FieldAscii::FieldAscii()
  : Field(false)
  , value_()
{
}

FieldAscii::~FieldAscii()
{
}

Field::FieldType
FieldAscii::getType() const
{
  return Field::ASCII;
}

bool
FieldAscii::isString() const
{
  return true;
}

const std::string &
FieldAscii::toString() const
{
  return toAscii();
}

const std::string &
FieldAscii::toAscii() const
{
  if(!valid_)
  {
    throw FieldNotPresent("Field not present");
  }
  return value_;
}

FieldCPtr
FieldAscii::create(const std::string & value)
{
  return new FieldAscii(value);
}

FieldCPtr
FieldAscii::create(const uchar * buffer, size_t length)
{
  return new FieldAscii(buffer, length);
}

FieldCPtr
FieldAscii::createNull()
{
  return new FieldAscii;
}

