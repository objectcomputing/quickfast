// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldOpDefault.h"
#include <Codecs/FieldInstruction.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;
bool
FieldOpDefault::usesPresenceMap(bool mandatory)const
{
  return true;
}

bool
FieldOpDefault::usesDictionary() const
{
  return false;
}


bool
FieldOpDefault::decode(
  const Codecs::FieldInstruction & instruction,
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::FieldSet & fieldSet) const
{
  return instruction.decodeDefault(source, pmap, decoder, fieldSet);
}

void
FieldOpDefault::encode(
  const Codecs::FieldInstruction & instruction,
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::FieldSet & fieldSet) const
{
  return instruction.encodeDefault(destination, pmap, encoder, fieldSet);
}

void
FieldOpDefault::setDefaultValue(
  Codecs::FieldInstruction & instruction) const
{
  return instruction.setDefaultValueDefault();
}

