// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldOpDelta.h"
#include <Codecs/FieldInstruction.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;
bool
FieldOpDelta::usesPresenceMap(bool /*mandatory*/)const
{
  // the Delta value is always present in the stream.
  return false;
}

bool
FieldOpDelta::usesDictionary() const
{
  return true;
}

void
FieldOpDelta::decode(
  const Codecs::FieldInstruction & instruction,
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & fieldSet) const
{
  instruction.decodeDelta(source, pmap, decoder, fieldSet);
}

void
FieldOpDelta::encode(
  const Codecs::FieldInstruction & instruction,
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & fieldSet) const
{
  return instruction.encodeDelta(destination, pmap, encoder, fieldSet);
}

void
FieldOpDelta::setDefaultValue(
  Codecs::FieldInstruction & instruction) const
{
  return instruction.setDefaultValueDelta();
}
FieldOp::OpType
FieldOpDelta::opType()const
{
  return FieldOp::DELTA;
}
