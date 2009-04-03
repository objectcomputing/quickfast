// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include <DotNet/UnmanagedPtr.h>
#include <DotNet/StlDotNet.h>
#include <Messages/FieldIdentity_fwd.h>

namespace QuickFASTDotNet{
  namespace Messages{

    /// @brief Identify a field within a FieldSet
    ///
    public ref class FieldIdentity
    {
    public:
      typedef QuickFAST::Messages::FieldIdentityCPtr TcspFieldIdentity;
      typedef QuickFAST::Messages::FieldIdentityPtr TspFieldIdentity;
      typedef QuickFAST::Messages::FieldIdentity TFieldIdentity;


      /// @brief Construct an anonomous FieldIdentity
      FieldIdentity();

      FieldIdentity(System::String^ fieldName);

      FieldIdentity(System::String^ fieldNamespace, System::String^ fieldName);

      FieldIdentity(System::String^ fieldNamespace, System::String^ fieldName, System::String^ Id);

      /// @brief Gets or sets the local name for a field
      property System::String^ LocalName
      {
        System::String^ get();
        void set(System::String^ name);
      }

      /// @brief Gets or set the namespace for a field
      property System::String^ Namespace
      {
        System::String^ get();
        void set(System::String^ fieldNamespace);
      }

      /// @brief Gets or sets the id for a field
      property System::String^ Id
      {
        System::String^ get();
        void set(System::String^ id);
      }

      /// @brief Gets the name qualified by the namespace (from the cached value)
      property System::String^ Name
      {
        System::String^ get();
      }

      /// @brief Gets or set a flag indicating whether or not this field is require in a message
      property bool Mandatory
      {
        bool get();
        void set(bool value);
      }

      /// @brief Makes a copy of this field
      /// @returns a new instance equivalent to this field.
      FieldIdentity^ Clone();

      virtual String^ ToString() override
      {
        return System::String::Format("[FieldIdentity: name=\"{0}\", ID=\"{1}\" {2}]", this->Name, this->Id, this->Mandatory?"Manditory":"");
      }

    internal:

      FieldIdentity(const TspFieldIdentity& fieldIdentity);
      FieldIdentity(const TcspFieldIdentity& fieldIdentity);

      property TFieldIdentity& Ref
      {
        TFieldIdentity& get();
      }

      property TspFieldIdentity& SmartPtr
      {
        TspFieldIdentity& get();
      }


    private:

      BoostPtrHolder<TspFieldIdentity> spFieldIdentity_;

      FieldIdentity(FieldIdentity^ rhs);

    };
  }
}
