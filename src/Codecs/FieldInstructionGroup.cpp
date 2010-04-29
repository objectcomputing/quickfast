// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionGroup.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>
#include <Messages/Group.h>
#include <Messages/ValueMessageBuilder.h>

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

void
FieldInstructionGroup::finalize(TemplateRegistry & templateRegistry)
{
  segmentBody_->finalize(templateRegistry);
  FieldInstruction::finalize(templateRegistry);
  // even though the field op is a NOP, an optional group uses a pmap bit
  if(!isMandatory())
  {
    presenceMapBitsUsed_ = 1;
  }
}

void
FieldInstructionGroup::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & messageBuilder) const
{
  bool present = true;

  if(! isMandatory())
  {
    present = pmap.checkNextField();
  }

  if(present)
  {
    if(!segmentBody_)
    {
      decoder.reportFatal("[ERR U08}", "Segment not defined for Group instruction.");
    }
    if(messageBuilder.getApplicationType() != segmentBody_->getApplicationType())
    {
      Messages::ValueMessageBuilder & groupBuilder(
        messageBuilder.startGroup(
          identity_,
          segmentBody_->getApplicationType(),
          segmentBody_->getApplicationTypeNamespace(),
          segmentBody_->fieldCount()));

      decoder.decodeGroup(source, segmentBody_, groupBuilder);
      messageBuilder.endGroup(
        identity_,
        groupBuilder);
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
      decoder.decodeGroup(source, segmentBody_, messageBuilder);
    }
  }
}

void
FieldInstructionGroup::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & messageAccessor) const
{
  if(!segmentBody_)
  {
    encoder.reportFatal("[ERR U08}", "Segment not defined for Group instruction.");
  }
  // retrieve the field corresponding to this group
  const QuickFAST::Messages::MessageAccessor * group;
  if(messageAccessor.getGroup(*identity_, group))
  {
    if(! isMandatory())
    {
      pmap.setNextField(true);
    }
    encoder.encodeGroup(destination, segmentBody_, *group);
  }
  else
  {
    // field does not appear in application message.
    // There are two possibilites:
    //   1) this group (mandatory or optional) has the same application type
    //      as the enclosing segment, and has therefore been merged into that segment.
    //   2) this is an optional group that isn't present.
    if(messageAccessor.getApplicationType() == getApplicationType())
    {
      // possiblity #1: encode this group using the original messageAccessor
      if(!isMandatory())
      {
        pmap.setNextField(true);
      }
      encoder.encodeGroup(destination, segmentBody_, messageAccessor);
    }
    else
    {
      // possibility #2: option group not present.
      if(isMandatory())
      {
        encoder.reportFatal("[ERR U01]", "Missing mandatory group.");
      }
      // let our counterparty know it's just not there.
      pmap.setNextField(false);
    }
  }
}

void
FieldInstructionGroup::interpretValue(const std::string & /*value*/)
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
  if(segmentBody_)
  {
    segmentBody_->indexDictionaries(indexer, dictionaryName,typeName, typeNamespace);
  }
}

ValueType::Type
FieldInstructionGroup::fieldInstructionType()const
{
  return ValueType::GROUP;
}

void
FieldInstructionGroup::displayBody(std::ostream & output, size_t indent)const
{
  segmentBody_->display(output, indent);
}
