// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

namespace QuickFAST
{
  namespace DotNet
  {
    ref class DNMessageBuilder;
    ref class DNFieldSet;

    class ImplSequence;

    public ref class DNSequence
      : public System::Collections::Generic::IEnumerable<DNFieldSet^>
    {
    public:
      DNSequence(ImplSequence & impl);
      ~DNSequence();

      property System::String ^ LengthName
      {
        System::String ^ get();
      }

      property System::String ^ LengthNamespace
      {
        System::String ^ get();
      }

      property System::String ^ LengthId
      {
        System::String ^ get();
      }

      virtual System::Collections::IEnumerator^ GetEnumerator() = System::Collections::IEnumerable::GetEnumerator;

      virtual System::Collections::Generic::IEnumerator<DNFieldSet^>^ GetSpecializedEnumerator() = System::Collections::Generic::IEnumerable<DNFieldSet^>::GetEnumerator;

      /// How many entries are defined
      property int Count
      {
        int get();
      }

      DNFieldSet ^ entry(int   index);

    private:
      ref class DNSequenceEnumerator
        : public System::Collections::IEnumerator
        , public System::Collections::Generic::IEnumerator<DNFieldSet ^ >

      {
      public:

        // The enumerator requires a reference to the source of the interators
        // to prevent their source from being prematurely released.
        DNSequenceEnumerator(ImplSequence * impl_, DNSequence^ parent);

        ~DNSequenceEnumerator();

        property DNFieldSet^ GenericCurrent{
          virtual DNFieldSet^ get () = System::Collections::Generic::IEnumerator<DNFieldSet^>::Current::get;
        }

        virtual property System::Object^ Current{
          System::Object^ get () = System::Collections::IEnumerator::Current::get;
        }

        virtual bool MoveNext();
        virtual void Reset();

      private:
        DNSequence^ parent_;
        ImplSequence * impl_;
        size_t position_;
        size_t size_;
      };


    private:
      ImplSequence * impl_;
    };
  }
}
