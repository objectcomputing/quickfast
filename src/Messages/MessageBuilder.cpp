// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "MessageBuilder.h"
#include <Messages/FieldInt64.h>
#include <Messages/FieldUInt64.h>
#include <Messages/FieldInt32.h>
#include <Messages/FieldUInt32.h>
#include <Messages/FieldInt16.h>
#include <Messages/FielduInt16.h>
#include <Messages/FieldInt8.h>
#include <Messages/FieldUInt8.h>
#include <Messages/FieldDecimal.h>
#include <Messages/FieldString.h>
#include <Messages/FieldAscii.h>
#include <Messages/FieldUtf8.h>
#include <Messages/FieldByteVector.h>

using namespace QuickFAST;
using namespace Messages;
////////////////////////////////
// Implement ValueMessageBuilder
void MessageBuilder::addValue(const Messages::FieldIdentityCPtr & identity, Field::FieldType type, const int64 value)
{
  FieldCPtr field(FieldInt64::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(const Messages::FieldIdentityCPtr & identity, Field::FieldType type, const uint64 value)
{
  FieldCPtr field(FieldUInt64::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(const Messages::FieldIdentityCPtr & identity, Field::FieldType type, const int32 value)
{
  FieldCPtr field(FieldInt32::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(const Messages::FieldIdentityCPtr & identity, Field::FieldType type, const uint32 value)
{
  FieldCPtr field(FieldUInt32::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(const Messages::FieldIdentityCPtr & identity, Field::FieldType type, const int16 value)
{
  FieldCPtr field(FieldInt16::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(const Messages::FieldIdentityCPtr & identity, Field::FieldType type, const uint16 value)
{
  FieldCPtr field(FieldUInt16::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(const Messages::FieldIdentityCPtr & identity, Field::FieldType type, const int8 value)
{
  FieldCPtr field(FieldInt8::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(const Messages::FieldIdentityCPtr & identity, Field::FieldType type, const uchar value)
{
  FieldCPtr field(FieldUInt8::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(const Messages::FieldIdentityCPtr & identity, Field::FieldType type, const Decimal& value)
{
  FieldCPtr field(FieldDecimal::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(const Messages::FieldIdentityCPtr & identity, Field::FieldType type, const unsigned char * value, size_t length)
{
  switch (type)
  {
  case Field::ASCII:
    addField(identity, FieldAscii::create(value, length));
    break;
  case Field::UTF8:
    addField(identity, FieldUtf8::create(value, length));
    break;
  case Field::BYTEVECTOR:
    addField(identity, FieldByteVector::create(value, length));
    break;
  default:
    addField(identity, FieldString::create(value, length));
  }
}
