// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionDecimal.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/FieldInstructionMantissa.h>
#include <Codecs/FieldInstructionExponent.h>
#include <Messages/SpecialAccessors.h>
#include <Messages/FieldDecimal.h>
#include <Messages/FieldInt32.h>
#include <Messages/FieldInt64.h>
#include <Common/Decimal.h>
#include <Messages/SingleValueBuilder.h>

#include <Common/Profiler.h>

using namespace QuickFAST;
using namespace QuickFAST::Codecs;


FieldInstructionDecimal::FieldInstructionDecimal(
      const std::string & name,
      const std::string & fieldNamespace)
  : FieldInstruction(name, fieldNamespace)
  , typedValueIsDefined_(false)
  , typedExponent_(0)
  , typedMantissa_(0)
  , typedValue_(0,0)
{
}

FieldInstructionDecimal::FieldInstructionDecimal()
  : typedValueIsDefined_(false)
  , typedExponent_(0)
  , typedMantissa_(0)
  , typedValue_(0,0)
{
}

FieldInstructionDecimal::~FieldInstructionDecimal()
{
}

void
FieldInstructionDecimal::setPresence(bool mandatory)
{
  FieldInstruction::setPresence(mandatory);
  if(!mandatory && bool(exponentInstruction_))
  {
    exponentInstruction_->setPresence(mandatory);
  }
}


void
FieldInstructionDecimal::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & accessor) const
{
  PROFILE_POINT("decimal::decodeNop");

  if(bool(exponentInstruction_))
  {
    Messages::SingleValueBuilder<int32> exponentBuilder;
    exponentInstruction_->decode(source, pmap, decoder, exponentBuilder);
    if(!exponentBuilder.isSet())
    {
      // null field
      return;
    }
    exponent_t exponent = static_cast<exponent_t>(exponentBuilder.value());

    Messages::SingleValueBuilder<mantissa_t> mantissaBuilder;
    mantissaInstruction_->decode(source, pmap, decoder, mantissaBuilder);
    mantissa_t mantissa = 0;
    if(mantissaBuilder.isSet())
    {
      mantissa = mantissaBuilder.value();
    }

    Decimal value(mantissa, exponent, false);
    accessor.addValue(identity_, ValueType::DECIMAL, value);
  }
  else
  {
    exponent_t exponent = 0;
    decodeSignedInteger(source, decoder, exponent, identity_.name());
    if(!isMandatory())
    {
      if(checkNullInteger(exponent))
      {
        return;
      }
    }
    mantissa_t mantissa;
    decodeSignedInteger(source, decoder, mantissa, identity_.name());
    Decimal value(mantissa, exponent);
    accessor.addValue(
      identity_,
      ValueType::DECIMAL,
      value);
  }
  return;
}

void
FieldInstructionDecimal::decodeConstant(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & /*decoder*/,
  Messages::ValueMessageBuilder & accessor) const
{
  PROFILE_POINT("decimal::decodeConstant");
  if(isMandatory() || pmap.checkNextField())
  {
    accessor.addValue(
      identity_,
      ValueType::DECIMAL,
      typedValue_);
  }
}

void
FieldInstructionDecimal::decodeDefault(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & accessor) const
{
  PROFILE_POINT("decimal::decodeDefault");
  if(pmap.checkNextField())
  {
    exponent_t exponent = 0;
    decodeSignedInteger(source, decoder, exponent, identity_.name());
    if(!isMandatory())
    {
      if(checkNullInteger(exponent))
      {
        return;
      }
    }
    mantissa_t mantissa;
    decodeSignedInteger(source, decoder, mantissa, identity_.name());
    Decimal value(mantissa, exponent);
    accessor.addValue(
      identity_,
      ValueType::DECIMAL,
      value);
  }
  else // field not in stream
  {
    if(typedValueIsDefined_)
    {
      accessor.addValue(
        identity_,
        ValueType::DECIMAL,
        typedValue_);
    }
    else if(isMandatory())
    {
      decoder.reportFatal("[ERR D5]", "Mandatory default operator with no value.", identity_);
    }
  }
}

