// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#include "Decoder.h"
#include <Codecs/DataSource.h>
#include <Codecs/PresenceMap.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/FieldInstruction.h>
#include <Messages/ValueMessageBuilder.h>
#include <Common/Profiler.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

Decoder::Decoder(Codecs::TemplateRegistryPtr registry)
: Context(registry)
{
}

//Decoder::Decoder()
//{
//}


void
Decoder::decodeMessage(
   DataSource & source,
   Messages::ValueMessageBuilder & messageBuilder)
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
  pmap.decode(source);

  static const std::string tid("templateID");
  source.beginField(tid);
  if(pmap.checkNextField())
  {
    template_id_t id;
    FieldInstruction::decodeUnsignedInteger(source, *this, id, tid);
    setTemplateId(id);
  }
  if(verboseOut_)
  {
    (*verboseOut_) << "Template ID: " << getTemplateId() << std::endl;
  }
  Codecs::TemplateCPtr templatePtr;
  if(getTemplateRegistry()->getTemplate(templateId_, templatePtr))
  {
    if(templatePtr->getReset())
    {
      reset(false);
    }
    Messages::ValueMessageBuilder & bodyBuilder(
      messageBuilder.startMessage(
        templatePtr->getApplicationType(),
        templatePtr->getApplicationTypeNamespace(),
        templatePtr->fieldCount()));

    decodeSegmentBody(source, pmap, templatePtr, bodyBuilder);
    if(templatePtr->getIgnore())
    {
      messageBuilder.ignoreMessage(bodyBuilder);
    }
    else
    {
      messageBuilder.endMessage(bodyBuilder);
    }
  }
  else if(templateId_ == SCPResetTemplateId)
  {
    reset(false);
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
   Messages::ValueMessageBuilder & messageBuilder,
   const Messages::FieldIdentity & identity)
{
  Codecs::PresenceMap pmap(getTemplateRegistry()->presenceMapBits());
  if(this->verboseOut_)
  {
    pmap.setVerbose(verboseOut_);
  }

  static const std::string pmp("PMAP");
  source.beginField(pmp);
  pmap.decode(source);

  static const std::string tid("templateID");
  source.beginField(tid);
  if(pmap.checkNextField())
  {
    template_id_t id;
    FieldInstruction::decodeUnsignedInteger(source, *this, id, tid);
    setTemplateId(id);
  }
  if(verboseOut_)
  {
    (*verboseOut_) << "Nested Template ID: " << getTemplateId() << std::endl;
  }
  Codecs::TemplateCPtr templatePtr;
  if(getTemplateRegistry()->getTemplate(getTemplateId(), templatePtr))
  {
    if(templatePtr->getReset())
    {
      reset(false);
    }
    Messages::ValueMessageBuilder & groupBuilder(
      messageBuilder.startGroup(
        identity,
        templatePtr->getApplicationType(),
        templatePtr->getApplicationTypeNamespace(),
        templatePtr->fieldCount()));

    decodeSegmentBody(source, pmap, templatePtr, groupBuilder);
    messageBuilder.endGroup(identity, groupBuilder);
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
  const Codecs::SegmentBodyCPtr & group,
  Messages::ValueMessageBuilder & messageBuilder)
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
    pmap.decode(source);
  }
// for debugging:  pmap.setVerbose(source.getEcho());
  decodeSegmentBody(source, pmap, group, messageBuilder);
}

void
Decoder::decodeSegmentBody(
  DataSource & source,
  Codecs::PresenceMap & pmap,
  const Codecs::SegmentBodyCPtr & segment,
  Messages::ValueMessageBuilder & messageBuilder)
{
  size_t instructionCount = segment->size();
  for( size_t nField = 0; nField < instructionCount; ++nField)
  {
    PROFILE_POINT("decode field");
    const Codecs::FieldInstructionCPtr & instruction = segment->getInstruction(nField);
    if(verboseOut_)
    {
      (*verboseOut_) <<std::endl << "Decode instruction[" <<nField << "]: " << instruction->getIdentity().name() << std::endl;
    }
    source.beginField(instruction->getIdentity().name());
    (void)instruction->decode(source, pmap, *this, messageBuilder);
  }
}
