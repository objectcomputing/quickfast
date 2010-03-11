// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#include <DotNet/DNSequence.h>
#include <DotNet/DNDecimal.h>
#include <DotNet/DNField.h>

namespace QuickFAST
{
  namespace DotNet
  {
    class ImplFieldSet;

    /// @brief A set of fields (Message, Group, or SequenceEntry)
    public ref class DNFieldSet
      : public System::Collections::Generic::IEnumerable<DNField ^ >
    {
    public:
      /// @brief construct the .NET field set to access the implementation field set
      DNFieldSet(ImplFieldSet & impl);

      ~DNFieldSet();

      /// @brief support enumeration
      virtual System::Collections::IEnumerator^ GetEnumerator() = System::Collections::IEnumerable::GetEnumerator;

      /// @brief support typesafe enumeration
      virtual System::Collections::Generic::IEnumerator<DNField ^ >^ GetSpecializedEnumerator()
        = System::Collections::Generic::IEnumerable<DNField^>::GetEnumerator;

      /// @brief How many fields are defined
      property int Count
      {
        int get();
      }

      /// @brief find a field with the given local name (ignores namespace)
      /// @param fieldName is the local name of the field
      /// @returns -1 if field not found
      int findIndexByName(System::String ^ fieldName);

      /// @brief find a field with the given local name (ignores namespace)
      /// @param fieldNamespace is the namespace qualifier for fieldName
      /// @param fieldName is the local name of the field
      /// @returns -1 if field not found
      int findIndexByQualifiedName(System::String ^ fieldName, System::String ^ fieldNamespace);

      /// @brief Find a field given an index
      /// @brief identifies the field (0 <= index < Count)
      /// @returns the field or nullptr
      DNField ^ getField(int index);

      /// @brief Find a field given the localName
      /// @param fieldName is the local name of the field
      /// @returns the field or nullptr
      DNField ^ findFieldByName(System::String ^ fieldName);

      /// @brief Find a field given the localName
      /// @param fieldName is the local name of the field
      /// @param fieldNamespace is the namespace qualifier for fieldName
      /// @returns the field or nullptr
      DNField ^ findFieldByQualifiedName(System::String ^ fieldName, System::String ^ fieldNamespace);

      /// @brief clear all fields and start over.
      void clear();

    private:
      ref class DNFieldSetEnumerator
        : public System::Collections::IEnumerator
        , public System::Collections::Generic::IEnumerator<DNField ^ >
      {
      public:

        // The enumerator requires a reference to the source of the interators
        // to prevent their source from being prematurely released.
        DNFieldSetEnumerator(ImplFieldSet * impl_, DNFieldSet^ parent);

        ~DNFieldSetEnumerator();

        property DNField ^ GenericCurrent{
          virtual DNField ^ get () = System::Collections::Generic::IEnumerator<DNField ^ >::Current::get;
        }

        virtual property System::Object^ Current{
          System::Object^ get () = System::Collections::IEnumerator::Current::get;
        }

        virtual bool MoveNext();
        virtual void Reset();

      private:
        DNFieldSet^ parent_;
        ImplFieldSet * impl_;
        size_t position_;
        size_t size_;
      };

    private:
      ImplFieldSet * impl_;
      bool owned_;
    };
  }
}
