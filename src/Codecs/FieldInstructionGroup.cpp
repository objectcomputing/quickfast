// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionGroup.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>
#include <Messages/Group.h>
#include <Messages/FieldGroup.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionGroup::FieldInstructionGroup(
  const std::string & name,
  const std::string & fieldNamespace)
  : FieldInstruction(name, fieldNamespace)
{
}

FieldInstructionGroup::FieldInstructionGroup()
{
}

FieldInstructionGroup::~FieldInstructionGroup()
{
}

bool
FieldInstructionGroup::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::FieldSet & fieldSet) const
{
  bool present = true;

  if(segmentBody_->presenceMapBitCount() > 0)
  {
    present = pmap.checkNextField();
  }

  if(present)
  {
    if(!segmentBody_)
    {
      decoder.reportFatal("[ERR U08}", "Segment not defined for Group instruction.");
    }
    if(fieldSet.getApplicationType() != segmentBody_->getApplicationType())
    {
      Messages::GroupPtr group(new Messages::Group(segmentBody_->fieldCount()));
      group->setApplicationType(segmentBody_->getApplicationType(), segmentBody_->getApplicationTypeNamespace());
      decoder.decodeGroup(source, segmentBody_, *group);
      Messages::FieldCPtr field(Messages::FieldGroup::create(group));
      fieldSet.addField(
        identity_,
        field);
    }
    else
    {
      // Because the application types match,
      // the group fields are decoded directly into to the current
      // field set.  As a result the group "disappears" completely
      // from the application message.  This is a good thing.  Groups
      // are an artifact of the template used to encode the message
      // rather than being an attribute of the actual message being
      // encoded.  In fact, the same message encoded with different
      // templates could be transmitted with different sets of fields
      // in groups.
      decoder.decodeGroup(source, segmentBody_, fieldSet);
    }
  }
  return true;
}

void
FieldInstructionGroup::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::FieldSet & fieldSet) const
{
  // retrieve the field corresponding to this group
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_.name(), field))
  {
    Messages::GroupCPtr group = field->toGroup();
    encoder.encodeGroup(destination, segmentBody_, *group);
  }
  else
  {
    // field does not appear in application message.
    // There are two possibilites:
    //   1) this group (mandatory or optional) has the same application type
    //      as the enclosing segment, and has therefore been merged into that segment.
    //   2) this is an optional group that isn't present.
    if(fieldSet.getApplicationType() == getApplicationType())
    {
      // possiblity #1: encode this group using the original fieldSet
      encoder.encodeGroup(destination, segmentBody_, fieldSet);
    }
    else
    {
      // possibility #2: option group not present.
      if(isMandatory())
      {
        encoder.reportFatal("[ERR U09]", "Missing mandatory group.");
      }
      // let our counterparty know it's just not there.
      pmap.setNextField(false);
    }
  }
}

size_t
FieldInstructionGroup::presenceMapBitsRequired() const
{
  if(bool(segmentBody_) && segmentBody_->presenceMapBitCount() > 0)
  {
    return 1;
  }
  return 0;
}


void
FieldInstructionGroup::interpretValue(const std::string & value)
{
  throw TemplateDefinitionError("[ERR U11] Value not needed by Group instruction.");
}

size_t
FieldInstructionGroup::fieldCount(const SegmentBody & parent)const
{
  if(parent.getApplicationType() == segmentBody_->getApplicationType())
  {
    return segmentBody_->fieldCount();
  }
  else
  {
    return 1;
  }
}

void
FieldInstructionGroup::indexDictionaries(
  DictionaryIndexer & indexer,
  const std::string & dictionaryName,
  const std::string & typeName,
  const std::string & typeNamespace)
{
  segmentBody_->indexDictionaries(indexer, dictionaryName, typeName, typeNamespace);
}

