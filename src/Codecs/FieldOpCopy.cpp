// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldOpCopy.h"
#include <Codecs/FieldInstruction.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;
bool
FieldOpCopy::usesPresenceMap(bool mandatory)const
{
  return true;
}

bool
FieldOpCopy::usesDictionary() const
{
  return true;
}

bool
FieldOpCopy::decode(
  const Codecs::FieldInstruction & instruction,
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::DecodedFields & fieldSet) const
{
  return instruction.decodeCopy(source, pmap, decoder, fieldSet);
}

void
FieldOpCopy::encode(
  const Codecs::FieldInstruction & instruction,
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::FieldSet & fieldSet) const
{
  return instruction.encodeCopy(destination, pmap, encoder, fieldSet);
}

void
FieldOpCopy::setDefaultValue(
  Codecs::FieldInstruction & instruction) const
{
  return instruction.setDefaultValueCopy();
}
