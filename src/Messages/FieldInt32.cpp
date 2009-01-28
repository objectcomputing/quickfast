// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInt32.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldInt32::FieldInt32(int32 value)
  : Field(true)
  , value_(value)
{
}

FieldInt32::FieldInt32()
  : Field(false)
  , value_(0)
{
}

FieldInt32::~FieldInt32()
{
}

Field::FieldType
FieldInt32::getType() const
{
  return Field::INT32;
}

void
FieldInt32::setValue(int32 value)
{
  value_ = value;
  valid_ = true;
}

int32
FieldInt32::toInt32() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return value_;
}

FieldCPtr
FieldInt32::create(int32 value)
{
  return new FieldInt32(value);
}

FieldCPtr
FieldInt32::createNull()
{
  return new FieldInt32;
}
