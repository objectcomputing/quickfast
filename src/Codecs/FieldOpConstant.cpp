// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldOpConstant.h"
#include <Codecs/FieldInstruction.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;
bool
FieldOpConstant::usesPresenceMap(bool mandatory)const
{
  return !mandatory;
}

bool
FieldOpConstant::usesDictionary() const
{
  return false;
}

void
FieldOpConstant::decode(
  const Codecs::FieldInstruction & instruction,
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & fieldSet) const
{
  instruction.decodeConstant(source, pmap, decoder, fieldSet);
}

void
FieldOpConstant::encode(
  const Codecs::FieldInstruction & instruction,
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & fieldSet) const
{
  return instruction.encodeConstant(destination, pmap, encoder, fieldSet);
}

void
FieldOpConstant::setDefaultValue(
  Codecs::FieldInstruction & instruction) const
{
  return instruction.setDefaultValueConstant();
}

FieldOp::OpType
FieldOpConstant::opType()const
{
  return FieldOp::CONSTANT;
}
