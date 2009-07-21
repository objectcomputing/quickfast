// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Examples/ExamplesPch.h>
#include "MessagePerformance.h"
#include <Messages/Message.h>
#include <Messages/Field.h>
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
MessagePerformance::retrieveMessage(const Messages::Message & decodedFields)
{
  const Messages::Message & message = dynamic_cast<const Messages::Message &>(decodedFields);
  for( Messages::Message::const_iterator it = message.begin();
    it != message.end();
    ++it)
  {
    const Messages::FieldIdentityCPtr & identity = it->getIdentity();
    const Messages::FieldCPtr & field = it->getField();
    Messages::Field::FieldType type = field->getType();
    if(type == Messages::Field::SEQUENCE)
    {
      retrieveSequence(identity, field);
    }
    else if(type == Messages::Field::GROUP)
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
      Messages::Field::FieldType type = field->getType();
      if(type == Messages::Field::SEQUENCE)
      {
        retrieveSequence(identity, field);
      }
      else if(type == Messages::Field::GROUP)
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
    Messages::Field::FieldType type = field->getType();
    if(type == Messages::Field::SEQUENCE)
    {
      retrieveSequence(identity, field);
    }
    else if(type == Messages::Field::GROUP)
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
  case Messages::Field::INT32:
    {
      volatile int32 value = field->toInt32();
      break;
    }
  case Messages::Field::UINT32:
    {
      volatile uint32 value = field->toUInt32();
      break;
    }
  case Messages::Field::INT64:
    {
      volatile int64 value = field->toInt64();
      break;
    }
  case Messages::Field::UINT64:
    {
      volatile uint64 value = field->toUInt64();
      break;
    }
  case Messages::Field::DECIMAL:
    {
      volatile Decimal value = field->toDecimal();
      break;
    }
  case Messages::Field::ASCII:
    {
      volatile std::string value = field->toAscii();
      break;
    }
  case Messages::Field::UTF8:
    {
      volatile std::string value = field->toUtf8();
      break;
    }
  case Messages::Field::BYTEVECTOR:
    {
      // todo: we probably should hex dump this
      volatile std::string value = field->toByteVector();
      break;
    }
  case Messages::Field::SEQUENCE:
  case Messages::Field::GROUP:
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
