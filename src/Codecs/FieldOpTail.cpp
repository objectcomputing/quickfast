// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldOpTail.h"
#include <Codecs/FieldInstruction.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;
bool
FieldOpTail::usesPresenceMap(bool /*mandatory*/)const
{
  return true;
}

bool
FieldOpTail::usesDictionary() const
{
  return true;
}

void
FieldOpTail::decode(
  const Codecs::FieldInstruction & instruction,
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & fieldSet) const
{
  instruction.decodeTail(source, pmap, decoder, fieldSet);
}

void
FieldOpTail::encode(
  const Codecs::FieldInstruction & instruction,
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & fieldSet) const
{
  instruction.encodeTail(destination, pmap, encoder, fieldSet);
}

void
FieldOpTail::setDefaultValue(
  Codecs::FieldInstruction & instruction) const
{
  instruction.setDefaultValueTail();
}

FieldOp::OpType
FieldOpTail::opType()const
{
  return FieldOp::TAIL;
}
