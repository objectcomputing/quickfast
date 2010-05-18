// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldAscii.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldAscii::FieldAscii(const std::string & value)
  : Field(ValueType::ASCII, true)
{
  string_ = value;
}

FieldAscii::FieldAscii(const uchar * value, size_t length)
  : Field(ValueType::ASCII, true)
{
  string_.assign(value, length);
}

FieldAscii::FieldAscii()
  : Field(ValueType::ASCII, false)
{
}

FieldAscii::~FieldAscii()
{
}

bool
FieldAscii::isString() const
{
  return true;
}

const StringBuffer &
FieldAscii::toAscii() const
{
  return toString();
}

FieldCPtr
FieldAscii::create(const std::string & value)
{
  return new FieldAscii(value);
}

FieldCPtr
FieldAscii::create(const uchar * buffer, size_t length)
{
  return new FieldAscii(buffer, length);
}

FieldCPtr
FieldAscii::createNull()
{
  return new FieldAscii;
}

