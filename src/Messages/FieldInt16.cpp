// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInt16.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldInt16::FieldInt16(int16 value)
  : Field(ValueType::INT16, true)
{
  signedInteger_ = value;
}

FieldInt16::FieldInt16()
  : Field(ValueType::INT16, false)
{
}

FieldInt16::~FieldInt16()
{
}

int16
FieldInt16::toInt16() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return static_cast<int16>(signedInteger_);
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

void
FieldInt16::valueToStringBuffer() const
{
  std::stringstream buffer;
  buffer << signedInteger_;
  string_.assign(reinterpret_cast<const unsigned char *>(buffer.str().data()), buffer.str().size());
}

bool
FieldInt16::isSignedInteger()const
{
  return true;
}
