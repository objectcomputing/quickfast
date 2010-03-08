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

    public ref class DNFieldSet
      : public System::Collections::Generic::IEnumerable<DNField ^ >
    {
    public:
      DNFieldSet(ImplFieldSet & impl);
      ~DNFieldSet();

      virtual System::Collections::IEnumerator^ GetEnumerator() = System::Collections::IEnumerable::GetEnumerator;

      virtual System::Collections::Generic::IEnumerator<DNField ^ >^ GetSpecializedEnumerator()
        = System::Collections::Generic::IEnumerable<DNField^>::GetEnumerator;

      /// How many fields are defined
      property int Count
      {
        int get();
      }

      /// @brief find a field with the given local name (ignores namespace)
      int findIndexByName(System::String ^ fieldName);

      DNField ^ getField(int index);

      DNField ^ findFieldByName(System::String ^ fieldName);

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
