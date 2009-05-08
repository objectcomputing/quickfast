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

    Sequence::Sequence()
      :sequencePtr_(QuickFAST::Messages::SequenceCPtr(new TSequence))
    {
    }

    Sequence::Sequence(const QuickFAST::Messages::SequenceCPtr& message)
      :sequencePtr_(message)
    {
    }

    FieldSet^ Sequence::default::get(unsigned int index)
    {
        QuickFAST::Messages::SequenceCPtr sequence = sequencePtr_.GetBoostPtr();
        if(sequence->size() <= index)
        {
          throw gcnew ArgumentOutOfRangeException();
        }
        return gcnew FieldSet((*sequence)[index]);
    }

    String^ Sequence::ApplicationType::get()
    {
      return StlDotNet::string_cast(sequencePtr_->getApplicationType());
    }

    /* Not yet implemented -- only for the encoder.  Will need a mutable FieldSetPtr.
    void Sequence::ApplicationType::set(String^ applicationType)
    {
      sequencePtr_->setApplicationType(StlDotNet::string_cast<std::string>(applicationType));
    }
    */

    int Sequence::Count::get()
    {
      return sequencePtr_->size();
    }

    /* Not yet implemented -- only for the encoder.  Will need a mutable FieldSetPtr.
    void Sequence::Add(FieldSet^ newFieldSet)
    {
      sequencePtr_->addEntry(newFieldSet->FieldSetCPtr.GetBoostPtr());
    }
    */

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
