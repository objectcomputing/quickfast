// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include <DotNet/DNDecimal.h>
#include <DotNet/DNSequence.h>

#pragma unmanaged
#include <Common/Value.h>
#pragma managed
namespace QuickFAST{
  namespace DotNet{

    ref class DNFieldSet;

    public enum struct FieldType
    {
      INT32 = ValueType::INT32,
      INT64 = ValueType::INT64,
      UINT32 = ValueType::UINT32,
      UINT64 = ValueType::UINT64,
      DECIMAL = ValueType::DECIMAL,
      ASCII = ValueType::ASCII,
      UTF8 = ValueType::UTF8,
      BYTEVECTOR = ValueType::BYTEVECTOR,
      SEQUENCE = ValueType::SEQUENCE,
      GROUP = ValueType::GROUP,
      INT8 = ValueType::INT8,     // not FAST Standard
      UINT8 = ValueType::UINT8,    // not FAST Standard
      INT16 = ValueType::INT16,    // not FAST Standard
      UINT16 = ValueType::UINT16,   // not FAST Standard
      BITMAP = ValueType::BITMAP,   // not FAST 1.1 Standard
      UNDEFINED = ValueType::UNDEFINED
    };

    struct ImplField;

    public ref class DNField
    {
    public:
      DNField(ImplField & impl);
      ~DNField();

      property System::String ^ LocalName
      {
        System::String ^ get();
      }

      property System::String ^ FieldNamespace
      {
        System::String ^ get();
      }

      property System::String ^ Id
      {
        System::String ^ get();
      }

      property bool IsDefined
      {
        bool get();
      }

      property FieldType Type
      {
        FieldType get();
      }

      property int Int32
      {
        int get();
      }

      property unsigned int UInt32
      {
        unsigned int get();
      }

      property __int64 Int64
      {
        __int64 get();
      }

      property unsigned __int64 UInt64
      {
        unsigned __int64 get();
      }

      property short Int16
      {
        short get();
      }

      property unsigned short UInt16
      {
        unsigned short get();
      }

      property signed char Int8
      {
        signed char get();
      }

      property unsigned char UInt8
      {
        unsigned char get();
      }

      property DNDecimal Decimal
      {
        DNDecimal get();
      }

      property System::String ^ Ascii
      {
        System::String ^ get();
      }

      property System::String ^ Utf8
      {
        System::String ^ get();
      }

      // todo sequence of octets
      property System::String ^ ByteVector
      {
        System::String ^ get();
      }


      property DNFieldSet ^ Group
      {
        DNFieldSet ^ get();
      }

      property DNSequence ^ Sequence
      {
        DNSequence ^ get();
      }


      property System::String ^ DisplayString
      {
        System::String ^ get();
      }

      property QuickFAST::mantissa_t Mantissa
      {
        QuickFAST::mantissa_t get();
      }

      property QuickFAST::exponent_t Exponent
      {
        QuickFAST::exponent_t get();
      }

    private:
      ImplField * impl_;
    };
  }
}
