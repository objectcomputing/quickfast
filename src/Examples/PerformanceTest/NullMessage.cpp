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
NullMessage::addField(const Messages::FieldIdentityCPtr & identity, const Messages::FieldCPtr & value)
{
  ++ size_;
}

Messages::ValueMessageBuilder &
NullMessage::startSequence(
  Messages::FieldIdentityCPtr /*identity*/,
  const std::string & /*applicationType*/,
  const std::string & /*applicationTypeNamespace*/,
  size_t /*fieldCount*/,
  Messages::FieldIdentityCPtr /*lengthIdentity*/,
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
NullMessage::endSequence( Messages::FieldIdentityCPtr identity, Messages::ValueMessageBuilder & )
{
  throw QuickFAST::TemplateDefinitionError("NullMessage does not use Sequences");
}



Messages::ValueMessageBuilder &
NullMessage::startGroup(
    Messages::FieldIdentityCPtr identity,
    const std::string & applicationType,
    const std::string & applicationTypeNamespace,
    size_t size)
{
  throw QuickFAST::TemplateDefinitionError("NullMessage does not use Group");
}

void
NullMessage::endGroup(
  Messages::FieldIdentityCPtr identity,
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

