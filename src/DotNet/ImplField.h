// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#pragma unmanaged
#include <Common/Value.h>
#include <Common/Decimal.h>
#include <Messages/FieldIdentity.h>

namespace QuickFAST{
  namespace DotNet{


    class ImplFieldSet;
    class ImplSequence;

    /// Basic implementation of a field
    struct ImplField
    {
    public:
      ImplField();
      ~ImplField();

      /// @brief Access the name= attribute of the field instruction element
      const std::string & localName();
      /// @brief Access the ns= attribute of the field instruction element
      const std::string & fieldNamespace();
      /// @brief Access the id= attribute of the field instruction element
      const std::string & id();
      /// @brief True if a value has been assigned
      bool isDefined();
      /// @brief What type of value does this field hold.
      ValueType::Type type();
      /// @brief The value as a 32 bit integer
      int toInt32();
      /// @brief The value as an unsigned 32 bit integer
      unsigned int toUInt32();
      /// @brief The value as a 64 bit integer
      __int64 toInt64();
      /// @brief The value as an unsigned 64 bit integer
      unsigned __int64 toUInt64();
      /// @brief The value as a 16 bit integer
      short toInt16();
      /// @brief The value as an unsigned 16 bit integer
      unsigned short toUInt16();
      /// @brief The value as an 8 bit integer
      signed char toInt8();
      /// @brief The value as an unsigned 8 bit integer
      unsigned char toUInt8();
      /// @brief The value as a FAST Decimal number
      Decimal toDecimal();
      /// @brief The value as an Ascii string
      const std::string & toAscii();
      /// @brief The value as a UTF8 string.
      const std::string & toUtf8();

      /// @brief The value as a sequence of octets
      const std::string & toByteVector();

      /// @brief The value as a field set
      ImplFieldSet & toGroup();

      /// @brief The value as a vector of field sets
      ImplSequence & toSequence();

      /// @brief A displayable version of the value
      const std::string & asString();

      /// @brief The mantissa portion of a Decimal value
      QuickFAST::mantissa_t mantissa();
      /// @brief The exponent portion of a Decimal value
      QuickFAST::exponent_t exponent();

      /// @brief Identify this field
      /// @param identity identifies this field.
      void setIdentity(const Messages::FieldIdentity & identity)
      {
        identity_ = identity;
      }

      /// @brief Store an unsigned value of the given type in the field
      void setUnsignedValue(ValueType::Type type, unsigned long long value);

      /// @brief Store a signed value of the given type in the field
      void setSignedValue(ValueType::Type type, long long value);

      /// @brief Store a Decimal value in the field
      void setDecimalValue(Decimal value);
      /// @brief Store a string value of the given type in the field
      void setStringValue(ValueType::Type type, const std::string & string);
      /// @brief Store a Sequence in the field
      void setSequence(ImplSequence * sequence);
      /// @brief Store a Group in the field
      void setGroup(ImplFieldSet * group);
    private:

      Messages::FieldIdentity identity_;
//      std::string localName_;
//      std::string fieldNamespace_;
//      std::string id_;

      ValueType::Type type_;

      ///@brief Data for any of the unsigned integral types.
      unsigned long long unsignedInteger_;
      ///@brief Data for any of the signed integral types. Also Decimal mantissa.
      signed long long signedInteger_;
      ///@brief Exponent for Decimal types (mantissa is in signedInteger_)
      QuickFAST::exponent_t exponent_;

      std::string string_;
      boost::scoped_ptr<ImplSequence> sequence_;
      boost::scoped_ptr<ImplFieldSet> group_;
    };

  }
}
#pragma managed

