// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "Field.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

Field::Field(FieldType type, bool valid)
: type_(type)
, valid_(valid)
, refcount_(0)
{
}

Field::~Field()
{
}

const std::string &
Field::typeName(FieldType type)
{
  switch(type)
  {
  case INT8:
    {
      static const std::string sts("int8");
      return sts;
    }
  case UINT8:
    {
      static const std::string sts("uint8");
      return sts;
    }
  case INT16:
    {
      static const std::string sts("int16");
      return sts;
    }
  case UINT16:
    {
      static const std::string sts("uint16");
      return sts;
    }
  case INT32:
    {
      static const std::string sts("int32");
      return sts;
    }
  case UINT32:
    {
      static const std::string sts("uint32");
      return sts;
    }
  case INT64:
    {
      static const std::string sts("int64");
      return sts;
    }
  case UINT64:
    {
      static const std::string sts("uint64");
      return sts;
    }
  case DECIMAL:
    {
      static const std::string sts("decimal");
      return sts;
    }
  case ASCII:
    {
      static const std::string sts("ascii");
      return sts;
    }
  case UTF8:
    {
      static const std::string sts("utf8");
      return sts;
    }
  case BYTEVECTOR:
    {
      static const std::string sts("bytevector");
      return sts;
    }
  case SEQUENCE:
    {
      static const std::string sts("sequence");
      return sts;
    }
  case GROUP:
    {
      static const std::string sts("group");
      return sts;
    }
  default:
    {
      static const std::string sts("Unknown");
      return sts;
    }
  }
}


bool
Field::isDefined() const
{
  return valid_;
}

uchar
Field::toUInt8() const
{
  UnsupportedConversion ex(typeName(getType()),"UInt8");
  throw ex;
}

int8
Field::toInt8() const
{
  UnsupportedConversion ex(typeName(getType()), "Int8");
  throw ex;
}

uint16
Field::toUInt16() const
{
  UnsupportedConversion ex(typeName(getType()), "UInt16");
  throw ex;
}

int16
Field::toInt16() const
{
  UnsupportedConversion ex(typeName(getType()), "Int16");
  throw ex;
}

uint32
Field::toUInt32() const
{
  UnsupportedConversion ex(typeName(getType()), "UInt32");
  throw ex;
}

int32
Field::toInt32() const
{
  UnsupportedConversion ex(typeName(getType()), "Int32");
  throw ex;
}

uint64
Field::toUInt64() const
{
  UnsupportedConversion ex(typeName(getType()), "UInt64");
  throw ex;
}

int64
Field::toInt64() const
{
  UnsupportedConversion ex(typeName(getType()), "Int64");
  throw ex;
}

const StringBuffer &
Field::toAscii() const
{
  UnsupportedConversion ex(typeName(getType()), "Ascii");
  throw ex;
}

const StringBuffer &
Field::toUtf8() const
{
  UnsupportedConversion ex(typeName(getType()), "UTF8");
  throw ex;
}

const StringBuffer &
Field::toByteVector()const
{
  UnsupportedConversion ex(typeName(getType()), "ByteVector");
  throw ex;
}

const BitMap &
Field::toBitMap() const
{
  UnsupportedConversion ex(typeName(getType()), "BitMap");
  throw ex;
}


bool
Field::isString()const
{
  return false;
}

const StringBuffer &
Field::toString()const
{
  UnsupportedConversion ex(typeName(getType()), "String");
  throw ex;
}

const Decimal
Field::toDecimal() const
{
  UnsupportedConversion ex(typeName(getType()), "Decimal");
  throw ex;
}


const Messages::GroupCPtr &
Field::toGroup() const
{
  UnsupportedConversion ex(typeName(getType()), "Group");
  throw ex;
}

const Messages::SequenceCPtr &
Field::toSequence() const
{
  UnsupportedConversion ex(typeName(getType()), "Sequence");
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
  UnsupportedConversion ex(typeName(getType()), "string");
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

