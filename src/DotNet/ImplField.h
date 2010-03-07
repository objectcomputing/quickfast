// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#include <DotNet/DNField.h>
namespace QuickFAST{
  namespace DotNet{

    class ImplFieldSet;
    class ImplSequence;

    struct ImplField
    {
    public:
      ImplField();
      ~ImplField();

      const std::string & localName();
      const std::string & fieldNamespace();
      const std::string & id();
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
      const std::string & toAscii();
      const std::string & toUtf8();

      const std::string & toByteVector();
      ImplFieldSet & toGroup();

      ImplSequence & toSequence();

      const std::string & asString();

      QuickFAST::mantissa_t mantissa();
      QuickFAST::exponent_t exponent();

      void setLocalName(const std::string & localName);
      void setFieldNamespace(const std::string & fieldNamespace);
      void setId(const std::string & id);
      void setUnsignedValue(FieldType type, unsigned long long value);
      void setSignedValue(FieldType type, long long value);
      void setDecimalValue(Decimal value);
      void setStringValue(FieldType type, const std::string & string);
      void setSequence(ImplSequence * sequence);
      void setGroup(ImplFieldSet * group);
    private:

      std::string localName_;
      std::string fieldNamespace_;
      std::string id_;

      FieldType type_;

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
