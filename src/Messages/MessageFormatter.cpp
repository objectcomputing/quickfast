// Copyright (c) 2009, 2010, 2011, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Common/QuickFASTPch.h>
#include "MessageFormatter.h"
#include <Messages/Message.h>
#include <Messages/FieldIdentity.h>
#include <Messages/Sequence.h>
#include <Messages/Group.h>

using namespace QuickFAST;
using namespace Messages;

MessageFormatter::MessageFormatter(std::ostream & out)
  : out_(out)
  , indent_(0)
{
}

MessageFormatter::~MessageFormatter()
{
}

void
MessageFormatter::formatMessage(const Messages::Message & message)
{
  if(!message.getApplicationType().empty())
  {
    out_ << " Type: ";
    if(!message.getApplicationTypeNs().empty())
    {
      out_ << message.getApplicationTypeNs() << '.';
    }
    out_ << message.getApplicationType() << ' ';
  }
  for( Messages::Message::const_iterator it = message.begin();
    it != message.end();
    ++it)
  {
    const Messages::FieldIdentity & identity = it->getIdentity();
    const Messages::FieldCPtr & field = it->getField();
    ValueType::Type type = field->getType();
    if(type == ValueType::SEQUENCE)
    {
      formatSequence(identity, field);
    }
    else if(type == ValueType::GROUP)
    {
      formatGroup(identity, field);
    }
    else
    {
      out_ << ' ' << identity.name() << '[' << identity.id() << "]=";
      displayFieldValue(field);
    }
  }
  newline();
}
void
MessageFormatter::newline()
{
  out_ << std::endl << std::string(indent_ * 2, ' ');
}

void
MessageFormatter::formatSequence(
  const Messages::FieldIdentity & identity,
  const Messages::FieldCPtr & field)
{
  Messages::SequenceCPtr sequence = field->toSequence();
  size_t count = sequence->size();
  newline();
  out_ << ' ' << identity.name() << '[' << identity.id() << "]=";

  const Messages::FieldIdentity & lengthIdentity = sequence->getLengthIdentity();

  out_ << "Sequence: " << lengthIdentity.name() << '[' << lengthIdentity.id() << ']' << " = " << count << " {";

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
      const Messages::FieldIdentity & identity = fsit->getIdentity();
      const Messages::FieldCPtr & field = fsit->getField();
      ValueType::Type type = field->getType();
      if(type == ValueType::SEQUENCE)
      {
        formatSequence(identity, field);
      }
      else if(type == ValueType::GROUP)
      {
        formatGroup(identity, field);
      }
      else
      {
        out_ << ' ' << identity.name() << '[' << identity.id() << "]=";
        displayFieldValue(field);
      }
    }
  }
  newline();
  out_ << '}';
  --indent_;
  newline();
}

void
MessageFormatter::formatGroup(
  const Messages::FieldIdentity & identity,
  const Messages::FieldCPtr & field)
{
  Messages::GroupCPtr group = field->toGroup();
  newline();
  out_ << ' ' << identity.name() << '[' << identity.id() << "] Group {"
    << group->getApplicationTypeNs() << ':' << group->getApplicationType()
    << "}= {";
  ++indent_;
  newline();

  for(Messages::FieldSet::const_iterator fsit = group->begin();
    fsit != group->end();
    ++fsit)
  {
    const Messages::FieldIdentity & identity = fsit->getIdentity();
    const Messages::FieldCPtr & field = fsit->getField();
    ValueType::Type type = field->getType();
    if(type == ValueType::SEQUENCE)
    {
      formatSequence(identity, field);
    }
    else if(type == ValueType::GROUP)
    {
      formatGroup(identity, field);
    }
    else
    {
      out_ << ' ' << identity.name() << '[' << identity.id() << "]=";
      displayFieldValue(field);
    }
  }
  newline();
  out_ << '}';
  --indent_;
  newline();
}

void
MessageFormatter::displayFieldValue(const Messages::FieldCPtr & field)
{
  switch(field->getType())
  {
  case ValueType::EXPONENT:
  case ValueType::INT32:
    out_ << field->toInt32();
    break;
  case ValueType::LENGTH:
  case ValueType::UINT32:
    out_ << field->toUInt32();
    break;
  case ValueType::MANTISSA:
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

