// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldSequence.h"
#include <Messages/Sequence.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldSequence::FieldSequence(Messages::SequenceCPtr sequence)
  : Field(true)
  , sequence_(sequence)
{
}

FieldSequence::~FieldSequence()
{
}

Field::FieldType
FieldSequence::getType()const
{
  return Field::SEQUENCE;
}

uint32
FieldSequence::toUInt32()const
{
  return sequence_->size();
}

#if 0
void
FieldSequence::setSequence(Messages::SequenceCPtr sequence)
{
  sequence_ = sequence;
  valid_ = true;
}
#endif

const Messages::SequenceCPtr &
FieldSequence::toSequence() const
{
  return sequence_;
}

FieldCPtr
FieldSequence::create(Messages::SequenceCPtr sequence)
{
  return new FieldSequence(sequence);
}

