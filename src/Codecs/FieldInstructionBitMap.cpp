// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionBitMap.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/DataDestination.h>
#include <Codecs/Encoder.h>
#include <Messages/Message.h>
#include <Messages/FieldBitMap.h>

#include <Common/Profiler.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionBitMap::FieldInstructionBitMap(
  const std::string & name,
  const std::string & fieldNamespace)
: FieldInstruction(name, fieldNamespace)
{
  initialValue_ = Messages::FieldBitMap::createNull();
}

FieldInstructionBitMap::FieldInstructionBitMap()
{
  initialValue_ = Messages::FieldBitMap::createNull();
}

FieldInstructionBitMap::~FieldInstructionBitMap()
{
}

namespace
{
  void
  appendByte(WorkingBuffer & buffer, unsigned short & savedBits, size_t & savedBitCount, uchar byte)
  {
    savedBits <<= savedBitCount;
    savedBits |= (byte << 1);
    if(savedBitCount == 0)
    {
      savedBitCount = 7;
    }
    else
    {
      savedBits <<=(8 - savedBitCount);
      buffer.push(savedBits >> 8);
      savedBitCount -= 1;
    }
  }
}

bool
FieldInstructionBitMap::decodeFromSource(
  Codecs::DataSource & source,
  bool mandatory,
  WorkingBuffer & buffer,
  Messages::FieldCPtr & field) const
{
  buffer.clear(false);
  unsigned short savedBits = 0;
  size_t savedBitCount = 0;
  uchar byte;
  if(!source.getByte(byte))
  {
    return false;
  }
  while((byte & stopBit) == 0)
  {
    appendByte(buffer, savedBits, savedBitCount, byte);
    if(!source.getByte(byte))
    {
      // todo: exception?
      return false;
    }
  }
  // write the last byte
  appendByte(buffer, savedBits, savedBitCount, byte);
  // and flush it to the buffer if necessary
  appendByte(buffer, savedBits, savedBitCount, 0);
  field = Messages::FieldBitMap::create(buffer.begin(), buffer.size());
  return false;
}


bool
FieldInstructionBitMap::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::MessageBuilder & fieldSet) const
{
  // note NOP never uses pmap.  It uses a null value instead for optional fields
  // so it's always safe to do the basic decode.
  Messages::FieldCPtr field;
  if(!decodeFromSource(source, isMandatory(), decoder.getWorkingBuffer(), field))
  {
    return false;
  }
  if(field)
  {
    fieldSet.addField(identity_, field);
  }
  return true;
}



void
FieldInstructionBitMap::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & fieldSet) const
{
  // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    BitMap value = field->toBitMap();
#if 0 /// encoding not supported (yet)
    if(!isMandatory())
    {
      encodeNullableBitMap(destination, value);
    }
    else
    {
      encodeBitMap(destination, value);
    }
  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.");
    }
    destination.putByte(nullBitMap);
#endif
  }
}


void
FieldInstructionBitMap::interpretValue(const std::string & value)
{
  /// initialization not supported yet.
  return;
}
