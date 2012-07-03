// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Examples/ExamplesPch.h>
#include "MessagePerformance.h"
#include <Messages/Message.h>
#include <Messages/Sequence.h>
#include <Messages/Group.h>

using namespace QuickFAST;
using namespace Examples;
//////////////////////////
// Performance Builder Implementation

PerformanceBuilder::PerformanceBuilder()
  : fieldCount_(0)
  , msgCount_(0)
  , sequenceCount_(0)
  , sequenceEntryCount_(0)
  , groupCount_(0)
{
}

PerformanceBuilder::~PerformanceBuilder()
{
}

const std::string &
PerformanceBuilder::getApplicationType()const
{
  static const std::string type("type");
  return type;
}

const std::string &
PerformanceBuilder::getApplicationTypeNs()const
{
  static const std::string ns("typens");
  return ns;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int64 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint64 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int32 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint32 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int16 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint16 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int8 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uchar value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const Decimal& value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const unsigned char * value, size_t length)
{
  ++fieldCount_;
}

Messages::ValueMessageBuilder &
PerformanceBuilder::startMessage(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  ++msgCount_;
  return *this;
}

bool
PerformanceBuilder::endMessage(Messages::ValueMessageBuilder & messageBuilder)
{
  return true;
}

bool
PerformanceBuilder::ignoreMessage(Messages::ValueMessageBuilder & messageBuilder)
{
  return true;
}

Messages::ValueMessageBuilder &
PerformanceBuilder::startSequence(
  const Messages::FieldIdentity & identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t fieldCount,
  const Messages::FieldIdentity & lengthIdentity,
  size_t length)
{
  ++sequenceCount_;
  return *this;
}

void
PerformanceBuilder::endSequence(
  const Messages::FieldIdentity & identity,
  Messages::ValueMessageBuilder & sequenceBuilder)
{
}

Messages::ValueMessageBuilder &
PerformanceBuilder::startSequenceEntry(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  ++sequenceEntryCount_;
  return *this;
}

void
PerformanceBuilder::endSequenceEntry(Messages::ValueMessageBuilder & entry)
{
}

Messages::ValueMessageBuilder &
PerformanceBuilder::startGroup(
  const Messages::FieldIdentity & identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  ++groupCount_;
  return *this;
}

void
PerformanceBuilder::endGroup(
  const Messages::FieldIdentity & identity,
  Messages::ValueMessageBuilder & groupBuilder)
{
}

/////////////////////////////
// Implement Logger interface
bool
PerformanceBuilder::wantLog(unsigned short level)
{
  return false;
}
bool
PerformanceBuilder::logMessage(unsigned short level, const std::string & logMessage)
{
  return true;
}
bool
PerformanceBuilder::reportDecodingError(const std::string & errorMessage)
{
  return true;
}
bool
PerformanceBuilder::reportCommunicationError(const std::string & errorMessage)
{
  return true;
}

