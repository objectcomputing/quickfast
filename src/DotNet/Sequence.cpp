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
      :sequencePtr_(QuickFAST::Messages::SequencePtr(new TSequence))
    {
    }

    Sequence::Sequence(const TSequence& sequence)
      :sequencePtr_(QuickFAST::Messages::SequencePtr(new TSequence(sequence)))
    {
    }

    Sequence::Sequence(const QuickFAST::Messages::SequencePtr& message)
      :sequencePtr_(message)
    {
    }

    FieldSet^ Sequence::default::get(unsigned int index)
    {
        const QuickFAST::Messages::Sequence& sequence = sequencePtr_.GetRef();
        if(sequence.size() <= index || index < 0)
        {
          throw gcnew ArgumentOutOfRangeException();
        }
        return gcnew FieldSet(*sequence[index]);
    }

    String^ Sequence::ApplicationType::get()
    {
      return StlDotNet::string_cast(sequencePtr_->getApplicationType());
    }

    void Sequence::ApplicationType::set(String^ applicationType)
    {
      sequencePtr_->setApplicationType(StlDotNet::string_cast<std::string>(applicationType));
    }

    int Sequence::Count::get()
    {
      return sequencePtr_->size();
    }

    void Sequence::Add(FieldSet^ newFieldSet)
    {
      QuickFAST::Messages::FieldSetCPtr tmpPtr;
      sequencePtr_->addEntry(newFieldSet->FieldSetPtr.GetBoostPtr());
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
      return gcnew FieldSet(*(*itHolder_->it).get());
    }

    Object^ Sequence::SequenceEnumerator::Current::get()
    {
      return gcnew FieldSet(*(*itHolder_->it).get());
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
