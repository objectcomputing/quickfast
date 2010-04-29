// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInt32.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldInt32::FieldInt32(int32 value)
  : Field(ValueType::INT32, true)
{
  signedInteger_ = value;
}

FieldInt32::FieldInt32()
  : Field(ValueType::INT32, false)
{
}

FieldInt32::~FieldInt32()
{
}

int32
FieldInt32::toInt32() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return static_cast<int32>(signedInteger_);
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

void
FieldInt32::valueToStringBuffer() const
{
  std::stringstream buffer;
  buffer << signedInteger_;
  string_.assign(reinterpret_cast<const unsigned char *>(buffer.str().data()), buffer.str().size());
}

bool
FieldInt32::isSignedInteger()const
{
  return true;
}
