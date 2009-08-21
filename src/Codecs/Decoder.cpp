// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#include "Decoder.h"
#include <Codecs/DataSource.h>
#include <Codecs/PresenceMap.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/FieldInstruction.h>
#include <Messages/MessageBuilder.h>
#include <Common/Profiler.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

Decoder::Decoder(Codecs::TemplateRegistryPtr registry)
: Context(registry)
{
}

void
Decoder::decodeMessage(
   DataSource & source,
   Messages::MessageBuilder & messageBuilder)
{
  PROFILE_POINT("decode");
  source.beginMessage();

  Codecs::PresenceMap pmap(getTemplateRegistry()->presenceMapBits());
  if(this->verboseOut_)
  {
    pmap.setVerbose(verboseOut_);
  }

  static const std::string pmp("PMAP");
  source.beginField(pmp);
  if(!pmap.decode(source))
  {
    reportError("[ERR U03]", "Unexpected end of data in presence map.");
    return;
  }

  static const std::string tid("templateID");
  source.beginField(tid);
  if(pmap.checkNextField())
  {
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
    if(templatePtr->getReset())
    {
      reset(false);
    }
    Messages::MessageBuilder & messageBuilder(
      messageBuilder.startMessage(
        templatePtr->getApplicationType(),
        templatePtr->getApplicationTypeNamespace(),
        templatePtr->fieldCount()));

    decodeSegmentBody(source, pmap, templatePtr, messageBuilder);
    if(templatePtr->getIgnore())
    {
      messageBuilder.ignoreMessage(messageBuilder);
    }
    else
    {
      messageBuilder.endMessage(messageBuilder);
    }
  }
  else
  {
    std::string error =  "Unknown template ID:";
    error += boost::lexical_cast<std::string>(getTemplateId());
    reportError("[ERR D9]", error);
  }
  return;
}

void
Decoder::decodeNestedTemplate(
   DataSource & source,
   Messages::MessageBuilder & messageBuilder,
   Messages::FieldIdentityCPtr identity)
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
    reportError("[ERR U03]", "Unexpected end of data in presence map.");
    return;
  }

  static const std::string tid("templateID");
  source.beginField(tid);
  if(pmap.checkNextField())
  {
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
    if(templatePtr->getReset())
    {
      reset(false);
    }
    Messages::MessageBuilder & messageBuilder(
      messageBuilder.startGroup(
        identity,
        templatePtr->getApplicationType(),
        templatePtr->getApplicationTypeNamespace(),
        templatePtr->fieldCount()));

    decodeSegmentBody(source, pmap, templatePtr, messageBuilder);
    messageBuilder.endGroup(identity, messageBuilder);
  }
  else
  {
    std::string error =  "Unknown template ID:";
    error += boost::lexical_cast<std::string>(getTemplateId());
    reportError("[ERR D9]", error);
  }
  return;
}

void
Decoder::decodeGroup(
  DataSource & source,
  Codecs::SegmentBodyCPtr group,
  Messages::MessageBuilder & messageBuilder)
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
  decodeSegmentBody(source, pmap, group, messageBuilder);
}

void
Decoder::decodeSegmentBody(
  DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::SegmentBodyCPtr segment, //todo: reference to avoid copy?
  Messages::MessageBuilder & messageBuilder)
{
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
      if(!instruction->decode(source, pmap, *this, messageBuilder))
      {
        reportError("[ERR U03]", "Unexpected end of file in message body.");
      }
    }
  }
}
