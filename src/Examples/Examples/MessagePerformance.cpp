// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Examples/ExamplesPch.h>
#include "MessagePerformance.h"
#include <Messages/Message.h>
////////////#include <Messages/Field.h>
#include <Messages/Sequence.h>
#include <Messages/Group.h>

using namespace QuickFAST;
using namespace Examples;

MessagePerformance::MessagePerformance(size_t recordLimit, size_t recordDuplicate)
: recordLimit_(recordLimit)
, recordDuplicate_(recordDuplicate)
, recordCount_(0)
{
}

MessagePerformance::~MessagePerformance()
{
}

bool
MessagePerformance::reportDecodingError(const std::string & errorMessage)
{
  std::cerr << "Decoding error: " << errorMessage << std::endl;
  return false;
}

bool
MessagePerformance::reportCommunicationError(const std::string & errorMessage)
{
  std::cerr << "Communication error: " << errorMessage << std::endl;
  return false;
}

void
MessagePerformance::decodingStarted()
{
}

void
MessagePerformance::decodingStopped()
{
}


bool
MessagePerformance::consumeMessage(Messages::Message & message)
{
  recordCount_ += 1;
  try
  {
    for(size_t nDup = 0; nDup < recordDuplicate_; ++nDup)
    {
      retrieveMessage(message);
    }
  }
  catch (const std::exception & ex)
  {
    std::cerr << "Caught exception: " << ex.what() << std::endl;
    return false;
  }
  return recordLimit_ == 0 || recordCount_ < recordLimit_;
}

void
MessagePerformance::retrieveMessage(const Messages::Message & message)
{
  for( Messages::Message::const_iterator it = message.begin();
    it != message.end();
    ++it)
  {
    const Messages::FieldIdentityCPtr & identity = it->getIdentity();
    const Messages::FieldCPtr & field = it->getField();
    ValueType::Type type = field->getType();
    if(type == ValueType::SEQUENCE)
    {
      retrieveSequence(identity, field);
    }
    else if(type == ValueType::GROUP)
    {
      retrieveGroup(field);
    }
    else
    {
      retrieveFieldValue(field);
    }
  }
}
void
MessagePerformance::retrieveSequence(
                                     const Messages::FieldIdentityCPtr & identity,
                                     const Messages::FieldCPtr & field)
{
  Messages::SequenceCPtr sequence = field->toSequence();
  size_t count = sequence->size();
  size_t entryCount = 0;
  for(Messages::Sequence::const_iterator it = sequence->begin();
    it != sequence->end();
    ++it)
  {
    Messages::FieldSetCPtr fieldSet = *it;
    for(Messages::FieldSet::const_iterator fsit = fieldSet->begin();
      fsit != fieldSet->end();
      ++fsit)
    {
      // todo: refactor with message decoding
      const Messages::FieldIdentityCPtr & identity = fsit->getIdentity();
      const Messages::FieldCPtr & field = fsit->getField();
      ValueType::Type type = field->getType();
      if(type == ValueType::SEQUENCE)
      {
        retrieveSequence(identity, field);
      }
      else if(type == ValueType::GROUP)
      {
        retrieveGroup(field);
      }
      else
      {
        retrieveFieldValue(field);
      }
    }
  }
}

void
MessagePerformance::retrieveGroup(const Messages::FieldCPtr & field)
{
  Messages::GroupCPtr group = field->toGroup();
  for(Messages::FieldSet::const_iterator fsit = group->begin();
    fsit != group->end();
    ++fsit)
  {
    const Messages::FieldIdentityCPtr & identity = fsit->getIdentity();
    const Messages::FieldCPtr & field = fsit->getField();
    ValueType::Type type = field->getType();
    if(type == ValueType::SEQUENCE)
    {
      retrieveSequence(identity, field);
    }
    else if(type == ValueType::GROUP)
    {
      retrieveGroup(field);
    }
    else
    {
      retrieveFieldValue(field);
    }
  }

}


void
MessagePerformance::retrieveFieldValue(const Messages::FieldCPtr & field)
{
  switch(field->getType())
  {
  case ValueType::INT32:
    {
      volatile int32 value = field->toInt32();
      break;
    }
  case ValueType::UINT32:
    {
      volatile uint32 value = field->toUInt32();
      break;
    }
  case ValueType::INT64:
    {
      volatile int64 value = field->toInt64();
      break;
    }
  case ValueType::UINT64:
    {
      volatile uint64 value = field->toUInt64();
      break;
    }
  case ValueType::DECIMAL:
    {
      volatile Decimal value = field->toDecimal();
      break;
    }
  case ValueType::ASCII:
    {
      volatile std::string value = field->toAscii();
      break;
    }
  case ValueType::UTF8:
    {
      volatile std::string value = field->toUtf8();
      break;
    }
  case ValueType::BYTEVECTOR:
    {
      // todo: we probably should hex dump this
      volatile std::string value = field->toByteVector();
      break;
    }
  case ValueType::SEQUENCE:
  case ValueType::GROUP:
  default:
    {
      std::cerr << "unknown field type" << std::endl;
      break;
    }
  }

}


bool
MessagePerformance::wantLog(unsigned short level)
{
  return false;
}

bool
MessagePerformance::logMessage(unsigned short level, const std::string & logMessage)
{
  return true;
}


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
PerformanceBuilder::addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const int64 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const uint64 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const int32 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const uint32 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const int16 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const uint16 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const int8 value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const uchar value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const Decimal& value)
{
  ++fieldCount_;
}

void
PerformanceBuilder::addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const unsigned char * value, size_t length)
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
  Messages::FieldIdentityCPtr identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t fieldCount,
  Messages::FieldIdentityCPtr lengthIdentity,
  size_t length)
{
  ++sequenceCount_;
  return *this;
}

void
PerformanceBuilder::endSequence(
  Messages::FieldIdentityCPtr identity,
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
  Messages::FieldIdentityCPtr identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  ++groupCount_;
  return *this;
}

void
PerformanceBuilder::endGroup(
  Messages::FieldIdentityCPtr identity,
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

