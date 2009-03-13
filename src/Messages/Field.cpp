// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "Field.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

Field::Field(bool valid)
: valid_(valid)
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
  UnsupportedConversion ex("Field cannot be converted to UInt8");
  throw ex;
}

int8
Field::toInt8() const
{
  UnsupportedConversion ex("Field cannot be converted to Int8");
  throw ex;
}

uint16
Field::toUInt16() const
{
  UnsupportedConversion ex("Field cannot be converted to UInt16");
  throw ex;
}

int16
Field::toInt16() const
{
  UnsupportedConversion ex("Field cannot be converted to Int16");
  throw ex;
}

uint32
Field::toUInt32() const
{
  UnsupportedConversion ex("Field cannot be converted to UInt32");
  throw ex;
}

int32
Field::toInt32() const
{
  UnsupportedConversion ex("Field cannot be converted to Int32");
  throw ex;
}

uint64
Field::toUInt64() const
{
  UnsupportedConversion ex("Field cannot be converted to UInt64");
  throw ex;
}

int64
Field::toInt64() const
{
  UnsupportedConversion ex("Field cannot be converted to Int64");
  throw ex;
}

const std::string &
Field::toAscii() const
{
  UnsupportedConversion ex("Field cannot be converted to Ascii");
  throw ex;
}

const std::string &
Field::toUtf8() const
{
  UnsupportedConversion ex("Field cannot be converted to UTF8");
  throw ex;
}

const std::string &
Field::toByteVector()const
{
  UnsupportedConversion ex("Field cannot be converted to ByteVector");
  throw ex;
}

bool
Field::isString()const
{
  return false;
}

const std::string &
Field::toString()const
{
  UnsupportedConversion ex("Field cannot be converted to String");
  throw ex;
}

const Decimal &
Field::toDecimal() const
{
  UnsupportedConversion ex("Field cannot be converted to Decimal");
  throw ex;
}


const Messages::GroupCPtr &
Field::toGroup() const
{
  UnsupportedConversion ex("Field cannot be converted to Group");
  throw ex;
}



const Messages::SequenceCPtr &
Field::toSequence() const
{
  UnsupportedConversion ex("Field cannot be converted to Sequence");
  throw ex;
}

void
Field::freeField()const
{
  delete this;
}
#if 0
void
Messages::intrusive_ptr_add_ref(const Messages::Field * ptr)
{
  ++ptr->refcount_;
}

void
Messages::intrusive_ptr_release(const Messages::Field * ptr)
{
  if(--ptr->refcount_ == 0)
  {
    ptr->freeField();
  }
}

#endif
