// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "QuickFASTDotNetPch.h"
#include <DotNet/ImplField.h>
#include <DotNet/ImplSequence.h>
#include <DotNet/ImplFieldSet.h>
#pragma unmanaged
#include <Common/Decimal.h>
#include <Messages/FieldIdentity.h>

using namespace QuickFAST;
using namespace DotNet;

ImplField::ImplField()
  : type_(ValueType::UNDEFINED)
  , unsignedInteger_(0)
  , signedInteger_(0)
  , exponent_(0)
  , group_(nullptr)
{
}

ImplField::~ImplField()
{
}

void
ImplField::setUnsignedValue(ValueType::Type type, unsigned long long value)
{
  type_ = type;
  unsignedInteger_ = value;
}

void
ImplField::setSignedValue(ValueType::Type type, long long value)
{
  type_ = type;
  signedInteger_ = value;
}

void
ImplField::setDecimalValue(Decimal value)
{
  type_ = ValueType::DECIMAL;
  signedInteger_ = value.getMantissa();
  exponent_ = value.getExponent();
}

void
ImplField::setStringValue(ValueType::Type type, const std::string & value)
{
  type_ = type;
  string_ = value;
}

void
ImplField::setSequence(ImplSequence * sequence)
{
  type_ = ValueType::SEQUENCE;
  sequence_.reset(sequence);
}

void
ImplField::setGroup(ImplFieldSet * group)
{
  type_ = ValueType::GROUP;
  group_.reset(group);
}

const std::string &
ImplField::localName()
{
  return identity_.getLocalName();
}

const std::string &
ImplField::fieldNamespace()
{
  return identity_.getNamespace();
}


const std::string &
ImplField::id()
{
  return identity_.id();
}


bool
ImplField::isDefined()
{
  return type_ != ValueType::UNDEFINED;
}


ValueType::Type
ImplField::type()
{
  return type_;
}


int
ImplField::toInt32()
{
  return static_cast<int>(signedInteger_);
}


unsigned int
ImplField::toUInt32()
{
  return static_cast<unsigned int>(unsignedInteger_);
}


__int64
ImplField::toInt64()
{
  return signedInteger_;
}


unsigned __int64
ImplField::toUInt64()
{
  return unsignedInteger_;
}


short
ImplField::toInt16()
{
  return static_cast<short>(signedInteger_);
}


unsigned short
ImplField::toUInt16()
{
  return static_cast<unsigned short>(unsignedInteger_);
}

signed char
ImplField::toInt8()
{
  return static_cast<signed char>(signedInteger_);
}


unsigned char
ImplField::toUInt8()
{
  return static_cast<unsigned char>(unsignedInteger_);
}

Decimal
ImplField::toDecimal()
{
  Decimal value(signedInteger_, exponent_);
  return value;
}


const std::string &
ImplField::toAscii()
{
  return string_;
}


const std::string &
ImplField::toUtf8()
{
  return string_;
}


const std::string &
ImplField::toByteVector()
{
  return string_;
}

ImplFieldSet &
ImplField::toGroup()
{
  return *group_;
}

ImplSequence &
ImplField::toSequence()
{
  return *sequence_;
}

const std::string &
ImplField::asString()
{
  if(string_.empty())
  {
    switch(type_)
    {
    case ValueType::INT8:
    case ValueType::INT16:
    case ValueType::INT32:
    case ValueType::INT64:
    case ValueType::MANTISSA:
    case ValueType::EXPONENT:
      {
        std::stringstream v;
        v << signedInteger_;
        string_ = v.str();
        break;
      }

    case ValueType::UINT8:
    case ValueType::UINT16:
    case ValueType::UINT32:
    case ValueType::UINT64:
    case ValueType::LENGTH:
      {
        std::stringstream v;
        v << unsignedInteger_;
        string_ = v.str();
        break;
      }
      {
        std::stringstream v;
        v << unsignedInteger_;
        string_ = v.str();
        break;
      }

    case ValueType::DECIMAL:
      {
        Decimal decimal(signedInteger_, exponent_);
        decimal.toString(string_);
        break;
      }

    case ValueType::ASCII:
    case ValueType::UTF8:
    case ValueType::BYTEVECTOR:
      {
        break;
      }

    case ValueType::SEQUENCE:
      {
        string_ = "Sequence";
        break;
      }
    case ValueType::GROUP:
      {
        string_ = "Group";
        break;
      }
    case ValueType::BITMAP:
    case ValueType::UNDEFINED:
      {
        string_= "Undefined";
      }
    }
  }
  return string_;
}


QuickFAST::mantissa_t
ImplField::mantissa()
{
  return static_cast<int>(signedInteger_);
}


QuickFAST::exponent_t
ImplField::exponent()
{
  return exponent_;
}

