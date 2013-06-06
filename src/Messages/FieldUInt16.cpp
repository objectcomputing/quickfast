// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldUInt16.h"
#include <Common/Exceptions.h>
using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldCPtr FieldUInt16::nullField_ = new FieldUInt16;

FieldUInt16::FieldUInt16(uint16 value)
  : Field(ValueType::UINT16, true)
{
  unsignedInteger_ = value;
}

FieldUInt16::FieldUInt16()
  : Field(ValueType::UINT16, false)
{
}

FieldUInt16::~FieldUInt16()
{
}

uint16
FieldUInt16::toUInt16() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return static_cast<uint16>(unsignedInteger_);
}

FieldCPtr
FieldUInt16::create(uint16 value)
{
  return new
    FieldUInt16(value);
}

void
FieldUInt16::freeField()const
{
  delete this;
}


FieldCPtr
FieldUInt16::createNull()
{
  return nullField_;
}

void
FieldUInt16::valueToStringBuffer() const
{
  std::stringstream buffer;
  buffer << unsignedInteger_;
  string_.assign(reinterpret_cast<const unsigned char *>(buffer.str().data()), buffer.str().size());
}

bool
FieldUInt16::isUnsignedInteger()const
{
  return true;
}
