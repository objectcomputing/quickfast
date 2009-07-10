// C+opyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionSequence.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>
#include <Codecs/FieldInstructionUInt32.h>
#include <Messages/FieldSequence.h>
#include <Messages/Sequence.h>
#include <Messages/FieldUInt32.h>
#include <Messages/SingleValueBuilder.h>
#include <Messages/SpecialAccessors.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;


FieldInstructionSequence::FieldInstructionSequence(
  const std::string & name,
  const std::string & fieldNamespace)
  : FieldInstruction(name, fieldNamespace)
{
}

FieldInstructionSequence::FieldInstructionSequence()
{
}

FieldInstructionSequence::~FieldInstructionSequence()
{
}

bool
FieldInstructionSequence::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::MessageBuilder & messageBuilder) const
{
  if(!segment_)
  {
    decoder.reportFatal("[ERR U07]", "SegmentBody not defined for Sequence instruction.");
  }
  size_t length = 0;
  Codecs::FieldInstructionCPtr lengthInstruction;
  Messages::SingleValueBuilder<uint32> lengthSet;
  if(segment_->getLengthInstruction(lengthInstruction))
  {
    source.beginField(lengthInstruction->getIdentity()->name());
    if(!lengthInstruction->decode(source, pmap, decoder, lengthSet))
    {
      return false;
    }
  }
  else
  {
    FieldInstructionUInt32 defaultLengthInstruction;
    defaultLengthInstruction.setPresence(isMandatory());
    if(!defaultLengthInstruction.decode(source, pmap, decoder, lengthSet))
    {
      return false;
    }
  }
  if(!lengthSet.isSet())
  {
    // this optional sequence is not present
    return true;
  }
  length = lengthSet.value();

  messageBuilder.startSequence(
    identity_,
    segment_->getApplicationType(),
    segment_->getApplicationTypeNamespace(),
    segment_->fieldCount());

  for(size_t nEntry = 0; nEntry < length; ++nEntry)
  {
    if(decoder.getLogOut())
    {
      std::stringstream msg;
      msg << "Sequence entry #" << nEntry << " of " << length << std::ends;
      decoder.logMessage(msg.str());
    }

    Messages::MessageBuilder & entrySet(
      messageBuilder.startSequenceEntry(
        segment_->getApplicationType(),
        segment_->getApplicationTypeNamespace(),
        segment_->fieldCount()));
    decoder.decodeGroup(source, segment_, entrySet);
    messageBuilder.endSequenceEntry(entrySet);
  }
  messageBuilder.endSequence(identity_);

  return true;
}

void
FieldInstructionSequence::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & messageBuilder) const
{
  if(!segment_)
  {
    encoder.reportFatal("[ERR U07]", "SegmentBody not defined for Sequence instruction.");
  }

  // retrieve the field corresponding to this sequence
  Messages::FieldCPtr field;
  if(messageBuilder.getField(identity_->name(), field))
  {
    Messages::SequenceCPtr sequence = field->toSequence();
    size_t length = sequence->size();

    Messages::FieldCPtr lengthField(Messages::FieldUInt32::create(QuickFAST::uint32(length)));

    Codecs::FieldInstructionCPtr lengthInstruction;
    if(segment_->getLengthInstruction(lengthInstruction))
    {
      Messages::SingleFieldAccessor accessor(lengthInstruction->getIdentity(), lengthField);
      lengthInstruction->encode(destination, pmap, encoder, accessor);
    }
    else
    {
       FieldInstructionUInt32 defaultLengthInstruction;
       defaultLengthInstruction.setPresence(isMandatory());
       Messages::SingleFieldAccessor accessor(defaultLengthInstruction.getIdentity(), lengthField);
       defaultLengthInstruction.encode(destination, pmap, encoder, accessor);
    }

    for(size_t pos = 0; pos < length; ++pos)
    {
      encoder.encodeGroup(destination, segment_, *(*sequence)[pos]);
    }
  }
  else
  {
    // possibility #2: option group not present.
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory group.");
    }
    // let our counterparty know it's just not there.
    pmap.setNextField(false);
  }
}

void
FieldInstructionSequence::interpretValue(const std::string & value)
{
  throw TemplateDefinitionError("Value not needed by Sequence instruction.");
}

void
FieldInstructionSequence::indexDictionaries(
  DictionaryIndexer & indexer,
  const std::string & dictionaryName,
  const std::string & typeName,
  const std::string & typeNamespace)
{
  segment_->indexDictionaries(indexer, dictionaryName, typeName, typeNamespace);
}

