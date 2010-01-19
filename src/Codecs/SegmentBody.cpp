// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "SegmentBody.h"
#include <Codecs/FieldInstruction.h>
#include <Common/Exceptions.h>
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

SegmentBody::SegmentBody(size_t pmapBits)
: isFinalizing_(false)
, isFinalized_(false)
, presenceMapBits_(pmapBits)
, initialPresenceMapBits_(pmapBits)
, allowLengthField_(false)
, mandatoryLength_(true)
{
}

void
SegmentBody::setApplicationType(const std::string & type, const std::string & typeNs)
{
  if(applicationType_ != type)
  {
    // because <typeref> doesn't have to be the first contained element
    // be sure child elements inherit the type
    size_t count = instructions_.size();
    for( size_t pos = 0; pos < count; ++pos)
    {
      const std::string oldType = instructions_[pos]->getApplicationType();
      if(oldType.empty() || oldType == applicationType_)
      {
        mutableInstructions_[pos]->setApplicationType(type, typeNs);
      }
    }
    applicationType_ = type;
    applicationNamespace_ = typeNs;
  }
}

void
SegmentBody::setDictionaryName(const std::string & name)
{
  dictionaryName_ = name;
}

void
SegmentBody::finalize(TemplateRegistry & templateRegistry)
{
  if(isFinalizing_ || isFinalized_)
  {
    return;
  }
  isFinalizing_ = true;

  if(bool(lengthInstruction_))
  {
    lengthInstruction_->finalize(templateRegistry);
  }
  for (size_t pos = 0; pos < mutableInstructions_.size(); ++pos)
  {
    FieldInstructionPtr pf = mutableInstructions_[pos];
    pf->finalize(templateRegistry);
  }

  presenceMapBits_ = initialPresenceMapBits_;
  fieldCount_ = 0;
  if(bool(lengthInstruction_))
  {
    presenceMapBits_ += lengthInstruction_->getPresenceMapBitsUsed();
  }
  for (size_t pos = 0; pos < instructions_.size(); ++pos)
  {
    size_t used = instructions_[pos]->getPresenceMapBitsUsed();
    presenceMapBits_ += used;
    fieldCount_ += instructions_[pos]->fieldCount(*this);
  }
  isFinalizing_ = false;
  isFinalized_ = true;
}

void
SegmentBody::addLengthInstruction(FieldInstructionPtr & field)
{
  if(!allowLengthField_)
  {
    throw TemplateDefinitionError("<length> element not allowed in this context.");
  }
  if(instructions_.size() > 0)
  {
    throw TemplateDefinitionError("<length> element must be first in context.");
  }
  if(this->lengthInstruction_)
  {
    throw TemplateDefinitionError("Only one <length> element may appear in context.");
  }
  field->setPresence(mandatoryLength_);
  lengthInstruction_ = field;
}


void
SegmentBody::addInstruction(FieldInstructionPtr & field)
{
  if(field->getApplicationType().empty())
  {
    field->setApplicationType(applicationType_, applicationNamespace_);
  }
  mutableInstructions_.push_back(field);
  instructions_.push_back(field);
}

size_t
SegmentBody::instructionIndex(const std::string & name)const
{
  // note for a small number of fields (typical), a linear search is probably just as fast
  // or faster than a map, and since field order is critical we keep fields in a vector.
  for (size_t pos = 0; pos < instructions_.size(); ++pos)
  {
    if(instructions_[pos]->getName() == name)
    {
      return pos;
    }
  }
  return instructions_.size();
}

bool
SegmentBody::getInstruction(const std::string & name, FieldInstructionCPtr & value)const
{
  size_t index = instructionIndex(name);
  if(index >= instructions_.size())
  {
    return false;
  }
  return getInstruction(index, value);
}

bool
SegmentBody::getInstruction(size_t index, FieldInstructionCPtr & value)const
{
  if(index >= instructions_.size())
  {
    throw UsageError("SegmentBody::getInstruction: ", "index out of range.");
  }
  value = instructions_[index];
  return bool(value);
}

#if 0
const FieldInstructionCPtr &
SegmentBody::getInstruction(size_t index)const
{
  if(index >= instructions_.size())
  {
    throw UsageError("SegmentBody::getInstruction: ", "index out of range.");
  }
  return instructions_[index];
}
#endif

bool
SegmentBody::getLengthInstruction(FieldInstructionCPtr & value)const
{
  if(bool(lengthInstruction_))
  {
    value = lengthInstruction_;
  }
  return bool(lengthInstruction_);
}

void
SegmentBody::indexDictionaries(
  DictionaryIndexer & indexer,
  const std::string & dictionaryName,
  const std::string & typeName,
  const std::string & typeNamespace
  )
{
  std::string name = dictionaryName;
  if(!dictionaryName_.empty())
  {
    name = dictionaryName_;
  }
  std::string type = typeName;
  std::string typeNs = typeNamespace;
  if(!applicationType_.empty())
  {
    type = applicationType_;
    typeNs = applicationNamespace_;

  }
  if(bool(lengthInstruction_))
  {
    lengthInstruction_->indexDictionaries(indexer, name, type, typeNs);
  }
  for(MutableInstructionVector::iterator it = mutableInstructions_.begin();
    it != mutableInstructions_.end();
    ++it)
  {
    (*it)->indexDictionaries(indexer, name, type, typeNs);
  }
}
