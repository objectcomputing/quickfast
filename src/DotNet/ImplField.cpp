// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "QuickFASTDotNetPch.h"
#include <DotNet/ImplField.h>
#include <DotNet/ImplSequence.h>
#include <DotNet/ImplFieldSet.h>
#include <DotNet/DNFieldSet.h>

using namespace QuickFAST;
using namespace DotNet;

ImplField::ImplField()
  : type_(FieldType::UNDEFINED)
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
ImplField::setUnsignedValue(FieldType type, unsigned long long value)
{
  type_ = type;
  unsignedInteger_ = value;
}

void
ImplField::setSignedValue(FieldType type, long long value)
{
  type_ = type;
  signedInteger_ = value;
}

void
ImplField::setDecimalValue(Decimal value)
{
  type_ = FieldType::DECIMAL;
  signedInteger_ = value.getMantissa();
  exponent_ = value.getExponent();
}

void
ImplField::setStringValue(FieldType type, const std::string & value)
{
  type_ = type;
  string_ = value;
}

void
ImplField::setSequence(ImplSequence * sequence)
{
  type_ = FieldType::SEQUENCE;
  sequence_.reset(sequence);
}

void
ImplField::setGroup(ImplFieldSet * group)
{
  type_ = FieldType::GROUP;
  group_.reset(group);
}

void
ImplField::setLocalName(const std::string & localName)
{
  localName_ = localName;
}

void
ImplField::setFieldNamespace(const std::string & fieldNamespace)
{
  fieldNamespace_ = fieldNamespace;
}

void
ImplField::setId(const std::string & id)
{
  id_ = id;
}




const std::string &
ImplField::localName()
{
  return localName_;
}

const std::string &
ImplField::fieldNamespace()
{
  return fieldNamespace_;
}


const std::string &
ImplField::id()
{
  return id_;
}


bool
ImplField::isDefined()
{
  return type_ != FieldType::UNDEFINED;
}


FieldType
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

DNDecimal
ImplField::toDecimal()
{
  DNDecimal value(signedInteger_, exponent_);
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
    case FieldType::INT8:
    case FieldType::INT16:
    case FieldType::INT32:
    case FieldType::INT64:
      {
        std::stringstream v;
        v << signedInteger_;
        string_ = v.str();
        break;
      }

    case FieldType::UINT8:
    case FieldType::UINT16:
    case FieldType::UINT32:
    case FieldType::UINT64:
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

    case FieldType::DECIMAL:
      {
        Decimal decimal(signedInteger_, exponent_);
        decimal.toString(string_);
        break;
      }

    case FieldType::ASCII:
    case FieldType::UTF8:
    case FieldType::BYTEVECTOR:
      {
        break;
      }

    case FieldType::SEQUENCE:
      {
        string_ = "Sequence";
        break;
      }
    case FieldType::GROUP:
      {
        string_ = "Group";
        break;
      }
    case FieldType::BITMAP:
    case FieldType::UNDEFINED:
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

