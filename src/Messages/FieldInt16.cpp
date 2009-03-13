// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInt16.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldInt16::FieldInt16(int16 value)
  : Field(true)
  , value_(value)
{
}

FieldInt16::FieldInt16()
  : Field(false)
  , value_(0)
{
}

FieldInt16::~FieldInt16()
{
}

Field::FieldType
FieldInt16::getType() const
{
  return Field::INT16;
}

int16
FieldInt16::toInt16() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return value_;
}

FieldCPtr
FieldInt16::create(int16 value)
{
  return new FieldInt16(value);
}

FieldCPtr
FieldInt16::createNull()
{
  return new FieldInt16;
}
