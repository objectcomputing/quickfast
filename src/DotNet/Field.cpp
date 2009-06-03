// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include <DotNet/Field.h>
#include <DotNet/FieldSet.h>
#include <DotNet/Sequence.h>
#include <DotNet/Decimal.h>
#include <DotNet/StlDotNet.h>
#include <DotNet/Exceptions.h>
#include <DotNet/ManagedHelpers.h>

#include "Messages/FieldAscii.h"
#include "Messages/FieldByteVector.h"
#include "Messages/FieldDecimal.h"
#include "Messages/FieldGroup.h"
#include "Messages/FieldInt32.h"
#include "Messages/FieldInt64.h"
#include "Messages/FieldUInt32.h"
#include "Messages/FieldUInt64.h"
#include "Messages/FieldSequence.h"
#include "Messages/FieldSet.h"
#include "Messages/FieldUtf8.h"
#include "Messages/FieldInt8.h"
#include "Messages/FieldUInt8.h"
#include "Messages/FieldInt16.h"
#include "Messages/FieldUInt16.h"
#include "Messages/FieldBitmap.h"

using namespace QuickFASTDotNet::Messages;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Text;
using namespace cli;

using namespace QuickFASTDotNet;
using namespace Messages;

bool
Field::isDefined::get()
{
  return cppField_->isDefined();
}


FieldType
Field::Type::get()
{
  return static_cast<FieldType>(cppField_->getType());
}

int
Field::toInt32::get()
{
  return cppField_->toInt32();
}

unsigned int
Field::toUInt32::get()
{
  return cppField_->toUInt32();
}
__int64
Field::toInt64::get()
{
  return cppField_->toInt64();
}
unsigned __int64
Field::toUInt64::get()
{
  return cppField_->toUInt64();
}

short
Field::toInt16::get()
{
  return cppField_->toInt16();
}

unsigned short
Field::toUInt16::get()
{
  return cppField_->toUInt16();
}

char
Field::toInt8::get()
{
  return cppField_->toInt8();
}

unsigned char
Field::toUInt8::get()
{
  return cppField_->toUInt8();
}

::QuickFASTDotNet::Decimal
Field::toDecimal::get()
{
  return ::QuickFASTDotNet::Decimal(cppField_->toDecimal());
}

System::String ^
Field::toAscii::get()
{
  return gcnew System::String(cppField_->toAscii().c_str());
}

System::String ^
Field::toUtf8::get()
{
  return gcnew System::String(cppField_->toUtf8().c_str());
}

cli::array<unsigned char> ^
Field::toByteVector::get()
{
  std::string v = cppField_->toByteVector();
  cli::array<unsigned char>^ result = gcnew cli::array<unsigned char>(v.length());
  int count = static_cast<int>(v.length());
  for(int p = 0; p < count; ++p)
  {
    result->SetValue(static_cast<unsigned char>(v[p]), p);
  }
  return result;
}

FieldSet ^
Field::toGroup::get()
{
  return gcnew FieldSet(cppField_->toGroup());
}

Sequence ^
Field::toSequence::get()
{
  return gcnew Sequence(cppField_->toSequence());
}