void
FieldInstructionDecimal::decodeCopy(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & accessor) const
{
  PROFILE_POINT("decimal::decodeCopy");
  exponent_t exponent = 0;
  mantissa_t mantissa = 0;
  if(pmap.checkNextField())
  {
    decodeSignedInteger(source, decoder, exponent, identity_.name());
    if(isMandatory())
    {
      decodeSignedInteger(source, decoder, mantissa, identity_.name());
      Decimal value(mantissa, exponent, false);
      accessor.addValue(
        identity_,
        ValueType::DECIMAL,
        value);
      fieldOp_->setDictionaryValue(decoder, value);
    }
    else
    {
      // not mandatory means it's nullable
      if(checkNullInteger(exponent))
      {
        fieldOp_->setDictionaryValueNull(decoder);
      }
      else
      {
        decodeSignedInteger(source, decoder, mantissa, identity_.name());
        Decimal value(mantissa, exponent, false);
        accessor.addValue(
          identity_,
          ValueType::DECIMAL,
          value);
        fieldOp_->setDictionaryValue(decoder, value);
      }
    }

  }
  else // pmap says not present, use copy
  {
    Decimal value(0,0);
    Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(decoder, value);
    if(previousStatus == Context::UNDEFINED_VALUE)
    {
      // value not found in dictionary
      // not a problem..  use initial value if it's available
      if(fieldOp_->hasValue())
      {
        accessor.addValue(
          identity_,
          ValueType::DECIMAL,
          typedValue_);
        fieldOp_->setDictionaryValue(decoder, typedValue_);
      }
      else
      {
        if(isMandatory())
        {
          decoder.reportFatal("[ERR D5]", "Copy operator missing mandatory Decimal field/no initial value", identity_);
        }
      }
    }
    else if(previousStatus == Context::OK_VALUE)
    {
      accessor.addValue(
        identity_,
        ValueType::DECIMAL,
        value);
    }
    //else previous was null so don't put anything in the record
  }
}

void
FieldInstructionDecimal::decodeDelta(
  Codecs::DataSource & source,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & accessor) const
{
  PROFILE_POINT("decimal::decodeDelta");
  int64 exponentDelta;
  decodeSignedInteger(source, decoder, exponentDelta, identity_.name(), true);
  if(!isMandatory())
  {
    if(checkNullInteger(exponentDelta))
    {
      // nothing in Message; no change to saved value
      return;
    }
  }
  int64 mantissaDelta;
  decodeSignedInteger(source, decoder, mantissaDelta, identity_.name(), true);

  Decimal value(typedValue_);
  (void)fieldOp_->getDictionaryValue(decoder, value);
  value.setExponent(exponent_t(value.getExponent() + exponentDelta));
  value.setMantissa(mantissa_t(value.getMantissa() + mantissaDelta));
  accessor.addValue(
    identity_,
    ValueType::DECIMAL,
    value);
  fieldOp_->setDictionaryValue(decoder, value);
}

void
FieldInstructionDecimal::encodeNullableDecimal(
  Codecs::DataDestination & destination,
  WorkingBuffer & buffer,
  exponent_t exponent,
  mantissa_t mantissa) const
{
  if(exponent >= 0)
  {
    exponent += 1;
  }
  encodeDecimal(destination, buffer, exponent, mantissa);
}

void
FieldInstructionDecimal::encodeDecimal(
  Codecs::DataDestination & destination,
  WorkingBuffer & buffer,
  exponent_t exponent,
  mantissa_t mantissa) const
{
  encodeSignedInteger(destination, buffer, exponent);
  encodeSignedInteger(destination, buffer, mantissa);
}

