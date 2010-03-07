// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "QuickFASTDotNetPch.h"
#include <DotNet/DNField.h>
#include <DotNet/DNFieldSet.h>

#include <DotNet/ImplField.h>
#include <DotNet/StringConversion.h>

using namespace QuickFAST;
using namespace DotNet;

DNField::DNField(ImplField & impl)
  : impl_(&impl)
{
}

DNField::~DNField()
{
}

System::String ^
DNField::localName()
{
  return string_cast(impl_->localName());
}

System::String ^
DNField::fieldNamespace()
{
  return string_cast(impl_->fieldNamespace());
}

System::String ^
DNField::id()
{
  return string_cast(impl_->id());
}

bool
DNField::isDefined()
{
  return impl_->isDefined();
}

FieldType
DNField::type()
{
  return impl_->type();
}

int
DNField::toInt32()
{
  return impl_->toInt32();
}

unsigned int
DNField::toUInt32()
{
  return impl_->toUInt32();
}

__int64
DNField::toInt64()
{
  return impl_->toInt64();
}

unsigned __int64
DNField::toUInt64()
{
  return impl_->toUInt64();
}

short
DNField::toInt16()
{
  return impl_->toInt16();
}

unsigned short
DNField::toUInt16()
{
  return impl_->toUInt16();
}

signed char
DNField::toInt8()
{
  return impl_->toInt8();
}

unsigned char
DNField::toUInt8()
{
  return impl_->toUInt8();
}

DNDecimal
DNField::toDecimal()
{
  return impl_->toDecimal();
}

System::String ^
DNField::toAscii()
{
  return string_cast(impl_->toAscii());
}

System::String ^
DNField::toUtf8()
{
  return string_cast(impl_->toUtf8());
}

System::String ^
DNField::toByteVector()
{
  return string_cast(impl_->toByteVector());
}

DNFieldSet ^
DNField::toGroup()
{
  return gcnew DNFieldSet(impl_->toGroup());
}

DNSequence ^
DNField::toSequence()
{
  return gcnew DNSequence(impl_->toSequence());
}

System::String ^
DNField::asString()
{
  return string_cast(impl_->asString());
}

QuickFAST::mantissa_t
DNField::mantissa()
{
  return impl_->mantissa();
}


QuickFAST::exponent_t
DNField::exponent()
{
  return impl_->exponent();
}

