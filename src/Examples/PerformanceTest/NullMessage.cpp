// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Examples/ExamplesPch.h>
#include "NullMessage.h"
#include <Common/Exceptions.h>

using namespace QuickFAST;
using namespace Examples;

NullMessage::NullMessage()
: size_(0)
{
}

NullMessage::NullMessage(size_t expectedNumberOfFields)
: size_(0)
{
}

NullMessage::NullMessage(const NullMessage & rhs)
: size_(rhs.size_)
, applicationType_(rhs.applicationType_)
, applicationTypeNamespace_(rhs.applicationTypeNamespace_)
{
}

void
NullMessage::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int64 value)
{
  ++ size_;
}

void
NullMessage::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint64 value)
{
  ++ size_;
}

void
NullMessage::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int32 value)
{
  ++ size_;
}

void
NullMessage::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint32 value)
{
  ++ size_;
}

void
NullMessage::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int16 value)
{
  ++ size_;
}

void
NullMessage::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint16 value)
{
  ++ size_;
}

void
NullMessage::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int8 value)
{
  ++ size_;
}

void
NullMessage::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uchar value)
{
  ++ size_;
}

void
NullMessage::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const Decimal& value)
{
  ++ size_;
}

void
NullMessage::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const unsigned char * value, size_t length)
{
  ++ size_;
}

Messages::ValueMessageBuilder &
NullMessage::startSequence(
  const Messages::FieldIdentity & /*identity*/,
  const std::string & /*applicationType*/,
  const std::string & /*applicationTypeNamespace*/,
  size_t /*fieldCount*/,
  const Messages::FieldIdentity & /*lengthIdentity*/,
  size_t /*length*/)
{
  throw QuickFAST::TemplateDefinitionError("NullMessage does not use Sequences");
}

Messages::ValueMessageBuilder &
NullMessage::startSequenceEntry(
    const std::string & applicationType,
    const std::string & applicationTypeNamespace,
    size_t size)
{
  throw QuickFAST::TemplateDefinitionError("NullMessage does not use Sequences");
}

void
NullMessage::endSequenceEntry(Messages::ValueMessageBuilder & entry)
{
  throw QuickFAST::TemplateDefinitionError("NullMessage does not use Sequences");
}

void
NullMessage::endSequence( const Messages::FieldIdentity & identity, Messages::ValueMessageBuilder & )
{
  throw QuickFAST::TemplateDefinitionError("NullMessage does not use Sequences");
}



Messages::ValueMessageBuilder &
NullMessage::startGroup(
    const Messages::FieldIdentity & identity,
    const std::string & applicationType,
    const std::string & applicationTypeNamespace,
    size_t size)
{
  throw QuickFAST::TemplateDefinitionError("NullMessage does not use Group");
}

void
NullMessage::endGroup(
  const Messages::FieldIdentity & identity,
  Messages::ValueMessageBuilder & entry)
{
  throw QuickFAST::TemplateDefinitionError("NullMessage does not use Group");
}

///////////////////////
// Null MessageConsumer

NullMessageConsumer::NullMessageConsumer()
  : messageCount_(0)
{
}

NullMessageConsumer::~NullMessageConsumer()
{
}

bool
NullMessageConsumer::consumeMessage(Messages::Message  & message)
{
  messageCount_ += 1;
  return true;
}

bool
NullMessageConsumer::wantLog(unsigned short level)
{
  return false;
}

bool
NullMessageConsumer::logMessage(unsigned short level, const std::string & logMessage)
{
  return true;
}

bool
NullMessageConsumer::reportDecodingError(const std::string & errorMessage)
{
  return false;
}

bool
NullMessageConsumer::reportCommunicationError(const std::string & errorMessage)
{
  return false;
}

void
NullMessageConsumer::decodingStopped()
{
}

