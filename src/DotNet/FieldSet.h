// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include <DotNet/UnmanagedPtr.h>
#include <DotNet/StlDotNet.h>
#include <Messages/FieldSet.h>

namespace QuickFASTDotNet{
  namespace Messages {

    ref class Field;
    ref class FieldIdentity;

    /// @brief Internal representation of a set of fields to be encoded or decoded.
    [System::Diagnostics::DebuggerDisplay("Field Count = {Count}")]
    [System::Diagnostics::DebuggerTypeProxy(FieldSet::DbView::typeid)]
    public ref class FieldSet:
      public System::Collections::Generic::ICollection<System::Collections::Generic::KeyValuePair<FieldIdentity^, Field^> >,
      public System::Collections::Generic::IEnumerable<System::Collections::Generic::KeyValuePair<FieldIdentity^, Field^> >
    {
    public:
      typedef QuickFAST::Messages::FieldSet TFieldSet;
      typedef BoostPtrHolder<QuickFAST::Messages::FieldSetCPtr> TFieldSetCPtr;

      typedef System::Collections::IEnumerable TEnumerable;
      typedef System::Collections::IEnumerator TEnumerator;
      typedef System::Collections::Generic::KeyValuePair<FieldIdentity^, Field^> TKeyValuePair;
      typedef System::Collections::Generic::ICollection<TKeyValuePair> TGenericCollection;
      typedef System::Collections::Generic::IEnumerator<TKeyValuePair> TGenericEnumerator;
      typedef System::Collections::Generic::IEnumerable<TKeyValuePair> TGenericEnumerable;

      property TKeyValuePair default[System::String^]
      {
        TKeyValuePair get(System::String^ fieldName);
      }

      /// @brief identify the application type associated with
      /// this set of fields via typeref.
      property System::String^ ApplicationType
      {
        System::String^ get();
      }

      /// @brief number of fields in this field set.
      property int Count
      {
        virtual int get() { return spFieldSet_->size(); }
      }

      virtual TEnumerator^ GetEnumerator() = System::Collections::IEnumerable::GetEnumerator
      {
        return gcnew FieldEnumerator(spFieldSet_->begin(), spFieldSet_->end(), this);
      }

      virtual TGenericEnumerator^ GetSpecializedEnumerator() = TGenericEnumerable::GetEnumerator
      {
        return gcnew FieldEnumerator(spFieldSet_->begin(), spFieldSet_->end(), this);
      }

      /// @brief Is the specified field present in this set?
      /// @param name Identifies the field.
      /// @returns true if the field is present.  Returns false if the field is
      /// unknown or doesn't currently have a value in this set.
      bool IsPresent(System::String^ name);

      Field^ GetField(System::String^ name);

      /// @brief Get the identity information for the specified field
      /// @param[in] name identifies the desired field
      /// @param[out] identity is the information for the field that was found
      /// @returns true if the field was found
      FieldIdentity^ GetIdentity(System::String^ name);


      /// @brief indicates whether this fieldset is readonly or not.
      property bool IsReadOnly { virtual bool get() { return true; } }
      virtual void Add(TKeyValuePair item);
      virtual void Clear();
      virtual bool Contains(TKeyValuePair item);
      virtual void CopyTo(array<TKeyValuePair>^ array, int arrayIndex);
      virtual bool Remove(TKeyValuePair item);

      virtual String^ ToString() override
      {
        return System::String::Format("FieldSet: Field Count = {0}", this->Count);
      }

    internal:

      ref class DbView
      {
      public:
        DbView(FieldSet^ viewedContainer)
          :viewedContainer_(viewedContainer)
        {
        }

        [System::Diagnostics::DebuggerBrowsable(System::Diagnostics::DebuggerBrowsableState::RootHidden)]
        property array<FieldSet::TKeyValuePair>^ Keys
        {
          array<FieldSet::TKeyValuePair>^ get()
          {
            typedef System::Collections::Generic::List<FieldSet::TKeyValuePair> TList;
            TList^ outList = gcnew TList();

            System::Collections::Generic::IEnumerator<FieldSet::TKeyValuePair>^ enumerator = viewedContainer_->GetSpecializedEnumerator();
            do
            {
              outList->Add(enumerator->Current);
            }while(enumerator->MoveNext());

            return outList->ToArray();
          }
        }
      private:
        FieldSet^ viewedContainer_;
      };


      /// @brief Constructs a new FieldSet instance increasing the ref count of the QuickFAST::Messages::FieldSetPtr shared_ptr.
      explicit FieldSet(const QuickFAST::Messages::FieldSetCPtr& fieldSet);

      /// Returns a reference to the unmanaged FieldSet entity.
      property const TFieldSet& FieldSetRef
      {
        const TFieldSet& get() { return spFieldSet_.GetRef(); }
      }

      /// Gets a reference to the BoostPtrHolder holding the FieldSetPtr boost shared_ptr
      property TFieldSetCPtr% FieldSetCPtr
      {
        TFieldSetCPtr% get() { return spFieldSet_; }
      }

    private:

      ref class FieldEnumerator:
        public TEnumerator,
        public TGenericEnumerator
      {
      public:
        typedef QuickFAST::Messages::FieldSet::const_iterator const_iterator;
        typedef StlDotNet::IteratorHolder<const_iterator> TIteratorHolder;

        // The enumerator requires a reference to the source of the interators
        // to prevent their source from being prematurely released.
        FieldEnumerator(const_iterator it, const_iterator end, FieldSet^ parent)
          : itHolder_(new TIteratorHolder(it, end))
          , parent_(parent)
        {
        }

        ~FieldEnumerator(){}

        property TKeyValuePair GenericCurrent{
          virtual TKeyValuePair get() = FieldSet::TGenericEnumerator::Current::get;
        }

        property System::Object^ Current{
          virtual System::Object^ get() = FieldSet::TEnumerator::Current::get;
        }

        virtual bool MoveNext() = IEnumerator::MoveNext;
        virtual void Reset() = IEnumerator::Reset;

      private:
        UnmanagedPtr<StlDotNet::IteratorHolder<const_iterator> > itHolder_;
        FieldSet^ parent_;
      };

    protected private:
      TFieldSetCPtr spFieldSet_;
    };
  }
}
