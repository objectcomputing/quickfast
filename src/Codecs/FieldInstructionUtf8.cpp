// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionUtf8.h"
#include <Messages/FieldUtf8.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionUtf8::FieldInstructionUtf8(
      const std::string & name,
      const std::string & fieldNamespace)
  : FieldInstructionBlob(ValueType::UTF8, name, fieldNamespace)
{
  initialValue_ = Messages::FieldUtf8::createNull();
}

FieldInstructionUtf8::FieldInstructionUtf8()
{
  initialValue_ = Messages::FieldUtf8::createNull();
}

FieldInstructionUtf8::~FieldInstructionUtf8()
{
}

Messages::FieldCPtr
FieldInstructionUtf8::createField(const uchar * buffer, size_t length)const
{
  return Messages::FieldUtf8::create(buffer, length);
}

Messages::FieldCPtr
FieldInstructionUtf8::createField(const std::string & value)const
{
  return Messages::FieldUtf8::create(value);
}

Messages::FieldCPtr
FieldInstructionUtf8::createEmptyField()const
{
  return Messages::FieldUtf8::createNull();
}
