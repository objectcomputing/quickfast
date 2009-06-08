// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldOpIncrement.h"
#include <Codecs/FieldInstruction.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;
bool
FieldOpIncrement::usesPresenceMap(bool mandatory)const
{
  return true;
}

bool
FieldOpIncrement::usesDictionary() const
{
  return true;
}

bool
FieldOpIncrement::decode(
  const Codecs::FieldInstruction & instruction,
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::MessageBuilder & fieldSet) const
{
  return instruction.decodeIncrement(source, pmap, decoder, fieldSet);
}

void
FieldOpIncrement::encode(
  const Codecs::FieldInstruction & instruction,
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::FieldSet & fieldSet) const
{
  return instruction.encodeIncrement(destination, pmap, encoder, fieldSet);
}

void
FieldOpIncrement::setDefaultValue(
  Codecs::FieldInstruction & instruction) const
{
  return instruction.setDefaultValueIncrement();
}

