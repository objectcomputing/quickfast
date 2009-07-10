// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionFiller.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionFiller::FieldInstructionFiller(
  const std::string & name,
  const std::string & fieldNamespace)
  : FieldInstruction(name, fieldNamespace)
{
}

FieldInstructionFiller::FieldInstructionFiller()
{
}

FieldInstructionFiller::~FieldInstructionFiller()
{
}

bool
FieldInstructionFiller::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::MessageBuilder & fieldSet) const
{
  bool present = pmap.checkNextField();
  assert(!present);
  return true;
}

void
FieldInstructionFiller::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & fieldSet) const
{
  pmap.setNextField(false);
}

size_t
FieldInstructionFiller::presenceMapBitsRequired() const
{
  return 1;
}


size_t
FieldInstructionFiller::fieldCount(const SegmentBody & parent)const
{
  return 0;
}

void
FieldInstructionFiller::indexDictionaries(
  DictionaryIndexer & indexer,
  const std::string & dictionaryName,
  const std::string & typeName,
  const std::string & typeNamespace)
{
  return;
}

void
FieldInstructionFiller::interpretValue(const std::string & ignored)
{
  return;
}
