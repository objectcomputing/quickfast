// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#include "Encoder.h"
#include <Codecs/DataDestination.h>
#include <Messages/MessageAccessor.h>
#include <Codecs/PresenceMap.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/FieldInstruction.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

Encoder::Encoder(Codecs::TemplateRegistryPtr registry)
: Context(registry)
{
}

void
Encoder::encodeMessages(
  DataDestination & destination,
  Messages::MessageAccessor & accessor)
{
  template_id_t templateId;
  while(accessor.pickTemplate(templateId))
  {
    encodeMessage(destination, templateId, accessor);
  }
}

void
Encoder::encodeMessage(
  DataDestination & destination,
  template_id_t templateId,
  const Messages::MessageAccessor & accessor)
{
  destination.startMessage(templateId);
  encodeSegment(destination, templateId, accessor);
  destination.endMessage();
}

void
Encoder::encodeSegment(
  DataDestination & destination,
  template_id_t templateId,
  const Messages::MessageAccessor & accessor)
{
  Codecs::TemplateCPtr templatePtr;
  if(getTemplateRegistry()->getTemplate(templateId, templatePtr))
  {
    if(templatePtr->getReset())
    {
      reset(true);
    }

    Codecs::PresenceMap pmap(templatePtr->presenceMapBitCount());

    DataDestination::BufferHandle header = destination.startBuffer();
    destination.startBuffer();
    // can we "copy" the template ID?
    if(templateId == templateId_)
    {
      pmap.setNextField(false);
    }
    else
    {
      pmap.setNextField(true);
      FieldInstruction::encodeUnsignedInteger(destination, getWorkingBuffer(), templateId);
      templateId_ = templateId;
    }

    encodeSegmentBody(destination, pmap, templatePtr, accessor);
    DataDestination::BufferHandle savedBuffer = destination.getBuffer();
    destination.selectBuffer(header);
    static Messages::FieldIdentity pmapIdentity("PMAP", "Message");
    destination.startField(pmapIdentity);
    pmap.encode(destination);
    destination.endField(pmapIdentity);
    destination.selectBuffer(savedBuffer);
  }
  else
  {
    throw EncodingError("[ERR D9] Unknown template ID.");
  }
}

void
Encoder::encodeGroup(
  DataDestination & destination,
  const Codecs::SegmentBodyPtr & group,
  const Messages::MessageAccessor & accessor)
{
  size_t presenceMapBits = group->presenceMapBitCount();
  Codecs::PresenceMap pmap(presenceMapBits);
  pmap.setVerbose(this->verboseOut_);

  // The presence map for the group will go into the current buffer
  // that will be the last thing to appear in that buffer
  DataDestination::BufferHandle pmapBuffer = destination.getBuffer();
  DataDestination::BufferHandle bodyBuffer = pmapBuffer;
  if(presenceMapBits > 0)
  {
    bodyBuffer = destination.startBuffer();
  }
  encodeSegmentBody(destination, pmap, group, accessor);
  // remember the buffer we're presently working on
  bodyBuffer = destination.getBuffer();
  if(presenceMapBits > 0)
  {
    destination.selectBuffer(pmapBuffer);
    static Messages::FieldIdentity pmapIdentity("PMAP", "Group");
    destination.startField(pmapIdentity);
    pmap.encode(destination);
    destination.endField(pmapIdentity);
  }
  // and just continue working in the buffer where we built the group body
  destination.selectBuffer(bodyBuffer);
}


void
Encoder::encodeSegmentBody(
  DataDestination & destination,
  Codecs::PresenceMap & pmap,
  const Codecs::SegmentBodyCPtr & segment,
  const Messages::MessageAccessor & accessor)
{
  size_t instructionCount = segment->size();
  for( size_t nField = 0; nField < instructionCount; ++nField)
  {
    PROFILE_POINT("encode field");
    Codecs::FieldInstructionCPtr instruction;
    if(segment->getInstruction(nField, instruction))
    {
      destination.startField(instruction->getIdentity());
      instruction->encode(destination, pmap, *this, accessor);
      destination.endField(instruction->getIdentity());
    }
  }
}
