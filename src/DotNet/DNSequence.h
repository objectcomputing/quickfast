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

    /// @brief C++ implementation of a sequence
    class ImplSequence;

    /// @brief Sequence of field sets.
    public ref class DNSequence
      : public System::Collections::Generic::IEnumerable<DNFieldSet^>
    {
    public:
      /// @brief wrap the .NET accessor around a Sequence implementatoin.
      DNSequence(ImplSequence & impl);
      ~DNSequence();

      /// @brief the name of the length field in the sequence
      property System::String ^ LengthName
      {
        System::String ^ get();
      }

      /// @brief the namespace of the length field in the sequence
      property System::String ^ LengthNamespace
      {
        System::String ^ get();
      }

      /// @brief the id of the length field in the sequence
      property System::String ^ LengthId
      {
        System::String ^ get();
      }

      /// @brief support enumeration
      virtual System::Collections::IEnumerator^ GetEnumerator() = System::Collections::IEnumerable::GetEnumerator;

      /// @brief support enumeration
      virtual System::Collections::Generic::IEnumerator<DNFieldSet^>^ GetSpecializedEnumerator() = System::Collections::Generic::IEnumerable<DNFieldSet^>::GetEnumerator;

      /// @brief How many entries are defined
      property int Count
      {
        int get();
      }

      /// @brief Access a specific entry by index (0 <= index <= Count)
      DNFieldSet ^ entry(int index);

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

        virtual property DNFieldSet^ GenericCurrent{
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
