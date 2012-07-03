// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionTemplateRef.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>
#include <Codecs/TemplateRegistry.h>
#include <Messages/ValueMessageBuilder.h>
#include <Messages/Group.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionStaticTemplateRef::FieldInstructionStaticTemplateRef(
  const std::string & name, const std::string & fieldNamespace)
  : FieldInstruction(name, fieldNamespace)
  , templateName_(name)
  , templateNamespace_(fieldNamespace)
  , isFinalized_(false)
  , fieldCount_(0)
{
}

FieldInstructionStaticTemplateRef::FieldInstructionStaticTemplateRef()
  : isFinalized_(false)
  , fieldCount_(0)
{
}

FieldInstructionStaticTemplateRef::~FieldInstructionStaticTemplateRef()
{
}


bool
FieldInstructionStaticTemplateRef::isPossiblyRecursive() const
{
  return true;
}

void
FieldInstructionStaticTemplateRef::finalize(TemplateRegistry & templateRegistry)
{
  TemplatePtr target;
  if(!templateRegistry.findNamedTemplate(templateName_, templateNamespace_, target))
  {
    std::stringstream exception;
    exception << "[ERR D9] Unknown template name for static templateref." << identity_.name();
    throw QuickFAST::TemplateDefinitionError(exception.str());
  }
  target->finalize(templateRegistry);
  // subtract one for the template ID
  presenceMapBitsUsed_ = target->presenceMapBitCount() - 1;
  fieldCount_ = target->fieldCount();
  isFinalized_ = true;
}

void
FieldInstructionStaticTemplateRef::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & messageBuilder) const
{
  TemplateCPtr target;
  if(!decoder.findTemplate(templateName_, templateNamespace_, target))
  {
    decoder.reportFatal("[ERR D9]", "Unknown template name for static templateref.", identity_);
  }

  if(messageBuilder.getApplicationType() != target->getApplicationType())
  {
    Messages::ValueMessageBuilder & groupBuilder(
      messageBuilder.startGroup(
        identity_,
        target->getApplicationType(),
        target->getApplicationTypeNamespace(),
        target->fieldCount()));

    decoder.decodeSegmentBody(source, pmap, target, groupBuilder);
    messageBuilder.endGroup(
      identity_,
      groupBuilder);

  }
  else
  {
    // Because the application types match,
    // the templateRef fields are decoded directly into to the current
    // field set.  As a result the templateRef "disappears" completely
    // from the application message.  This is a good thing.
    // The same message encoded with different
    // templates could be transmitted with different sets of fields defined
    // by templateRefs, but the underlying application type should not reflect
    // the technique used to encode/decode it.
    decoder.decodeSegmentBody(source, pmap, target, messageBuilder);
  }
}

void
FieldInstructionStaticTemplateRef::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  // static templateRef
  // static
  TemplateCPtr target;
  if(!encoder.findTemplate(templateName_, templateNamespace_, target))
  {
    encoder.reportFatal("[ERR D9]", "Unknown template name for static templateref.", identity_);
  }

  // retrieve the field corresponding to this templateRef
  // which if it exists should be a FieldGroup
  const QuickFAST::Messages::MessageAccessor * group;
  if(accessor.getGroup(identity_, group))
  {
    encoder.encodeSegmentBody(
      destination,
      pmap,
      target,
      *group);
  }
  else
  {
    encoder.encodeSegmentBody(
      destination,
      pmap,
      target,
      accessor);
  }
}

void
FieldInstructionStaticTemplateRef::interpretValue(const std::string & value)
{
  throw TemplateDefinitionError("Unexpected field operation for templateref");
}

size_t
FieldInstructionStaticTemplateRef::fieldCount(const SegmentBody & parent)const
{
  if(!isFinalized_)
  {
    throw TemplateDefinitionError("Field count requested from static template reference before it has been finalized.");
  }
  return fieldCount_;
}

ValueType::Type
FieldInstructionStaticTemplateRef::fieldInstructionType()const
{
  return ValueType::TEMPLATEREF;
}


/////////////////////////////////////
// FieldInstructionDynamicTemplateRef
FieldInstructionDynamicTemplateRef::FieldInstructionDynamicTemplateRef()
  : FieldInstruction("dynamic template reference", "")
{
}

FieldInstructionDynamicTemplateRef::~FieldInstructionDynamicTemplateRef()
{
}

void
FieldInstructionDynamicTemplateRef::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & messageBuilder) const
{
   decoder.decodeNestedTemplate(source, messageBuilder, this->getIdentity());
}

void
FieldInstructionDynamicTemplateRef::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  encoder.reportFatal("[ERR I1]", "Encoding dynamic templates is not supported.");
}

void
FieldInstructionDynamicTemplateRef::interpretValue(const std::string & value)
{
  throw TemplateDefinitionError("Unexpected field operation for templateref");
}

size_t
FieldInstructionDynamicTemplateRef::fieldCount(const SegmentBody & parent)const
{
  // TODO: Someday we should actually check the target template, but that's
  // not doable right now.
  return 1;
}

ValueType::Type
FieldInstructionDynamicTemplateRef::fieldInstructionType()const
{
  return ValueType::TEMPLATEREF;
}
