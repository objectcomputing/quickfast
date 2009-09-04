// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include <DotNet/Sequence.h>
#include <DotNet/FieldSet.h>
#include <DotNet/StlDotNet.h>

using namespace System;

namespace QuickFASTDotNet{
  namespace Messages{

    Sequence::Sequence(const QuickFAST::Messages::SequenceCPtr& message)
      :sequencePtr_(message)
    {
      // Because of the way the derived class constructor for MutableSequence works,
      // this class may *not* depend on sequencePtr_ being non-NULL here.
    }

    FieldSet^ Sequence::default::get(int index)
    {
      QuickFAST::Messages::SequenceCPtr sequence = sequencePtr_.GetBoostPtr();
        if(sequence->size() <= static_cast<size_t>(index))
        {
          throw gcnew ArgumentOutOfRangeException();
        }
        return gcnew FieldSet((*sequence)[index]);
    }

    String^ Sequence::ApplicationType::get()
    {
      return StlDotNet::string_cast(sequencePtr_->getApplicationType());
    }

    int Sequence::Count::get()
    {
      return int(sequencePtr_->size());
    }

    System::Collections::IEnumerator^ Sequence::GetEnumerator()
    {
      return gcnew SequenceEnumerator(sequencePtr_->begin(), sequencePtr_->end(), this);
    }

    System::Collections::Generic::IEnumerator<FieldSet^>^ Sequence::GetSpecializedEnumerator()
    {
      return gcnew SequenceEnumerator(sequencePtr_->begin(), sequencePtr_->end(), this);
    }

    FieldSet^ Sequence::SequenceEnumerator::GenericCurrent::get()
    {
      return gcnew FieldSet(*itHolder_->it);
    }

    Object^ Sequence::SequenceEnumerator::Current::get()
    {
      return gcnew FieldSet(*itHolder_->it);
    }

    bool Sequence::SequenceEnumerator::MoveNext()
    {
      return (++itHolder_->it == itHolder_->end)? false: true;
    }

    void Sequence::SequenceEnumerator::Reset()
    {
      throw gcnew System::NotSupportedException();
    }
  }
}