void
FieldInstructionDecimal::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  // get the value from the application data
  Decimal value;
  if(accessor.getDecimal(identity_, ValueType::DECIMAL, value))
  {
    exponent_t exponent = value.getExponent();
    mantissa_t mantissa = value.getMantissa();

    if(bool(exponentInstruction_))
    {
      Messages::FieldCPtr exponentField(Messages::FieldInt32::create(exponent));
      Messages::SingleFieldAccessor exponentAccessor(exponentInstruction_->getIdentity(), exponentField);

      exponentInstruction_->encode(
        destination,
        pmap,
        encoder,
        exponentAccessor);

      Messages::FieldCPtr mantissaField(Messages::FieldInt64::create(mantissa));
      Messages::SingleFieldAccessor mantissaAccessor(mantissaInstruction_->getIdentity(), mantissaField);

      mantissaInstruction_->encode(
        destination,
        pmap,
        encoder,
        mantissaAccessor);

    }
    else
    {
      if(!isMandatory())
      {
        encodeNullableDecimal(
          destination,
          encoder.getWorkingBuffer(),
          value.getExponent(),
          value.getMantissa());
      }
      else
      {
        encodeDecimal(
          destination,
          encoder.getWorkingBuffer(),
          value.getExponent(),
          value.getMantissa());
      }
    }
  }
  else // not defined by accessor
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", identity_);
    }
    if(exponentInstruction_)
    {
      Messages::EmptyAccessor empty;
      exponentInstruction_->encode(
        destination,
        pmap,
        encoder,
        empty);
    }
    else
    {
      destination.putByte(nullInteger);
    }
  }
}

void
FieldInstructionDecimal::encodeConstant(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  if(!isMandatory())
  {
    // get the value from the application data
    Decimal value;
    if(accessor.getDecimal(identity_, ValueType::DECIMAL, value))
    {
      if(value != typedValue_)
      {
        encoder.reportFatal("[ERR U10]", "Constant value does not match application data.", identity_);
      }

      pmap.setNextField(true);
    }
    else // not defined by accessor
    {
      pmap.setNextField(false);
    }
  }
}

void
FieldInstructionDecimal::encodeDefault(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  // get the value from the application data
  Decimal value;
  if(accessor.getDecimal(identity_, ValueType::DECIMAL, value))
  {
    if(typedValueIsDefined_ &&
      value == typedValue_)
    {
      pmap.setNextField(false); // not in stream. use default
    }
    else
    {
      pmap.setNextField(true); // != default.  Send value

      if(isMandatory())
      {
        encodeDecimal(destination, encoder.getWorkingBuffer(), value.getExponent(), value.getMantissa());
      }
      else
      {
        encodeNullableDecimal(destination, encoder.getWorkingBuffer(), value.getExponent(), value.getMantissa());
      }
    }
  }
  else // not defined by accessor
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", identity_);
    }
    // if there is a default value
    // you have to cancel this by sending a null.
    if(fieldOp_->hasValue())
    {
      pmap.setNextField(true);
      destination.putByte(nullDecimal);
    }
    else
    {
      pmap.setNextField(false);
    }
  }
}


void
FieldInstructionDecimal::encodeCopy(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  Decimal previousValue(0,0);
  Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(encoder, previousValue);
  if(previousStatus == Context::UNDEFINED_VALUE)
  {
    if(fieldOp_->hasValue())
    {
      previousValue = typedValue_;
      fieldOp_->setDictionaryValue(encoder, previousValue);
      // pretend we got the data from the dictionary
      previousStatus = Context::OK_VALUE;
    }
    else
    {
      // pretend we got a null from the dictionary
      fieldOp_->setDictionaryValueNull(encoder);
      previousStatus = Context::NULL_VALUE;
    }
  }

  // get the value from the application data
  Decimal value;
  if(accessor.getDecimal(identity_, ValueType::DECIMAL, value))
  {
    if(previousStatus == Context::OK_VALUE && previousValue == value)
    {
      pmap.setNextField(false); // not in stream, use copy
    }
    else
    {
      pmap.setNextField(true);// value in stream
      if(isMandatory())
      {
        encodeDecimal(destination, encoder.getWorkingBuffer(), value.getExponent(), value.getMantissa());
      }
      else
      {
        encodeNullableDecimal(destination, encoder.getWorkingBuffer(), value.getExponent(), value.getMantissa());
      }
      fieldOp_->setDictionaryValue(encoder, value);
    }
  }
  else // not defined by accessor
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory decimal field.", identity_);
      // if reportFatal returns we're being lax about encoding rules
      // let the copy happen.
      pmap.setNextField(false);
    }
    else
    {
      // Missing optional field.  If we have a previous, non-null value
      // we need to explicitly null it out.  Otherwise just don't send it.
      if(previousValue != Context::NULL_VALUE)
      {
        pmap.setNextField(true);// value in stream
        destination.putByte(nullDecimal);
        fieldOp_->setDictionaryValueNull(encoder);
      }
      else
      {
        pmap.setNextField(false);
      }
    }
  }
}


