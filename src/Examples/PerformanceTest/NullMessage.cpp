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
NullMessage::clear(size_t capacity)
{
}

void
NullMessage::reserve(size_t capacity)
{
}

size_t
NullMessage::size()const
{
  return size_;
}

void
NullMessage::addField(const Messages::FieldIdentityCPtr & identity, const Messages::FieldCPtr & value)
{
  ++ size_;
}

bool
NullMessage::getIdentity(const std::string &name, Messages::FieldIdentityCPtr & identity) const
{
  return false;
}

void
NullMessage::setApplicationType(const std::string & type, const std::string & ns)
{
  applicationType_ = type;
  applicationTypeNamespace_ = ns;

}

const std::string &
NullMessage::getApplicationType()const
{
  return applicationType_;
}

const std::string &
NullMessage::getApplicationTypeNs()const
{
  return applicationTypeNamespace_;
}

void
NullMessage::startSequence(
  Messages::FieldIdentityCPtr identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  throw QuickFAST::TemplateDefinitionError("NullMessage does not use Sequences");
}

Messages::MessageBuilder &
NullMessage::startSequenceEntry(
    const std::string & applicationType,
    const std::string & applicationTypeNamespace,
    size_t size)
{
  throw QuickFAST::TemplateDefinitionError("NullMessage does not use Sequences");
}

void
NullMessage::endSequenceEntry(Messages::MessageBuilder & entry)
{
  throw QuickFAST::TemplateDefinitionError("NullMessage does not use Sequences");
}

void
NullMessage::endSequence( Messages::FieldIdentityCPtr identity)
{
  throw QuickFAST::TemplateDefinitionError("NullMessage does not use Sequences");
}



Messages::MessageBuilder &
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
  Messages::MessageBuilder & entry)
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
NullMessageConsumer::consumeMessage(Examples::NullMessage & message)
{
  messageCount_ += 1;
  return true;
}

bool
NullMessageConsumer::consumeMessage(Messages::MessageBuilder & message)
{
  // shouldn't happen.
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

