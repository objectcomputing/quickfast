// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONDECIMAL_H
#define FIELDINSTRUCTIONDECIMAL_H
#include <Codecs/FieldInstructionMantissa.h>
#include <Codecs/FieldInstructionExponent.h>
#include <Codecs/FieldInstruction.h>
#include <Common/Decimal.h>
namespace QuickFAST{
  namespace Codecs{
    ///@brief A FieldInstruction to encode/decode a Decimal data type.
    class QuickFAST_Export FieldInstructionDecimal : public FieldInstruction
    {
    public:
      /// @brief construct with a name and a namespace
      /// @param name is the local name
      /// @param fieldNamespace is the namespace to qualify this name
      FieldInstructionDecimal(
        const std::string & name,
        const std::string & fieldNamespace);

      /// @brief construct anonomous field instruction
      FieldInstructionDecimal();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionDecimal();

      virtual void setPresence(bool mandatory);

      /// @brief Support &lt;mantissa> element.
      ///
      /// The mantissa field instruction may have special operations or properties.
      virtual void setMantissaInstruction(FieldInstructionPtr mantissa)
      {
        mantissaInstruction_ = mantissa;
      }

      /// @brief Get the mantissa field instruction.
      virtual bool getMantissaInstruction(FieldInstructionCPtr & mantissa) const
      {
        mantissa = mantissaInstruction_;
        return bool(mantissa);
      }

      /// @brief Support &lt;exponent> element.
      ///
      /// The exponent field instruction may have special operations or properties.
      virtual void setExponentInstruction(FieldInstructionPtr exponent)
      {
        exponentInstruction_ = exponent;
        if(!isMandatory())
        {
          exponentInstruction_->setPresence(false);
        }
      }

      /// @brief Get the exponent field instruction.
      virtual bool getExponentInstruction(FieldInstructionCPtr & exponent) const
      {
        exponent = exponentInstruction_;
        return bool(exponent);
      }

      /// @brief Set the default/constant/copy, etc value.
      void setValue(exponent_t exponent, mantissa_t mantissa)
      {
        typedExponent_ = exponent;
        typedMantissa_ = mantissa;
        typedValueIsDefined_ = true;
        typedValue_ = Decimal(mantissa, exponent);
      }

      // virtual methods defined and documented in FieldInstruction
      virtual void decodeNop(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void decodeConstant(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void decodeDefault(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void decodeCopy(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void decodeDelta(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void encodeNop(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void encodeConstant(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void encodeDefault(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void encodeCopy(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void encodeDelta(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void indexDictionaries(
        DictionaryIndexer & indexer,
        const std::string & dictionaryName,
        const std::string & typeName,
        const std::string & typeNamespace);

      virtual void finalize(TemplateRegistry & registry);
      virtual ValueType::Type fieldInstructionType()const;
      virtual void displayBody(std::ostream & output, size_t indent)const;

    private:
      void encodeDecimal(
        Codecs::DataDestination & destination,
        WorkingBuffer & buffer,
        exponent_t exponent,
        mantissa_t mantissa) const;

      void encodeNullableDecimal(
        Codecs::DataDestination & destination,
        WorkingBuffer & buffer,
        exponent_t exponent,
        mantissa_t mantissa) const;

    private:
      void interpretValue(const std::string & value);

    private:
      bool typedValueIsDefined_;
      exponent_t typedExponent_;
      mantissa_t typedMantissa_;
      // redundant, but efficient.  Must be Decimal(typedMantissa_, typedExponent_);
      Decimal typedValue_;
      FieldInstructionPtr mantissaInstruction_;
      FieldInstructionPtr exponentInstruction_;
    };
  }
}
#endif // FIELDINSTRUCTIONDECIMAL_H
