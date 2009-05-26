// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldByteVector.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldByteVector::FieldByteVector(const std::string & value)
  : Field(Field::BYTEVECTOR, true)
  , value_(value)
{
}

FieldByteVector::FieldByteVector(const uchar * buffer, size_t length)
  : Field(Field::BYTEVECTOR, true)
  , value_(std::string(reinterpret_cast<const char *>(buffer), length))
{
}

FieldByteVector::FieldByteVector()
  : Field(Field::BYTEVECTOR, false)
  , value_()
{
}

FieldByteVector::~FieldByteVector()
{
}

bool
FieldByteVector::isString() const
{
  return true;
}

const std::string &
FieldByteVector::toByteVector() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return value_;
}


const std::string &
FieldByteVector::toString() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return value_;
}

FieldCPtr
FieldByteVector::create(const std::string & value)
{
  return new FieldByteVector(value);
}

FieldCPtr
FieldByteVector::create(const uchar * buffer, size_t length)
{
  return new FieldByteVector(buffer, length);
}

FieldCPtr
FieldByteVector::createNull()
{
  return new FieldByteVector;
}