void
FieldInstructionDecimal::encodeDelta(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  // assume default
  Decimal previousValue;

  // ... then initialize them from the dictionary
  Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(encoder, previousValue);
  if(previousStatus != Context::OK_VALUE && fieldOp_->hasValue())
  {
    previousValue = typedValue_;
    fieldOp_->setDictionaryValue(encoder, previousValue);
  }

  // get the value from the application data
  Decimal value;
  if(accessor.getDecimal(identity_, ValueType::DECIMAL, value))
  {
    int32 exponentDelta = static_cast<int32>(value.getExponent()) - int64(previousValue.getExponent());
    if(!isMandatory())
    {
      if(exponentDelta >= 0)
      {
        exponentDelta += 1;
      }
    }
    encodeSignedInteger(destination, encoder.getWorkingBuffer(), exponentDelta);

    int64 mantissaDelta = int64(value.getMantissa()) - int64(previousValue.getMantissa());
    encodeSignedInteger(destination, encoder.getWorkingBuffer(), mantissaDelta);

    if(previousStatus != Context::OK_VALUE  || value != previousValue)
    {
      fieldOp_->setDictionaryValue(encoder, value);
    }
  }
  else // not defined by accessor
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", identity_);
    }
    destination.putByte(nullInteger);
  }
}

void
FieldInstructionDecimal::interpretValue(const std::string & value)
{
  typedValue_.parse(value);
  typedValueIsDefined_ = true;
  typedMantissa_ = typedValue_.getMantissa();
  typedExponent_ = typedValue_.getExponent();

}

void
FieldInstructionDecimal::indexDictionaries(
  DictionaryIndexer & indexer,
  const std::string & dictionaryName,
  const std::string & typeName,
  const std::string & typeNamespace)
{
  FieldInstruction::indexDictionaries(indexer, dictionaryName,typeName, typeNamespace);
  if(bool(exponentInstruction_))
  {
    exponentInstruction_->indexDictionaries(indexer, dictionaryName,typeName, typeNamespace);
    mantissaInstruction_->indexDictionaries(indexer, dictionaryName,typeName, typeNamespace);
  }
}

void
FieldInstructionDecimal::finalize(TemplateRegistry & templateRegistry)
{
  if(bool(exponentInstruction_))
  {
    exponentInstruction_->finalize(templateRegistry);
    mantissaInstruction_->finalize(templateRegistry);
    presenceMapBitsUsed_ =
      exponentInstruction_->getPresenceMapBitsUsed() +
      mantissaInstruction_->getPresenceMapBitsUsed();
  }
  else
  {
    FieldInstruction::finalize(templateRegistry);
  }
}

ValueType::Type
FieldInstructionDecimal::fieldInstructionType()const
{
  return ValueType::DECIMAL;
}

void
FieldInstructionDecimal::displayBody(std::ostream & output, size_t indent)const
{
  if(bool(exponentInstruction_))
  {
    exponentInstruction_->display(output, indent);
  }
  if(bool(mantissaInstruction_))
  {
    mantissaInstruction_->display(output, indent);
  }
}
