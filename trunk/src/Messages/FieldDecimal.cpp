// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldDecimal.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldDecimal::FieldDecimal()
  : Field(false)
{
}

FieldDecimal::FieldDecimal(const Decimal & value)
  : Field(true)
  , value_(value)
{
}

FieldDecimal::~FieldDecimal()
{
}

Field::FieldType
FieldDecimal::getType() const
{
  return Field::DECIMAL;
}

void
FieldDecimal::setValue(Decimal value)
{
  value_ = value;
  valid_ = true;
}

const Decimal &
FieldDecimal::toDecimal() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return value_;
}

FieldCPtr
FieldDecimal::create(const Decimal & value)
{
  return new FieldDecimal(value);
}

FieldCPtr
FieldDecimal::createNull()
{
  return new FieldDecimal;
}
