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


bool
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
    if(!exponentInstruction_->decode(source, pmap, decoder, exponentBuilder))
    {
      return false;
    }
    if(!exponentBuilder.isSet())
    {
      // null field
      return true;
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
    decodeSignedInteger(source, decoder, exponent, identity_->name());
    if(!isMandatory())
    {
      if(checkNullInteger(exponent))
      {
        return true;
      }
    }
    mantissa_t mantissa;
    decodeSignedInteger(source, decoder, mantissa, identity_->name());
    Decimal value(mantissa, exponent);
    accessor.addValue(
      identity_,
      ValueType::DECIMAL,
      value);
  }
  return true;
}

bool
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
  return true;
}

bool
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
    decodeSignedInteger(source, decoder, exponent, identity_->name());
    if(!isMandatory())
    {
      if(checkNullInteger(exponent))
      {
        return true;
      }
    }
    mantissa_t mantissa;
    decodeSignedInteger(source, decoder, mantissa, identity_->name());
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
      decoder.reportFatal("[ERR D5]", "Mandatory default operator with no value.", *identity_);
    }
  }
  return true;
}

bool
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
    decodeSignedInteger(source, decoder, exponent, identity_->name());
    if(isMandatory())
    {
      decodeSignedInteger(source, decoder, mantissa, identity_->name());
      Decimal value(mantissa, exponent, false);
      accessor.addValue(
        identity_,
        ValueType::DECIMAL,
        value);
#if 0
      fieldOp_->setDictionaryValue(decoder, newField);
#else
      fieldOp_->setDictionaryValue(decoder, value);
#endif
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
        decodeSignedInteger(source, decoder, mantissa, identity_->name());
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
    if(fieldOp_->getDictionaryValue(decoder, value))
    {
      accessor.addValue(
        identity_,
        ValueType::DECIMAL,
        value);
    }
    else
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
          decoder.reportFatal("[ERR D5]", "Copy operator missing mandatory Decimal field/no initial value", *identity_);
        }
      }
    }
  }
  return true;
}

