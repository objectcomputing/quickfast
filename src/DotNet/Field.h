// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include <DotNet/Decimal.h>
#include <DotNet/Sequence.h>
#include <DotNet/FieldSet.h>
#include <Messages/Field.h>


namespace QuickFASTDotNet{

  value struct Decimal;

  namespace Messages{

    ref class FieldSet;
    ref class Sequence;

    public enum struct FieldType
    {
      INT32 = ::QuickFAST::Messages::Field::INT32,
      INT64 = ::QuickFAST::Messages::Field::INT64,
      UINT32 = ::QuickFAST::Messages::Field::UINT32,
      UINT64 = ::QuickFAST::Messages::Field::UINT64,
      DECIMAL = ::QuickFAST::Messages::Field::DECIMAL,
      ASCII = ::QuickFAST::Messages::Field::ASCII,
      UTF8 = ::QuickFAST::Messages::Field::UTF8,
      BYTEVECTOR = ::QuickFAST::Messages::Field::BYTEVECTOR,
      SEQUENCE = ::QuickFAST::Messages::Field::SEQUENCE,
      GROUP = ::QuickFAST::Messages::Field::GROUP,
      INT8 = ::QuickFAST::Messages::Field::INT8,     // not FAST Standard
      UINT8 = ::QuickFAST::Messages::Field::UINT8,    // not FAST Standard
      INT16 = ::QuickFAST::Messages::Field::INT16,    // not FAST Standard
      UINT16 = ::QuickFAST::Messages::Field::UINT16,   // not FAST Standard
      BITMAP = ::QuickFAST::Messages::Field::BITMAP    // not FAST Standard
    };

    public ref class Field
    {
    public:
      Field(const QuickFAST::Messages::FieldCPtr & cppField)
        : cppField_(cppField)
      {
      }

      ~Field()
      {
        this->!Field();
      }
      !Field()
      {
      }

      property bool isDefined
      {
        virtual bool get();
      }
      property FieldType Type
      {
        QuickFASTDotNet::Messages::FieldType get();
      }

      property int toInt32 {
        int get();
      }

      property unsigned int toUInt32{
        unsigned int get();
      }
      property __int64 toInt64{
        __int64 get();
      }
      property unsigned __int64 toUInt64{
        unsigned __int64 get();
      }

      property short toInt16 {
        short get();
      }

      property unsigned short toUInt16{
        unsigned short get();
      }

      property char toInt8 {
        char get();
      }

      property unsigned char toUInt8{
        unsigned char get();
      }

      property ::QuickFASTDotNet::Decimal toDecimal{
        Decimal get();
      }

      property System::String ^ toAscii{
        System::String ^ get();
      }

      property System::String ^ toUtf8{
        System::String ^ get();
      }

      property cli::array<unsigned char> ^ toByteVector{
        cli::array<unsigned char>  ^ get();
      }

      property FieldSet ^ toGroup{
        FieldSet ^ get();
      }

      property Sequence ^ toSequence{
        Sequence ^ get();
      }

      const QuickFAST::Messages::FieldCPtr & cppField_;
    };

  }
}
