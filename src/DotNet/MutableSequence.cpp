// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include <DotNet/MutableSequence.h>
#include <DotNet/FieldSet.h>
#include <DotNet/StlDotNet.h>

namespace QuickFASTDotNet{
  namespace Messages{

    MutableSequence::MutableSequence()
      : Sequence(QuickFAST::Messages::SequenceCPtr())
      , mutableSequencePtr_(QuickFAST::Messages::SequencePtr(new TSequence))
    {
      sequencePtr_.GetBoostPtr() = mutableSequencePtr_.GetBoostPtr();
    }

    void MutableSequence::ApplicationType::set(String^ applicationType)
    {
      mutableSequencePtr_->setApplicationType(StlDotNet::string_cast<std::string>(applicationType));
    }

    void MutableSequence::Add(FieldSet^ newFieldSet)
    {
      mutableSequencePtr_->addEntry(newFieldSet->FieldSetCPtr.GetBoostPtr());
    }
  }
}
