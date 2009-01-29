// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInt64.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldInt64::FieldInt64(int64 value)
  : Field(true)
  , value_(value)
{
}

FieldInt64::FieldInt64()
  : Field(false)
  , value_(0)
{
}

FieldInt64::~FieldInt64()
{
}

Field::FieldType
FieldInt64::getType() const
{
  return Field::INT64;
}

int64
FieldInt64::toInt64() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return value_;
}

FieldCPtr
FieldInt64::create(int64 value)
{
  return new FieldInt64(value);
}

FieldCPtr
FieldInt64::createNull()
{
  return new FieldInt64;
}
