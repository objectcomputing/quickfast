// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#include "QuickFASTDotNetPch.h"
#include "ImplFieldSetBuilder.h"

//#include <DotNet/DNMessageDeliverer.h>
#include <DotNet/ImplSequenceBuilder.h>
#include <DotNet/ImplFieldSet.h>
#include <DotNet/ImplField.h>
//#include <DotNet/StringConversion.h>
#pragma unmanaged
#include <Messages/FieldIdentity.h>

using namespace QuickFAST;
using namespace DotNet;
#pragma unmanaged

ImplFieldSetBuilder::ImplFieldSetBuilder(ImplMessageDeliverer & deliverer)
  : ImplBuilderBase(deliverer)
  , fieldSet_(0)
{
}

ImplFieldSetBuilder::~ImplFieldSetBuilder()
{
  delete fieldSet_;
}

ImplFieldSet *
ImplFieldSetBuilder::endSet()
{
  ImplFieldSet * result = fieldSet_;
  fieldSet_ = 0;
  return result;
}

void
ImplFieldSetBuilder::reset(size_t size)
{
  delete fieldSet_;
  fieldSet_ = new ImplFieldSet(size);
}

void
ImplFieldSetBuilder::addValue(
  const Messages::FieldIdentity &  identity,
  ValueType::Type type, const int64 value)
{
  ImplField & field = fieldSet_->append();
  field.setIdentity(identity);
  field.setSignedValue(type, value);
}

void
ImplFieldSetBuilder::addValue(
  const Messages::FieldIdentity &  identity,
  ValueType::Type type,
  const uint64 value)
{
  ImplField & field = fieldSet_->append();
  field.setIdentity(identity);
  field.setUnsignedValue(type, value);
}

void
ImplFieldSetBuilder::addValue(
  const Messages::FieldIdentity &  identity,
  ValueType::Type type,
  const int32 value)
{
  ImplField & field = fieldSet_->append();
  field.setIdentity(identity);
  field.setSignedValue(type, value);
}

void
ImplFieldSetBuilder::addValue(
  const Messages::FieldIdentity &  identity,
  ValueType::Type type,
  const uint32 value)
{
  ImplField & field = fieldSet_->append();
  field.setIdentity(identity);
  field.setUnsignedValue(type, value);
}

void
ImplFieldSetBuilder::addValue(
  const Messages::FieldIdentity &  identity,
  ValueType::Type type,
  const int16 value)
{
  ImplField & field = fieldSet_->append();
  field.setIdentity(identity);
  field.setSignedValue(type, value);
}

void
ImplFieldSetBuilder::addValue(
  const Messages::FieldIdentity &  identity,
  ValueType::Type type,
  const uint16 value)
{
  ImplField & field = fieldSet_->append();
  field.setIdentity(identity);
  field.setUnsignedValue(type, value);
}


void
ImplFieldSetBuilder::addValue(
  const Messages::FieldIdentity &  identity,
  ValueType::Type type,
  const int8 value)
{
  ImplField & field = fieldSet_->append();
  field.setIdentity(identity);
  field.setSignedValue(type, value);
}


void
ImplFieldSetBuilder::addValue(
  const Messages::FieldIdentity &  identity,
  ValueType::Type type,
  const uchar value)
{
  ImplField & field = fieldSet_->append();
  field.setIdentity(identity);
  field.setUnsignedValue(type, value);
}


void
ImplFieldSetBuilder::addValue(
  const Messages::FieldIdentity &  identity,
  ValueType::Type type,
  const Decimal& value)
{
  ImplField & field = fieldSet_->append();
  field.setIdentity(identity);
  field.setDecimalValue(value);
}


void
ImplFieldSetBuilder::addValue(
  const Messages::FieldIdentity &  identity,
  ValueType::Type type,
  const unsigned char * value,
  size_t length)
{
  ImplField & field = fieldSet_->append();
  field.setIdentity(identity);
  std::string v(reinterpret_cast<const char *>(value), length);
  field.setStringValue(type, v);
}


Messages::ValueMessageBuilder &
ImplFieldSetBuilder::startMessage(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  delete fieldSet_;
  fieldSet_ = new ImplFieldSet(size);
  return *this;
}
#pragma managed

bool
ImplFieldSetBuilder::endMessage(Messages::ValueMessageBuilder & messageBuilder)
{
  bool result = true;
  if(fieldSet_ && fieldSet_->size() > 0)
  {
    result = deliverer_.deliverMessage(*fieldSet_);
  }
  delete fieldSet_;
  fieldSet_ = 0;
  return result;
}

#pragma unmanaged

bool
ImplFieldSetBuilder::ignoreMessage(Messages::ValueMessageBuilder & messageBuilder)
{
  delete fieldSet_;
  fieldSet_ = 0;
  return true;
}

Messages::ValueMessageBuilder &
ImplFieldSetBuilder::startSequence(
  const Messages::FieldIdentity &  identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t fieldCount,
  const Messages::FieldIdentity &  lengthIdentity,
  size_t length)
{
  if(!sequenceBuilder_)
  {
    sequenceBuilder_.reset(new ImplSequenceBuilder(deliverer_));
  }
  sequenceBuilder_->initialize(lengthIdentity, length);
  return *sequenceBuilder_;
}

void
ImplFieldSetBuilder::endSequence(
  const Messages::FieldIdentity &  identity,
  Messages::ValueMessageBuilder & sequenceBuilder)
{
  ImplField & field = fieldSet_->append();
  field.setSequence(sequenceBuilder_->endSequence());
  field.setIdentity(identity);
  sequenceBuilder_.reset();
}

Messages::ValueMessageBuilder &
ImplFieldSetBuilder::startGroup(
  const Messages::FieldIdentity &  identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  if(!groupBuilder_)
  {
    groupBuilder_.reset(new ImplFieldSetBuilder(deliverer_));
  }
  groupBuilder_->reset(size);
  return *groupBuilder_;
}

void
ImplFieldSetBuilder::endGroup(
  const Messages::FieldIdentity &  identity,
  Messages::ValueMessageBuilder & groupBuilder)
{
  ImplField & field = fieldSet_->append();
  field.setGroup(groupBuilder_->endSet());
  field.setIdentity(identity);
}
#pragma managed
