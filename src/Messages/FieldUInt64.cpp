// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldUInt64.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldUInt64::FieldUInt64(uint64 value)
  : Field(true)
  , value_(value)
{
}

FieldUInt64::FieldUInt64()
  : Field(false)
  , value_(0)
{
}

FieldUInt64::~FieldUInt64()
{
}

Field::FieldType
FieldUInt64::getType() const
{
  return Field::UINT64;
}

uint64
FieldUInt64::toUInt64() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return value_;
}

FieldCPtr
FieldUInt64::create(uint64 value)
{
  return new FieldUInt64(value);
}

FieldCPtr
FieldUInt64::createNull()
{
  return new FieldUInt64;
}
