// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "Field.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

Field::Field(ValueType::Type type, bool valid)
: type_(type)
, valid_(valid)
, refcount_(0)
{
}

Field::~Field()
{
}

bool
Field::isDefined() const
{
  return valid_;
}

uchar
Field::toUInt8() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()),"UInt8");
  throw ex;
}

int8
Field::toInt8() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "Int8");
  throw ex;
}

uint16
Field::toUInt16() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "UInt16");
  throw ex;
}

int16
Field::toInt16() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "Int16");
  throw ex;
}

uint32
Field::toUInt32() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "UInt32");
  throw ex;
}

int32
Field::toInt32() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "Int32");
  throw ex;
}

uint64
Field::toUInt64() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "UInt64");
  throw ex;
}

int64
Field::toInt64() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "Int64");
  throw ex;
}

const StringBuffer &
Field::toAscii() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "Ascii");
  throw ex;
}

const StringBuffer &
Field::toUtf8() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "UTF8");
  throw ex;
}

const StringBuffer &
Field::toByteVector()const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "ByteVector");
  throw ex;
}

const BitMap &
Field::toBitMap() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "BitMap");
  throw ex;
}


bool
Field::isString()const
{
  return false;
}

bool
Field::isUnsignedInteger()const
{
  return false;
}

bool
Field::isSignedInteger()const
{
  return false;
}

const StringBuffer &
Field::toString()const
{
  if(!valid_ || !isString())
  {
    UnsupportedConversion ex(ValueType::typeName(getType()), "String");
    throw ex;
  }
  return string_;
}

uint64
Field::toUnsignedInteger() const
{
  if(!valid_ || !isUnsignedInteger())
  {
    UnsupportedConversion ex(ValueType::typeName(getType()), "Unsigned Integer");
    throw ex;
  }
  return unsignedInteger_;
}

int64
Field::toSignedInteger() const
{
  if(!valid_ || !isSignedInteger())
  {
    UnsupportedConversion ex(ValueType::typeName(getType()), "Signed Integer");
    throw ex;
  }
  return signedInteger_;
}


const Decimal
Field::toDecimal() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "Decimal");
  throw ex;
}


const Messages::GroupCPtr &
Field::toGroup() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "Group");
  throw ex;
}

const Messages::SequenceCPtr &
Field::toSequence() const
{
  UnsupportedConversion ex(ValueType::typeName(getType()), "Sequence");
  throw ex;
}

void
Field::freeField()const
{
  delete this;
}

void
Field::valueToStringBuffer() const
{
  /// should never be called for stringable types
  /// should be overridden for non-stringable types
  UnsupportedConversion ex(ValueType::typeName(getType()), "string");
  throw ex;
}
bool
Field::operator == (const Field & rhs)const
{
  if(this->getType() != rhs.getType())
  {
    return false;
  }
  if(isString())
  {
    return string_ == rhs.string_;
  }
  else
  {
    // compare integral or decimal values.  Unused fields
    // will compare equal.
    return unsignedInteger_ == rhs.unsignedInteger_
        && signedInteger_ == rhs.signedInteger_
        && exponent_ == rhs.exponent_;
  }
}

