// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Examples/ExamplesPch.h>
#include "MessageInterpreter.h"
#include <Messages/Message.h>
#include <Messages/Sequence.h>
#include <Messages/Group.h>

using namespace QuickFAST;
using namespace Examples;

MessageInterpreter::MessageInterpreter(std::ostream & out, bool silent)
  : out_(out)
  , indent_(0)
  , recordCount_(0)
  , logLevel_(Messages::Logger::QF_LOG_WARNING)
  , silent_(silent)
{
}

MessageInterpreter::~MessageInterpreter()
{
}

bool
MessageInterpreter::wantLog(unsigned short level)
{
  return level <= logLevel_;
}

bool
MessageInterpreter::logMessage(unsigned short level, const std::string & logMessage)
{
  if(level <= logLevel_)
  {
    std::cerr << logMessage << std::endl;
  }
  return true;
}


bool
MessageInterpreter::reportDecodingError(const std::string & errorMessage)
{
  std::cerr << "Decoding error: " << errorMessage << std::endl;
  return false;
}

bool
MessageInterpreter::reportCommunicationError(const std::string & errorMessage)
{
  //out_
  std::cerr << "Communication error: " << errorMessage << std::endl;
  return false;
}

void
MessageInterpreter::decodingStarted()
{
  // ignore this.
}

void
MessageInterpreter::decodingStopped()
{
  out_ << "End of data" << std::endl;
}


bool
MessageInterpreter::consumeMessage(Messages::Message & message)
{
  recordCount_ += 1;
  out_ << "Record #" << recordCount_ << ' ' << std::flush;
  formatMessage(message);
  out_ << std::endl;
  return true;
}

void
MessageInterpreter::formatMessage(const Messages::Message & message)
{
  if(!silent_)
  {
  for( Messages::Message::const_iterator it = message.begin();
    it != message.end();
    ++it)
  {
    Messages::FieldIdentityCPtr & identity = it->getIdentity();
    const Messages::FieldCPtr & field = it->getField();
    ValueType::Type type = field->getType();
    if(type == ValueType::SEQUENCE)
    {
      formatSequence(identity, field);
    }
    else if(type == ValueType::GROUP)
    {
      formatGroup(field);
    }
    else
    {
      out_ << ' ' << identity->name() << '[' << identity->id() << "]=";
      displayFieldValue(field);
      }
    }
  }
}
void
MessageInterpreter::newline()
{
  out_ << std::endl << std::string(indent_ * 2, ' ');
}

void
MessageInterpreter::formatSequence(
  Messages::FieldIdentityCPtr & identity,
  const Messages::FieldCPtr & field)
{
  Messages::SequenceCPtr sequence = field->toSequence();
  size_t count = sequence->size();
  newline();
  out_ << ' ' << identity->name() << '[' << identity->id() << "]=";

  Messages::FieldIdentityCPtr lengthIdentity = sequence->getLengthIdentity();

  out_ << "Sequence: " << lengthIdentity->name() << '[' << lengthIdentity->id() << ']' << " = " << count;

  size_t entryCount = 0;
  ++indent_;
  for(Messages::Sequence::const_iterator it = sequence->begin();
    it != sequence->end();
    ++it)
  {
    newline();
    out_ << '[' << entryCount++ << "]: ";

    Messages::FieldSetCPtr fieldSet = *it;
    for(Messages::FieldSet::const_iterator fsit = fieldSet->begin();
      fsit != fieldSet->end();
      ++fsit)
    {
      // todo: refactor with message decoding
      Messages::FieldIdentityCPtr & identity = fsit->getIdentity();
      const Messages::FieldCPtr & field = fsit->getField();
      ValueType::Type type = field->getType();
      if(type == ValueType::SEQUENCE)
      {
        formatSequence(identity, field);
      }
      else if(type == ValueType::GROUP)
      {
        formatGroup(field);
      }
      else
      {
        out_ << ' ' << identity->name() << '[' << identity->id() << "]=";
        displayFieldValue(field);
      }
    }
  }
  --indent_;
  newline();
}

void
MessageInterpreter::formatGroup(const Messages::FieldCPtr & field)
{
  Messages::GroupCPtr group = field->toGroup();
  for(Messages::FieldSet::const_iterator fsit = group->begin();
    fsit != group->end();
    ++fsit)
  {
    // todo: refactor with message decoding
    Messages::FieldIdentityCPtr & identity = fsit->getIdentity();
    const Messages::FieldCPtr & field = fsit->getField();
    ValueType::Type type = field->getType();
    if(type == ValueType::SEQUENCE)
    {
      formatSequence(identity, field);
    }
    else if(type == ValueType::GROUP)
    {
      formatGroup(field);
    }
    else
    {
      out_ << ' ' << identity->name() << '[' << identity->id() << "]=";
      displayFieldValue(field);
    }
  }

}


void
MessageInterpreter::displayFieldValue(const Messages::FieldCPtr & field)
{
  switch(field->getType())
  {
  case ValueType::INT32:
    out_ << field->toInt32();
    break;
  case ValueType::UINT32:
    out_ << field->toUInt32();
    break;
  case ValueType::INT64:
    out_ << field->toInt64();
    break;
  case ValueType::UINT64:
    out_ << field->toUInt64();
    break;
  case ValueType::DECIMAL:
    {
      std::string value;
      field->toDecimal().toString(value);
      out_ << value;
      break;
    }
  case ValueType::ASCII:
    {
      out_ << field->toAscii();
      break;
    }
  case ValueType::UTF8:
    {
      out_ << field->toUtf8();
      break;
    }
  case ValueType::BYTEVECTOR:
    {
      // todo: we probably should hex dump this
      out_ << field->toByteVector();
      break;
    }
  case ValueType::SEQUENCE:
    {
      out_ << "sequence";
      break;
    }
  case ValueType::GROUP:
    {
      static const std::string sts("group");
      out_ << "group";
      break;
    }
  default:
    {
      out_ << "unknown field type";
      break;
    }
  }

}

