// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include <DotNet/UnmanagedPtr.h>
#include <DotNet/FieldSet.h>

namespace QuickFASTDotNet{
  namespace Messages{

    /// @brief Internal representation of a set of fields to be encoded or decoded.
    [System::Diagnostics::DebuggerDisplay("Field Count = {Count}")]
    [System::Diagnostics::DebuggerTypeProxy(FieldSet::DbView::typeid)]
    public ref class MutableFieldSet: public FieldSet
    {
    public:
      typedef BoostPtrHolder<QuickFAST::Messages::FieldSetPtr> TFieldSetPtr;

      MutableFieldSet();
      explicit MutableFieldSet(unsigned int expectedNumberOfFields);

      void SetApplicationType(System::String^ applicationType, System::String^ nameSpace);

      /// @brief Add a field to the set.
      ///
      /// The FieldCPtr is copied, not the actual Field object.
      /// @param identity identifies this field
      /// @param value is the value to be assigned.
      void AddField(FieldIdentity^ identity, Field^ newField);

      property bool IsReadOnly { virtual bool get() override { return false; } }

    internal:
      property TFieldSet& FieldSetRef
      {
        TFieldSet& get() { return mutableFieldSetPtr_.GetRef(); }
      }

    private:
      TFieldSetPtr mutableFieldSetPtr_;
    };
  }
}