bool
FieldInstructionDecimal::decodeDelta(
  Codecs::DataSource & source,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & accessor) const
{
  PROFILE_POINT("decimal::decodeDelta");
  int64 exponentDelta;
  decodeSignedInteger(source, decoder, exponentDelta, identity_->name(), true);
  if(!isMandatory())
  {
    if(checkNullInteger(exponentDelta))
    {
      // nothing in Message; no change to saved value
      return true;
    }
  }
  int64 mantissaDelta;
  decodeSignedInteger(source, decoder, mantissaDelta, identity_->name(), true);

  Decimal value(typedValue_);
  Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(decoder, value);
  value.setExponent(exponent_t(value.getExponent() + exponentDelta));
  value.setMantissa(mantissa_t(value.getMantissa() + mantissaDelta));
  accessor.addValue(
    identity_,
    ValueType::DECIMAL,
    value);
  fieldOp_->setDictionaryValue(decoder, value);
  return true;
}
void
FieldInstructionDecimal::encodeNullableDecimal(
  Codecs::DataDestination & destination,
  WorkingBuffer & buffer,
  exponent_t exponent,
  mantissa_t mantissa) const
{
  if(exponent > 0)
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
  Messages::FieldCPtr field;
  if(accessor.getField(identity_->name(), field))
  {
    Decimal value = field->toDecimal();
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
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", *identity_);
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
  // get the value from the application data
  Messages::FieldCPtr field;
  if(accessor.getField(identity_->name(), field))
  {
    Decimal value = field->toDecimal();
    if(value != typedValue_)
    {
      encoder.reportFatal("[ERR U10]", "Constant value does not match application data.", *identity_);
    }

    if(!isMandatory())
    {
      pmap.setNextField(true);
    }
  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", *identity_);
    }
    pmap.setNextField(false);
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
  Messages::FieldCPtr field;
  if(accessor.getField(identity_->name(), field))
  {
    Decimal value = field->toDecimal();
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
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", *identity_);
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
  // declare a couple of variables...
  bool previousIsKnown = typedValueIsDefined_;
  bool previousNotNull = typedValueIsDefined_;
  Decimal previousValue(typedValue_);

  // ... then initialize them from the dictionary
#if 0
  Messages::FieldCPtr previousField;
  if(fieldOp_->findDictionaryField(encoder, previousField))
  {
    if(!previousField->isType(ValueType::DECIMAL))
    {
      encoder.reportFatal("[ERR D4]", "Previous value type mismatch.", *identity_);
    }
    previousIsKnown = true;
    previousNotNull = previousField->isDefined();
    if(previousNotNull)
    {
      previousValue = previousField->toDecimal();
    }
  }
#else
  if(fieldOp_->getDictionaryValue(encoder, previousValue))
  {
    previousIsKnown = true;
    previousNotNull = true;
    /// @TODO distinguish null vs unknown
  }
#endif
  if(!previousIsKnown && fieldOp_->hasValue())
  {
    previousIsKnown = true;
    previousNotNull = true;
    previousValue = typedValue_;
#if 0
    fieldOp_->setDictionaryValue(encoder, Messages::FieldDecimal::create(previousValue));
#else
    fieldOp_->setDictionaryValue(encoder, previousValue);
#endif

  }

  // get the value from the application data
  Messages::FieldCPtr field;
  if(accessor.getField(identity_->name(), field))
  {
    Decimal value = field->toDecimal();

    if(previousNotNull && previousValue == value)
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
#if 0
      field = Messages::FieldDecimal::create(value);
      fieldOp_->setDictionaryValue(encoder, field);
#else
      fieldOp_->setDictionaryValue(encoder, value);
#endif
    }
  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", *identity_);
      // if reportFatal returns we're being lax about encoding rules
      // send a dummy value
      destination.putByte(zeroIntegerNonnullable);//exponent
      destination.putByte(zeroIntegerNonnullable);//mantissa
#if 0
      field = Messages::FieldDecimal::create(0,0);
      fieldOp_->setDictionaryValue(encoder, field);
#else
      fieldOp_->setDictionaryValue(encoder, Decimal(0,0));
#endif

    }
    else
    {
      if(previousIsKnown && previousNotNull)
      {
        pmap.setNextField(true);// value in stream
        destination.putByte(nullDecimal);
#if 0
        field = Messages::FieldDecimal::createNull();
        fieldOp_->setDictionaryValue(encoder, field);
#else
        fieldOp_->setDictionaryValueNull(encoder);
#endif

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

  // declare a couple of variables...
  bool previousIsKnown = false;
  bool previousNotNull = false;
  Decimal previousValue(typedValue_);

  // ... then initialize them from the dictionary
#if 0
  Messages::FieldCPtr previousField;
  if(fieldOp_->findDictionaryField(encoder, previousField))
  {
    if(!previousField->isType(ValueType::DECIMAL))
    {
      encoder.reportFatal("[ERR D4]", "Previous value type mismatch.", *identity_);
    }
    previousIsKnown = true;
    previousNotNull = previousField->isDefined();
    if(previousNotNull)
    {
      previousValue = previousField->toDecimal();
    }
  }
#else
  if(fieldOp_->getDictionaryValue(encoder, previousValue))
  {
    previousIsKnown = true;
    previousNotNull = true;
    /// @TODO distinguish null vs unknown
  }
#endif

  if(!previousIsKnown && fieldOp_->hasValue())
  {
    previousIsKnown = true;
    previousNotNull = true;
    previousValue = typedValue_;
#if 0
    fieldOp_->setDictionaryValue(encoder, Messages::FieldDecimal::create(previousValue));
#else
    fieldOp_->setDictionaryValue(encoder, previousValue);
#endif

  }

  // get the value from the application data
  Messages::FieldCPtr field;
  if(accessor.getField(identity_->name(), field))
  {
    Decimal value = field->toDecimal();

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

    if(!previousIsKnown  || value != previousValue)
    {
#if 0
      field = Messages::FieldDecimal::create(value);
      fieldOp_->setDictionaryValue(encoder, field);
#else
      fieldOp_->setDictionaryValue(encoder, value);
#endif

    }

  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", *identity_);
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
