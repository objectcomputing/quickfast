// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldSequence.h"
#include <Messages/Sequence.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldSequence::FieldSequence(Messages::SequenceCPtr sequence)
  : Field(Field::SEQUENCE, true)
  , sequence_(sequence)
{
}

FieldSequence::~FieldSequence()
{
}

uint32
FieldSequence::toUInt32()const
{
  return static_cast<uint32>(sequence_->size());
}



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

