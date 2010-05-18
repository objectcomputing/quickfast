// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldUInt64.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldUInt64::FieldUInt64(uint64 value)
  : Field(ValueType::UINT64, true)
{
  unsignedInteger_ = value;
}

FieldUInt64::FieldUInt64()
  : Field(ValueType::UINT64, false)
{
}

FieldUInt64::~FieldUInt64()
{
}

uint64
FieldUInt64::toUInt64() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return unsignedInteger_;
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

void
FieldUInt64::valueToStringBuffer() const
{
  std::stringstream buffer;
  buffer << unsignedInteger_;
  string_.assign(reinterpret_cast<const unsigned char *>(buffer.str().data()), buffer.str().size());
}

bool
FieldUInt64::isUnsignedInteger()const
{
  return true;
}
