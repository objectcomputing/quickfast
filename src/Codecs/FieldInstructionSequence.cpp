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

using namespace QuickFAST;
using namespace QuickFAST::Codecs;


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
void
FieldInstructionSequence::finalize(TemplateRegistry & templateRegistry)
{
  segment_->finalize(templateRegistry);
  // Some non-standard implementatons of FAST put the presence="optional" attribute on the
  // length element rather than the sequence element.
  // This code promotes the "optional" flag from the length instruction
  // to the sequence instruction where it belongs.
  // In standards compliant templates the length instruction will always be mandatory
  // so this code will have no effect.
  FieldInstructionCPtr lengthPtr;
  if(segment_->getLengthInstruction(lengthPtr))
  {
    if(!lengthPtr->isMandatory())
    {
      setPresence(false);
    }
  }
  FieldInstruction::finalize(templateRegistry);
}

void
FieldInstructionSequence::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & builder) const
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
    source.beginField(lengthInstruction->getIdentity().name());
    lengthInstruction->decode(source, pmap, decoder, lengthSet);
  }
  else
  {
    FieldInstructionUInt32 defaultLengthInstruction;
    defaultLengthInstruction.setPresence(isMandatory());
    defaultLengthInstruction.decode(source, pmap, decoder, lengthSet);
  }
  if(lengthSet.isSet())
  {
    length = lengthSet.value();

    Messages::ValueMessageBuilder & sequenceBuilder = builder.startSequence(
      identity_,
      segment_->getApplicationType(),
      segment_->getApplicationTypeNamespace(),
      segment_->fieldCount(),
      lengthSet.identity(),
      length);

    for(size_t nEntry = 0; nEntry < length; ++nEntry)
    {
      if(decoder.getLogOut())
      {
        std::stringstream msg;
        msg << "Sequence entry #" << nEntry << " of " << length << std::ends;
        decoder.logMessage(msg.str());
      }

      Messages::ValueMessageBuilder & entrySet(
        sequenceBuilder.startSequenceEntry(
          segment_->getApplicationType(),
          segment_->getApplicationTypeNamespace(),
          segment_->fieldCount()));
      decoder.decodeGroup(source, segment_, entrySet);
      sequenceBuilder.endSequenceEntry(entrySet);
    }
    builder.endSequence(identity_, sequenceBuilder);
  }
}

void
FieldInstructionSequence::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  if(!segment_)
  {
    encoder.reportFatal("[ERR U07]", "SegmentBody not defined for Sequence instruction.");
  }

  size_t length = 0;
  if(accessor.getSequenceLength(identity_, length))
  {
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
      const Messages::MessageAccessor * entry;
      if(accessor.getSequenceEntry(identity_, pos, entry))
      {
        encoder.encodeGroup(destination, segment_, *entry);
      }
      else
      {
        std::stringstream msg;
        msg << "Sequence entry " << pos << " of " << length << " missing.";
        encoder.reportError("Uxx", msg.str(), identity_);
      }
      accessor.endSequenceEntry(identity_, pos, entry);
    }
    accessor.endSequence(identity_);
  }
  else
  {
    // The sequence is not present in the application record
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory sequence.");
      // if reportFail returns, we're being lax about encoding rules
      // Send a sequence with no entries.
      Codecs::FieldInstructionCPtr lengthInstruction;
      if(segment_->getLengthInstruction(lengthInstruction))
      {
         Messages::SingleFieldAccessor accessor(lengthInstruction->getIdentity(), 0);
        lengthInstruction->encode(destination, pmap, encoder, accessor);
      }
      else
      {
        destination.putByte(zeroIntegerNonnullable);
      }
    }
    else
    {
      Codecs::FieldInstructionCPtr lengthInstruction;
      if(segment_->getLengthInstruction(lengthInstruction))
      {
        // let the length instruction encode the fact that it's missing
        Messages::EmptyAccessor accessor;
        lengthInstruction->encode(destination, pmap, encoder, accessor);
      }
      else
      {
        // implicit length instruction: nullable uint32 with no field operator
        destination.putByte(nullInteger);
      }
    }
  }
}

void
FieldInstructionSequence::interpretValue(const std::string & /*value*/)
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
  segment_->indexDictionaries(indexer, dictionaryName,typeName, typeNamespace);
}

ValueType::Type
FieldInstructionSequence::fieldInstructionType()const
{
  return ValueType::SEQUENCE;
}

void
FieldInstructionSequence::displayBody(std::ostream & output, size_t indent)const
{
  std::string indentString(indent, ' ');
  segment_->display(output, indent);
}
