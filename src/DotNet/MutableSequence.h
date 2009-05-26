// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include <DotNet/UnmanagedPtr.h>
#include <DotNet/BoostPtr.h>
#include <DotNet/Sequence.h>

namespace QuickFASTDotNet{
  namespace Messages{

    /// Container for a sequence of field groups, allows mutating operations
    [System::Diagnostics::DebuggerTypeProxy(Sequence::DbView::typeid)]
    public ref class MutableSequence: public Sequence
    {
    public:
      typedef BoostPtrHolder<QuickFAST::Messages::SequencePtr> TSequencePtr;

      MutableSequence();

      /// @brief sets the appliction type this FieldSet pertains to.
      property System::String^ ApplicationType
      {
        void set(System::String^ applicationType);
      }

      ///@brief Adds a field set to this sequence.
      void Add(FieldSet^ newFieldSet);

    private:
      TSequencePtr mutableSequencePtr_;
    };
  }
}
