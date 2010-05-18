// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldUtf8.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldUtf8::FieldUtf8(const std::string & value)
  : Field(ValueType::UTF8, true)
{
  string_ = value;
  cachedValue_ = string_;
}

FieldUtf8::FieldUtf8(const uchar * buffer, size_t length)
  : Field(ValueType::UTF8, true)
{
  string_.assign(buffer, length);
  cachedValue_ = string_;
}


FieldUtf8::FieldUtf8()
  : Field(ValueType::UTF8, false)
{
}

FieldUtf8::~FieldUtf8()
{
}

bool
FieldUtf8::isString() const
{
  return true;
}

const StringBuffer &
FieldUtf8::toUtf8() const
{
  return toString();
}

FieldCPtr
FieldUtf8::create(const std::string & value)
{
  return new FieldUtf8(value);
}

FieldCPtr
FieldUtf8::create(const uchar * buffer, size_t length)
{
  return new FieldUtf8(buffer, length);
}

FieldCPtr
FieldUtf8::createNull()
{
  return new FieldUtf8;
}
