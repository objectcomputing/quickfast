// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldUInt32.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldUInt32::FieldUInt32(uint32 value)
  : Field(true)
  , value_(value)
{
}

FieldUInt32::FieldUInt32()
  : Field(false)
  , value_(0)
{
}

FieldUInt32::~FieldUInt32()
{
}

Field::FieldType
FieldUInt32::getType()const
{
  return Field::UINT32;
}

void
FieldUInt32::setValue(uint32 value)
{
  value_ = value;
  valid_ = true;
}

uint32
FieldUInt32::toUInt32() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return value_;
}

FieldCPtr
FieldUInt32::create(uint32 value)
{
  return new FieldUInt32(value);
}

FieldCPtr
FieldUInt32::createNull()
{
  return new FieldUInt32;
}
