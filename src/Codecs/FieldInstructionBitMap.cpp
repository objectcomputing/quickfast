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

bool
FieldInstructionBitMap::decodeFromSource(
  Codecs::DataSource & source,
  bool mandatory,
  WorkingBuffer & buffer,
  Messages::FieldCPtr & field) const
{
  int todo;
  return false;
}


bool
FieldInstructionBitMap::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::DecodedFields & fieldSet) const
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
  const Messages::FieldSet & fieldSet) const
{
  // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    BitMap value = field->toBitMap();
#if 0
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
      encoder.reportFatal("[ERR U9]", "Missing mandatory field.");
    }
    destination.putByte(nullBitMap);
#endif
  }
}


void
FieldInstructionBitMap::interpretValue(const std::string & value)
{
  return;
}
