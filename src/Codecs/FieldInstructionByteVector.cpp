// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionByteVector.h"
#include <Messages/FieldByteVector.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionByteVector::FieldInstructionByteVector(
  const std::string & name,
  const std::string & fieldNamespace)
  : FieldInstructionBlob(ValueType::BYTEVECTOR, name, fieldNamespace)
{
  initialValue_ = Messages::FieldByteVector::createNull();
}

FieldInstructionByteVector::FieldInstructionByteVector()
{
  initialValue_ = Messages::FieldByteVector::createNull();
}

FieldInstructionByteVector::~FieldInstructionByteVector()
{
}

Messages::FieldCPtr
FieldInstructionByteVector::createField(const uchar * buffer, size_t length)const
{
  return Messages::FieldCPtr(Messages::FieldByteVector::create(buffer, length));
}

Messages::FieldCPtr
FieldInstructionByteVector::createField(const std::string & value)const
{
  return Messages::FieldCPtr(Messages::FieldByteVector::create(value));
}

Messages::FieldCPtr
FieldInstructionByteVector::createEmptyField()const
{
  return Messages::FieldCPtr(Messages::FieldByteVector::createNull());
}

