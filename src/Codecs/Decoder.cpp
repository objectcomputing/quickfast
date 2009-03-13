// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#include "Decoder.h"
#include <Codecs/DataSource.h>
#include <Codecs/PresenceMap.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/FieldInstruction.h>
#include <Messages/DecodedFields.h>
//#include <Messages/Message.h>
#include <Common/Profiler.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

Decoder::Decoder(Codecs::TemplateRegistryPtr registry)
: Context(registry)
, strict_(true)
{
}

bool
Decoder::decodeMessage(
   DataSource & source,
   Messages::DecodedFields & message)
{
  PROFILE_POINT("decode");
  source.beginMessage();
  return decodeSegment(source, message);
}

bool
Decoder::decodeSegment(
   DataSource & source,
   Messages::DecodedFields & fieldSet)
{
  Codecs::PresenceMap pmap(getTemplateRegistry()->presenceMapBits());
  if(this->verboseOut_)
  {
    pmap.setVerbose(verboseOut_);
  }

  static const std::string pmp("PMAP");
  source.beginField(pmp);
  if(!pmap.decode(source))
  {
    return false;
  }

  if(pmap.checkNextField())
  {
    static const std::string tid("templateID");
    source.beginField(tid);
    template_id_t id;
    FieldInstruction::decodeUnsignedInteger(source, *this, id);
    setTemplateId(id);
  }
  if(verboseOut_)
  {
    (*verboseOut_) << "Template ID: " << getTemplateId() << std::endl;
  }
  Codecs::TemplateCPtr templatePtr;
  if(getTemplateRegistry()->getTemplate(getTemplateId(), templatePtr))
  {
    if(!decodeSegmentBody(source, pmap, templatePtr, fieldSet))
    {
      throw EncodingError("Unexpected end of file in message body.");
    }
  }
  else
  {
    std::string message =  "[ERR D9] Unknown template ID:";
    message += boost::lexical_cast<std::string>(getTemplateId());
    throw EncodingError(message);
  }
  return true;
}

void
Decoder::decodeGroup(
  DataSource & source,
  Codecs::SegmentBodyCPtr group,
  Messages::DecodedFields & fieldSet)
{
  size_t presenceMapBits = group->presenceMapBitCount();
  Codecs::PresenceMap pmap(presenceMapBits);
  if(this->verboseOut_)
  {
    pmap.setVerbose(verboseOut_);
  }

  if(presenceMapBits > 0)
  {
    static const std::string pm("PMAP");
    source.beginField(pm);
    if(!pmap.decode(source))
    {
      throw EncodingError("Unexpected end of file expecting Group presence map.");
    }
  }
// for debugging:  pmap.setVerbose(source.getEcho());
  if(!decodeSegmentBody(source, pmap, group, fieldSet))
  {
    throw EncodingError("Unexpected end of file in message body.");
  }
}

bool
Decoder::decodeSegmentBody(
  DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::SegmentBodyCPtr segment, //todo: reference to avoid copy?
  Messages::DecodedFields & fieldSet)
{
  fieldSet.setApplicationType(
    segment->getApplicationType(),
    segment->getApplicationTypeNamespace());
  size_t instructionCount = segment->size();
  for( size_t nField = 0; nField < instructionCount; ++nField)
  {
    PROFILE_POINT("decode field");
    Codecs::FieldInstructionCPtr instruction;
    if(segment->getInstruction(nField, instruction))
    {
      if(verboseOut_)
      {
        (*verboseOut_) << "Decode instruction[" <<nField << "]: " << instruction->getIdentity()->name() << std::endl;
      }
      source.beginField(instruction->getIdentity()->name());
      if(!instruction->decode(source, pmap, *this, fieldSet))
      {
        return false;
      }
    }
  }
  return true;
}
