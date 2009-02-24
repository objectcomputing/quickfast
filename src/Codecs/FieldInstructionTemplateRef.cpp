// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionTemplateRef.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>
#include <Messages/Message.h>
#include <Messages/Group.h>
#include <Messages/FieldGroup.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionTemplateRef::FieldInstructionTemplateRef(
  const std::string & name, const std::string & fieldNamespace)
  : FieldInstruction(name, fieldNamespace)
{
}

FieldInstructionTemplateRef::FieldInstructionTemplateRef()
{
}

FieldInstructionTemplateRef::~FieldInstructionTemplateRef()
{
}

bool
FieldInstructionTemplateRef::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::FieldSet & fieldSet) const
{
  if(templateName_.empty())
  {
    // dynamic
    decoder.decodeSegment(source, fieldSet);
  }
  else
  {
    // static
    TemplateCPtr target;
    if(!decoder.findTemplate(templateName_, templateNamespace_, target))
    {
      decoder.reportFatal("[ERR D9]", "Unknown template name for static templateref.");
    }

    bool present = true;
    if(!isMandatory())
    {
      if(target->presenceMapBitCount() > 1)
      {
        present = pmap.checkNextField();
      }

      if(present)
      {
        if(fieldSet.getApplicationType() != target->getApplicationType())
        {
          // application types do not match.  Decode this into a FieldGroup
          size_t fieldCount = target->fieldCount();
          Messages::GroupPtr group(new Messages::Group(fieldCount));
          group->setApplicationType(target->getApplicationType(), target->getApplicationTypeNamespace());
          decoder.decodeGroup(
            source,
            target,
            *group);
          fieldSet.addField(
            identity_,
            Messages::FieldGroup::create(group));
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
          decoder.decodeGroup(
            source,
            target,
            fieldSet);
        }
      }
    }
  }
  return true;
}

void
FieldInstructionTemplateRef::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::FieldSet & fieldSet) const
{
  if(templateName_.empty())
  {
    encoder.reportFatal("[ERR I1]", "Encoding dynamic templates is not supported.");
  }
  else
  {
    // static templateRef
    // static
    TemplateCPtr target;
    if(!encoder.findTemplate(templateName_, templateNamespace_, target))
    {
      encoder.reportFatal("[ERR D9]", "Unknown template name for static templateref.");
    }

    // retrieve the field corresponding to this templateRef
    // which if it exists should be a FieldGroup
    Messages::FieldCPtr field;
    if(fieldSet.getField(identity_->name(), field))
    {
      Messages::GroupCPtr group = field->toGroup();
      encoder.encodeGroup(
        destination,
        target,
        *group);
    }
    else
    {
      encoder.encodeGroup(
        destination,
        target,
        fieldSet);
    }
  }
}

void
FieldInstructionTemplateRef::interpretValue(const std::string & value)
{
  throw TemplateDefinitionError("Unexpected field operation for templateref");
}

size_t
FieldInstructionTemplateRef::fieldCount(const SegmentBody & parent)const
{
  // TODO: Someday we should actually check the target template, but that's
  // not doable right now.
  return 1;
}

