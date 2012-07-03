// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#include "QuickFASTDotNetPch.h"
#include "ImplBuilderBase.h"
#include <DotNet/ImplMessageDeliverer.h>

#pragma unmanaged

using namespace QuickFAST;
using namespace DotNet;

ImplBuilderBase::ImplBuilderBase(ImplMessageDeliverer & deliverer)
  : deliverer_(deliverer)
  , logLimit_(Common::Logger::QF_LOG_WARNING)
{
}

ImplBuilderBase::~ImplBuilderBase()
{
}


void
ImplBuilderBase::setApplicationType(const std::string & applicationType, const std::string & applicationTypeNamespace)
{
  applicationType_ = applicationType;
  applicationTypeNs_ = applicationTypeNamespace;
}

void
ImplBuilderBase::setLogLimit(unsigned short limit)
{
  logLimit_ = limit;
}

const std::string &
ImplBuilderBase::getApplicationType()const
{
  return applicationType_;
}

const std::string &
ImplBuilderBase::getApplicationTypeNs()const
{
  return applicationTypeNs_;
}

void
ImplBuilderBase::addValue(const Messages::FieldIdentity &  identity, ValueType::Type type, const int64 value)
{
  throw std::logic_error("Unexpected call to MessageBuilder::addValue");
}

void
ImplBuilderBase::addValue(const Messages::FieldIdentity &  identity, ValueType::Type type, const uint64 value)
{
  throw std::logic_error("Unexpected call to MessageBuilder::addValue");
}

void
ImplBuilderBase::addValue(const Messages::FieldIdentity &  identity, ValueType::Type type, const int32 value)
{
  throw std::logic_error("Unexpected call to MessageBuilder::addValue");
}

void
ImplBuilderBase::addValue(const Messages::FieldIdentity &  identity, ValueType::Type type, const uint32 value)
{
  throw std::logic_error("Unexpected call to MessageBuilder::addValue");
}

void
ImplBuilderBase::addValue(const Messages::FieldIdentity &  identity, ValueType::Type type, const int16 value)
{
  throw std::logic_error("Unexpected call to MessageBuilder::addValue");
}

void
ImplBuilderBase::addValue(const Messages::FieldIdentity &  identity, ValueType::Type type, const uint16 value)
{
  throw std::logic_error("Unexpected call to MessageBuilder::addValue");
}

void
ImplBuilderBase::addValue(const Messages::FieldIdentity &  identity, ValueType::Type type, const int8 value)
{
  throw std::logic_error("Unexpected call to MessageBuilder::addValue");
}

void
ImplBuilderBase::addValue(const Messages::FieldIdentity &  identity, ValueType::Type type, const uchar value)
{
  throw std::logic_error("Unexpected call to MessageBuilder::addValue");
}

void
ImplBuilderBase::addValue(const Messages::FieldIdentity &  identity, ValueType::Type type, const Decimal& value)
{
  throw std::logic_error("Unexpected call to MessageBuilder::addValue");
}

void
ImplBuilderBase::addValue(const Messages::FieldIdentity &  identity, ValueType::Type type, const unsigned char * value, size_t length)
{
  throw std::logic_error("Unexpected call to MessageBuilder::addValue");
}


Messages::ValueMessageBuilder &
ImplBuilderBase::startMessage(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  throw std::logic_error("Unexpected call to MessageBuilder::startMessage");
}

bool
ImplBuilderBase::endMessage(Messages::ValueMessageBuilder & messageBuilder)
{
  throw std::logic_error("Unexpected call to MessageBuilder::endMessage");
}

bool
ImplBuilderBase::ignoreMessage(Messages::ValueMessageBuilder & messageBuilder)
{
  throw std::logic_error("Unexpected call to MessageBuilder::ignoreMessage");
}

Messages::ValueMessageBuilder &
ImplBuilderBase::startSequence(
  const Messages::FieldIdentity &  identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t fieldCount,
  const Messages::FieldIdentity &  lengthIdentity,
  size_t length)
{
  throw std::logic_error("Unexpected call to MessageBuilder::startSequence");
}

void
ImplBuilderBase::endSequence(
  const Messages::FieldIdentity &  identity,
  Messages::ValueMessageBuilder & sequenceBuilder)
{
  throw std::logic_error("Unexpected call to MessageBuilder::startSequenceEntry");
}

Messages::ValueMessageBuilder &
ImplBuilderBase::startSequenceEntry(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  throw std::logic_error("Unexpected call to MessageBuilder::endSequence");
}

void
ImplBuilderBase::endSequenceEntry(Messages::ValueMessageBuilder & entry)
{
  throw std::logic_error("Unexpected call to MessageBuilder::endSequenceEntry");
}

Messages::ValueMessageBuilder &
ImplBuilderBase::startGroup(
  const Messages::FieldIdentity &  identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  throw std::logic_error("Unexpected call to MessageBuilder::startGroup");
}

void
ImplBuilderBase::endGroup(
  const Messages::FieldIdentity &  identity,
  Messages::ValueMessageBuilder & groupBuilder)
{
  throw std::logic_error("Unexpected call to MessageBuilder::endGroup");
}

bool
ImplBuilderBase::wantLog(unsigned short level)
{
  return level >= logLimit_;
}

bool
ImplBuilderBase::logMessage(unsigned short level, const std::string & logMessage)
{
  if(level >= logLimit_)
  {
    return deliverer_.logMessage(level, logMessage);
  }
  return true;
}

bool
ImplBuilderBase::reportDecodingError(const std::string & errorMessage)
{
  return deliverer_.reportDecodingError(errorMessage);
}

bool
ImplBuilderBase::reportCommunicationError(const std::string & errorMessage)
{
  return deliverer_.reportDecodingError(errorMessage);
}
