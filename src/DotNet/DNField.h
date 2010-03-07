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

      System::String ^ localName();
      System::String ^ fieldNamespace();
      System::String ^ id();
      bool isDefined();
      FieldType type();
      int toInt32();
      unsigned int toUInt32();
      __int64 toInt64();
      unsigned __int64 toUInt64();
      short toInt16();
      unsigned short toUInt16();
      signed char toInt8();
      unsigned char toUInt8();
      DNDecimal toDecimal();
      System::String ^ toAscii();
      System::String ^ toUtf8();

      // todo sequence of octets
      System::String ^ toByteVector();
      DNFieldSet ^ toGroup();

      DNSequence ^ toSequence();

      System::String ^ asString();

      QuickFAST::mantissa_t mantissa();
      QuickFAST::exponent_t exponent();

    private:
      ImplField * impl_;
    };
  }
}
