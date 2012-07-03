// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#include "QuickFASTDotNetPch.h"
#include "ImplSequenceBuilder.h"
#include <DotNet/ImplFieldSetBuilder.h>
#include <DotNet/ImplSequence.h>

#pragma unmanaged

using namespace QuickFAST;
using namespace DotNet;

ImplSequenceBuilder::ImplSequenceBuilder(ImplMessageDeliverer & deliverer)
  : ImplBuilderBase(deliverer)
  , sequence_(0)//new ImplSequence)
{
}

ImplSequenceBuilder::~ImplSequenceBuilder()
{
  // In theory endSequence will have been called and sequence_ will be zero
  // however...
  delete sequence_;
}
#pragma unmanaged
void
ImplSequenceBuilder::initialize(
  const Messages::FieldIdentity & lengthIdentity,
  size_t length
  )
{
  delete sequence_;
  sequence_ = new ImplSequence(lengthIdentity, length);
}

ImplSequence *
ImplSequenceBuilder::endSequence()
{
  ImplSequence * result = sequence_;
  sequence_ = 0;
  return result;
}

Messages::ValueMessageBuilder &
ImplSequenceBuilder::startSequenceEntry(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  if(!sequenceEntryBuilder_)
  {
    sequenceEntryBuilder_.reset(new ImplFieldSetBuilder(deliverer_));
  }
  sequenceEntryBuilder_->reset(size);
  return *sequenceEntryBuilder_;
}

void
ImplSequenceBuilder::endSequenceEntry(Messages::ValueMessageBuilder & entry)
{
  sequence_->append(sequenceEntryBuilder_->endSet());
}

#pragma managed
