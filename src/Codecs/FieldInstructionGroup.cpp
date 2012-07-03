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
//      std::cout << "Decoding group into new segment: " << segmentBody_->getApplicationType() << std::endl;
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
//      std::cout << "Folding group into parent segment: " << segmentBody_->getApplicationType() << std::endl;
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
  // Note that applications may support merging groups
  // by returning true from getGroup but using the same accessor.
  const Messages::MessageAccessor * group;
  if(messageAccessor.getGroup(identity_, group))
  {
    if(! isMandatory())
    {
      pmap.setNextField(true);
    }
    encoder.encodeGroup(destination, segmentBody_, *group);
    messageAccessor.endGroup(identity_, group);
  }
  else
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory group.");
      encoder.encodeGroup(destination, segmentBody_, messageAccessor);
    }
    else
    {
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
