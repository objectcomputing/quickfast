// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInt64.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldInt64::FieldInt64(int64 value)
  : Field(ValueType::INT64, true)
{
  signedInteger_ = value;
}

FieldInt64::FieldInt64()
  : Field(ValueType::INT64, false)
{
}

FieldInt64::~FieldInt64()
{
}

int64
FieldInt64::toInt64() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return static_cast<int64>(signedInteger_);
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

void
FieldInt64::valueToStringBuffer() const
{
  std::stringstream buffer;
  buffer << signedInteger_;
  string_.assign(reinterpret_cast<const unsigned char *>(buffer.str().data()), buffer.str().size());
}

bool
FieldInt64::isSignedInteger()const
{
  return true;
}
