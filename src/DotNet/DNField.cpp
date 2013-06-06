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
DNField::LocalName::get()
{
  return string_cast(impl_->localName());
}

System::String ^
DNField::FieldNamespace::get()
{
  return string_cast(impl_->fieldNamespace());
}

System::String ^
DNField::Id::get()
{
  return string_cast(impl_->id());
}

bool
DNField::IsDefined::get()
{
  return impl_->isDefined();
}

FieldType
DNField::Type::get()
{
  return static_cast<FieldType>(impl_->type());
}

int
DNField::Int32::get()
{
  return impl_->toInt32();
}

unsigned int
DNField::UInt32::get()
{
  return impl_->toUInt32();
}

__int64
DNField::Int64::get()
{
  return impl_->toInt64();
}

unsigned __int64
DNField::UInt64::get()
{
  return impl_->toUInt64();
}

short
DNField::Int16::get()
{
  return impl_->toInt16();
}

unsigned short
DNField::UInt16::get()
{
  return impl_->toUInt16();
}

signed char
DNField::Int8::get()
{
  return impl_->toInt8();
}

unsigned char
DNField::UInt8::get()
{
  return impl_->toUInt8();
}

DNDecimal
DNField::Decimal::get()
{
  DNDecimal value(impl_->toDecimal());
  return value;
}

System::String ^
DNField::Ascii::get()
{
  return string_cast(impl_->toAscii());
}

System::String ^
DNField::Utf8::get()
{
  return string_cast(impl_->toUtf8());
}

System::String ^
DNField::ByteVector::get()
{
  return string_cast(impl_->toByteVector());
}

DNFieldSet ^
DNField::Group::get()
{
  return gcnew DNFieldSet(impl_->toGroup());
}

DNSequence ^
DNField::Sequence::get()
{
  return gcnew DNSequence(impl_->toSequence());
}

System::String ^
DNField::DisplayString::get()
{
  return string_cast(impl_->asString());
}

QuickFAST::mantissa_t
DNField::Mantissa::get()
{
  return impl_->mantissa();
}


QuickFAST::exponent_t
DNField::Exponent::get()
{
  return impl_->exponent();
}

