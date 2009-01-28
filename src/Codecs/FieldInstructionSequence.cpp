// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionSequence.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>
#include <Messages/Message.h>
#include <Messages/FieldSequence.h>
#include <Messages/Sequence.h>
#include <Messages/FieldUInt32.h>

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
  Messages::FieldSet & fieldSet) const
{
  if(!isMandatory())
  {
    if(!pmap.checkNextField())
    {
      return true;
    }
  }

  if(!segment_)
  {
    throw TemplateDefinitionError("SegmentBody not defined for Group instruction.");
  }
  size_t length = 0;
  Codecs::FieldInstructionCPtr lengthInstruction;
  if(!segment_->getLengthInstruction(lengthInstruction))
  {
    decoder.reportFatal("", "Length instruction not available for sequence.");
  }

  // todo: performance could be improved by bypassing the field set
  source.beginField(lengthInstruction->getIdentity().name());
  Messages::FieldSet lengthSet(1);
  if(!lengthInstruction->decode(source, pmap, decoder, lengthSet))
  {
    return false;
  }

  Messages::FieldSet::const_iterator fld = lengthSet.begin();
  if(fld == lengthSet.end())
  {
    /// @todo: report this?
    return false;
  }
  length = fld->getField()->toUInt32();

  Messages::SequencePtr sequence(new Messages::Sequence);
  for(size_t nEntry = 0; nEntry < length; ++nEntry)
  {
    if(decoder.getLogOut())
    {
      std::stringstream msg;
      msg << "Sequence entry #" << nEntry << " of " << length << std::ends;
      decoder.logMessage(msg.str());
    }

    Messages::FieldSetPtr entrySet(new Messages::FieldSet(segment_->fieldCount()));
    entrySet->setApplicationType(segment_->getApplicationType());
    decoder.decodeGroup(source, segment_, *entrySet);
    sequence->addEntry(entrySet);
  }
  Messages::FieldCPtr field(Messages::FieldSequence::create(sequence));
  fieldSet.addField(
    identity_,
    field);
  return true;
}

void
FieldInstructionSequence::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::FieldSet & fieldSet) const
{
  // retrieve the field corresponding to this sequence
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_.name(), field))
  {
    Messages::SequenceCPtr sequence = field->toSequence();
    size_t length = sequence->size();

    Codecs::FieldInstructionCPtr lengthInstruction;
    if(!segment_->getLengthInstruction(lengthInstruction))
    {
      throw EncodingError("Length instruction not available for sequence.");
    }
    // todo: performance could be improved here
    Messages::FieldCPtr lengthField(Messages::FieldUInt32::create(length));
    Messages::FieldSet lengthSet(1);

    lengthSet.addField(lengthInstruction->getIdentity(), lengthField);
    lengthInstruction->encode(destination, pmap, encoder, lengthSet);

    for(size_t pos = 0; pos < length; ++pos)
    {
      Messages::FieldSetCPtr entry = (*sequence)[pos];
      encoder.encodeGroup(destination, segment_, fieldSet);
    }
  }
  else
  {
    // possibility #2: option group not present.
    if(isMandatory())
    {
      throw EncodingError("Missing mandatory group.");
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

