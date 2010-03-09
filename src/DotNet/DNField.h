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

    /// @brief what type of data is contained in a DNField
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

    /// @brief the unmanaged C++ implementation of a Field
    struct ImplField;

    /// @brief Represent a single field to a .NET application
    public ref class DNField
    {
    public:
      /// @brief construct the .NET field to access the implementation field
      DNField(ImplField & impl);
      ~DNField();

      /// @brief from the XML: the name= attribute for a field instruction used to produce this field
      property System::String ^ LocalName
      {
        System::String ^ get();
      }

      /// @brief from the XML: the ns= attribute for a field instruction used to produce this field
      property System::String ^ FieldNamespace
      {
        System::String ^ get();
      }


      /// @brief from the XML: the id= attribute for a field instruction used to produce this field
      property System::String ^ Id
      {
        System::String ^ get();
      }

      /// @brief true if a value is defined ('most always true)
      property bool IsDefined
      {
        bool get();
      }

      /// @brief what type of data this field contains
      property FieldType Type
      {
        FieldType get();
      }

      /// @brief the field considered as a 32 bit integer (may throw for inappropriate field types.)
      property int Int32
      {
        int get();
      }

      /// @brief the field considered as an unsigned 32 bit integer (may throw for inappropriate field types.)
      property unsigned int UInt32
      {
        unsigned int get();
      }

      /// @brief the field considered as a 64 bit integer (may throw for inappropriate field types.)
      property __int64 Int64
      {
        __int64 get();
      }

      /// @brief the field considered as an unsigned 64 bit integer (may throw for inappropriate field types.)
      property unsigned __int64 UInt64
      {
        unsigned __int64 get();
      }

      /// @brief the field considered as a 16 bit integer (may throw for inappropriate field types.)
      property short Int16
      {
        short get();
      }

      /// @brief the field considered as an unsigned 16 bit integer (may throw for inappropriate field types.)
      property unsigned short UInt16
      {
        unsigned short get();
      }

      /// @brief the field considered as an 8 bit integer (may throw for inappropriate field types.)
      property signed char Int8
      {
        signed char get();
      }

      /// @brief the field considered as an unsigned 8 bit integer (may throw for inappropriate field types.)
      property unsigned char UInt8
      {
        unsigned char get();
      }

      /// @brief the field considered as FAST Decimal (may throw for inappropriate field types.)
      property DNDecimal Decimal
      {
        DNDecimal get();
      }

      /// @brief the field considered as an Ascii string (may throw for inappropriate field types.)
      property System::String ^ Ascii
      {
        System::String ^ get();
      }

      /// @brief the field considered as a UTF8 string (may throw for inappropriate field types.)
      property System::String ^ Utf8
      {
        System::String ^ get();
      }

      /// @brief the field considered as a sequence of octets (ByteVector) (may throw for inappropriate field types.)
      /// @todo: use an appropriate data type
      property System::String ^ ByteVector
      {
        System::String ^ get();
      }

      /// @brief the field considered as Group (field set) (may throw for inappropriate field types.)
      property DNFieldSet ^ Group
      {
        DNFieldSet ^ get();
      }

      /// @brief the field considered as Sequence (vector of field sets) (may throw for inappropriate field types.)
      property DNSequence ^ Sequence
      {
        DNSequence ^ get();
      }


      /// @brief A displayable version of the field: may be somewhat slow for non-string data types.
      property System::String ^ DisplayString
      {
        System::String ^ get();
      }

      /// @brief The mantissa portion of a Decimal field (may throw for inappropriate field types.)
      property QuickFAST::mantissa_t Mantissa
      {
        QuickFAST::mantissa_t get();
      }

      /// @brief The exponent portion of a Decimal field (may throw for inappropriate field types.)
      property QuickFAST::exponent_t Exponent
      {
        QuickFAST::exponent_t get();
      }

    private:
      ImplField * impl_;
    };
  }
}
