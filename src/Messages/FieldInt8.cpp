// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInt8.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldInt8::FieldInt8(int8 value)
  : Field(true)
  , value_(value)
{
}

FieldInt8::FieldInt8()
  : Field(false)
  , value_(0)
{
}

FieldInt8::~FieldInt8()
{
}

Field::FieldType
FieldInt8::getType() const
{
  return Field::INT8;
}

int8
FieldInt8::toInt8() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return value_;
}

FieldCPtr
FieldInt8::create(int8 value)
{
  return new FieldInt8(value);
}

FieldCPtr
FieldInt8::createNull()
{
  return new FieldInt8;
}
