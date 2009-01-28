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
      throw TemplateDefinitionError("[ERR D9] Unknown template name for static templateref.");
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
          group->setApplicationType(target->getApplicationType());
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
    throw TemplateDefinitionError("Encoding dynamic templates is not supported.");

  }
  else
  {
    // static templateRef
    // static
    TemplateCPtr target;
    if(!encoder.findTemplate(templateName_, templateNamespace_, target))
    {
      throw TemplateDefinitionError("[ERR D9] Unknown template name for static templateref.");
    }

    // retrieve the field corresponding to this templateRef
    // which if it exists should be a FieldGroup
    Messages::FieldCPtr field;
    if(fieldSet.getField(identity_.name(), field))
    {
      Messages::GroupCPtr group = field->toGroup();
      encoder.encodeGroup(
        destination,
        target,
        *group);
    }
    else
    {
#if 0
        // corresponding group field does not appear in application message.
        // There are two possibilites:
        //   1) this templateRef (mandatory or optional) has the same application type
        //      as the enclosing segment, and has therefore been merged into that segment.
        //   2) this is an optional templateRef that isn't present.
      if(fieldSet.getApplicationType() == getApplicationType())
      {
#endif
        // possiblity #1: encode this template using the original fieldSet
        encoder.encodeGroup(
          destination,
          target,
          fieldSet);
#if 0
      }
      else
      {
        // possibility #2: option template not present.
        if(isMandatory())
        {
          throw EncodingError("Missing mandatory templateRef/group.");
        }
        // let our counterparty know it's just not there.
        pmap.setNextField(false);
      }
#endif
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

