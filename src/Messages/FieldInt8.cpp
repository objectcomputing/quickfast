// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInt8.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldInt8::FieldInt8(int8 value)
  : Field(ValueType::INT8, true)
{
  signedInteger_ = value;
}

FieldInt8::FieldInt8()
  : Field(ValueType::INT8, false)
{
}

FieldInt8::~FieldInt8()
{
}

int8
FieldInt8::toInt8() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return static_cast<int8>(signedInteger_);
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

void
FieldInt8::valueToStringBuffer() const
{
  std::stringstream buffer;
  buffer << signedInteger_;
  string_.assign(reinterpret_cast<const unsigned char *>(buffer.str().data()), buffer.str().size());
}

bool
FieldInt8::isSignedInteger()const
{
  return true;
}
