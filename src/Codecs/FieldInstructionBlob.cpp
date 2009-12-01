// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionBlob.h"
#include <Codecs/DataSource.h>
#include <Codecs/DataDestination.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>
#include <Messages/MessageBuilder.h>
#include <Messages/Field.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionBlob::FieldInstructionBlob(
  const std::string & name,
  const std::string & fieldNamespace)
  : FieldInstruction(name, fieldNamespace)
{
}

FieldInstructionBlob::FieldInstructionBlob()
{
}

FieldInstructionBlob::~FieldInstructionBlob()
{
}

bool
FieldInstructionBlob::decodeFromSource(
  Codecs::DataSource & source,
  Codecs::Context & context,
  bool mandatory,
  WorkingBuffer & buffer,
  Messages::FieldCPtr & field) const
{
  PROFILE_POINT("blob::decodeFromSource");

  uint32 length;
  decodeUnsignedInteger(source, context, length, identity_->name());
  if(!mandatory)
  {
    if(checkNullInteger(length))
    {
      // optional and missing.  we're done
      return true;
    }
  }
  decodeByteVector(context, source, identity_->name(), buffer, length);
  field = createField(buffer.begin(), buffer.size());
  return true;
}

bool
FieldInstructionBlob::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::MessageBuilder & fieldSet) const
{
  PROFILE_POINT("blob::decodeNop");
  // note NOP never uses pmap.  It uses a null value instead for optional fields
  // so it's always safe to do the basic decode.
  Messages::FieldCPtr field;
  if(!decodeFromSource(source, decoder, isMandatory(), decoder.getWorkingBuffer(), field))
  {
    return false;
  }
  if(field)
  {
    fieldSet.addField(identity_, field);
  }
  return true;
}

bool
FieldInstructionBlob::decodeConstant(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & /*decoder*/,
  Messages::MessageBuilder & fieldSet) const
{
  PROFILE_POINT("blob::decodeConstant");
  if(isMandatory())
  {
    fieldSet.addField(
      identity_,
      initialValue_);
  }
  else
  {
    if(pmap.checkNextField())
    {
      fieldSet.addField(
        identity_,
        initialValue_);
    }
    else
    {
      // not present. Nothing to do
    }
  }
  return true;
}

bool
FieldInstructionBlob::decodeDefault(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::MessageBuilder & fieldSet) const
{
  PROFILE_POINT("blob::decodeDefault");
  if(pmap.checkNextField())
  {
    Messages::FieldCPtr field;
    if(!decodeFromSource(source, decoder, isMandatory(), decoder.getWorkingBuffer(), field))
    {
      return false;
    }
    if(field)
    {
      fieldSet.addField(
        identity_,
        field);
    }
    return true;
  }
  else // pmap says nothing in stream
  {
    if(fieldOp_->hasValue())
    {
      fieldSet.addField(
        identity_,
        initialValue_);
    }
    else if(isMandatory())
    {
      decoder.reportFatal("[ERR D5]", "Mandatory default operator with no value.", *identity_);
    }

  }
  return true;
}

bool
FieldInstructionBlob::decodeCopy(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::MessageBuilder & fieldSet) const
{
  PROFILE_POINT("blob::decodeCopy");
  if(pmap.checkNextField())
  {
    // field is in the stream, use it
    Messages::FieldCPtr field;
    if(!decodeFromSource(source, decoder, isMandatory(), decoder.getWorkingBuffer(), field))
    {
      return false;
    }
    if(field)
    {
      fieldSet.addField(
        identity_,
        field);
      fieldOp_->setDictionaryValue(decoder, field);
    }
  }
  else // pmap says not in stream
  {
    Messages::FieldCPtr previousField;
    if(!fieldOp_->findDictionaryField(decoder, previousField))
    {
      previousField = initialValue_;
    }

    if(bool(previousField) && previousField->isDefined())
    {
      fieldSet.addField(
        identity_,
        previousField);
    }
    else
    {
      if(isMandatory())
      {
        decoder.reportFatal("[ERR D6]", "No value available for mandatory copy field.", *identity_);
      }
    }
  }
  return true;
}

bool
FieldInstructionBlob::decodeDelta(
  Codecs::DataSource & source,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::MessageBuilder & fieldSet) const
{
  PROFILE_POINT("blob::decodeDelta");
  int32 deltaLength;
  decodeSignedInteger(source, decoder, deltaLength, identity_->name());
  if(!isMandatory())
  {
    if(checkNullInteger(deltaLength))
    {
      // NULL delta does not clear previous
      // so there's nothing to do
      return true;
    }
  }
  Messages::FieldCPtr deltaField;
  if(!decodeFromSource(source, decoder, true, decoder.getWorkingBuffer(), deltaField))
  {
    return false;
  }
  const std::string deltaValue = deltaField->toString();
  Messages::FieldCPtr previousField;
  if(!fieldOp_->findDictionaryField(decoder, previousField))
  {
    previousField = initialValue_;
  }
  std::string previousValue;
  if(previousField->isDefined())
  {
    previousValue = previousField->toString();
  }
  size_t previousLength = previousValue.length();

  if( deltaLength < 0)
  {
    // operate on front of string
    // compensete for the excess -11 encoding that allows -0 != +0
    deltaLength = -(deltaLength + 1);
    // don't chop more than is there
    if(static_cast<unsigned long>(deltaLength) > previousLength)
    {
      deltaLength = QuickFAST::int32(previousLength);
    }
    Messages::FieldCPtr field = createField(
      deltaValue + previousValue.substr(deltaLength));
    fieldSet.addField(
      identity_,
      field);
    fieldOp_->setDictionaryValue(decoder, field);
  }
  else
  { // operate on end of string
    // don't chop more than is there
    if(static_cast<unsigned long>(deltaLength) > previousLength)
    {
      deltaLength = QuickFAST::uint32(previousLength);
    }
    Messages::FieldCPtr field = createField(
      previousValue.substr(0, previousLength - deltaLength) + deltaValue);
    fieldSet.addField(
      identity_,
      field);
    fieldOp_->setDictionaryValue(decoder, field);
  }
  return true;
}

bool
FieldInstructionBlob::decodeTail(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::MessageBuilder & fieldSet) const
{
  PROFILE_POINT("blob::decodeTail");
  if(pmap.checkNextField())
  {
    // field is in the stream, use it
    Messages::FieldCPtr tailField;
    if(!decodeFromSource(source, decoder, isMandatory(), decoder.getWorkingBuffer(), tailField))
    {
      return false;
    }
    if(bool(tailField)) // NULL?
    {
      const std::string & tailValue = tailField->toString();

      Messages::FieldCPtr previousField;
      if(!fieldOp_->findDictionaryField(decoder, previousField))
      {
        previousField = initialValue_;
      }
      std::string previousValue;
      if(previousField->isDefined())
      {
        previousValue = previousField->toString();
      }

      size_t tailLength = tailValue.length();
      size_t previousLength = previousValue.length();
      if(tailLength > previousLength)
      {
        tailLength = previousLength;
      }
      Messages::FieldCPtr field = createField(previousValue.substr(0, previousLength - tailLength) + tailValue);
      fieldSet.addField(
        identity_,
        field);
      fieldOp_->setDictionaryValue(decoder, field);
    }
    else // null
    {
      fieldOp_->setDictionaryValue(decoder, createEmptyField());
    }
  }
  else // pmap says not in stream
  {
    Messages::FieldCPtr previousField;
    if(!fieldOp_->findDictionaryField(decoder, previousField))
    {
      previousField = initialValue_;
      fieldOp_->setDictionaryValue(decoder, previousField);
    }
    if(bool(previousField) && previousField->isDefined())
    {
      fieldSet.addField(
        identity_,
        previousField);
    }
    else
    {
      if(isMandatory())
      {
        decoder.reportFatal("[ERR D6]", "No value available for mandatory copy field.", *identity_);
      }
    }
  }
  return true;
}

void
FieldInstructionBlob::encodeNullableBlob(
  Codecs::DataDestination & destination,
  Codecs::Context & /*context*/,
  WorkingBuffer & buffer,
  const std::string & value) const
{
    uint32 length = QuickFAST::uint32(value.length());
    length += 1;
    encodeUnsignedInteger(destination, buffer, length);
    encodeBlobData(destination, value);
}

void
FieldInstructionBlob::encodeBlob(
  Codecs::DataDestination & destination,
  WorkingBuffer & buffer,
  const std::string & value) const
{
    uint32 length = QuickFAST::uint32(value.length());
    encodeUnsignedInteger(destination, buffer, length);
    encodeBlobData(destination, value);
}

void
FieldInstructionBlob::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & fieldSet) const
{
    // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    std::string value = field->toString();
    if(!isMandatory())
    {
      encodeNullableBlob(destination, encoder, encoder.getWorkingBuffer(), value);
    }
    else
    {
      encodeBlob(destination, encoder.getWorkingBuffer(), value);
    }
  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", *identity_);
    }
    destination.putByte(nullBlob);
  }
}

void
FieldInstructionBlob::encodeConstant(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & fieldSet) const
{
  // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    const std::string & value = field->toString();
    const std::string & constant = initialValue_->toString();
    if(value != constant)
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
FieldInstructionBlob::encodeDefault(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & fieldSet) const
{
  // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    std::string value = field->toString();
    if(initialValue_->isDefined() &&
      initialValue_->toString() == value)
    {
      pmap.setNextField(false); // not in stream. use default
    }
    else
    {
      pmap.setNextField(true); // != default.  Send value
      if(!isMandatory())
      {
        encodeNullableBlob(destination, encoder, encoder.getWorkingBuffer(), value);
      }
      else
      {
        encodeBlob(destination,  encoder.getWorkingBuffer(), value);
      }
    }
  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", *identity_);
    }
    if(fieldOp_->hasValue())
    {
      pmap.setNextField(true);
      destination.putByte(nullBlob);
    }
    else
    {
      pmap.setNextField(false);
    }
  }
}


void
FieldInstructionBlob::encodeCopy(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & fieldSet) const
{
  // declare a couple of variables...
  bool previousIsKnown = false;
  bool previousNotNull = false;
  std::string previousValue;

  // ... then initialize them from the dictionary
  Messages::FieldCPtr previousField;
  if(fieldOp_->findDictionaryField(encoder, previousField))
  {
    if(!previousField->isString())
    {
      encoder.reportFatal("[ERR D4]", "Previous value type mismatch.", *identity_);
    }
    previousIsKnown = true;
    previousNotNull = previousField->isDefined();
    if(previousNotNull)
    {
      previousValue = previousField->toString();
    }
  }
  if(!previousIsKnown && fieldOp_->hasValue())
  {
    previousIsKnown = true;
    previousValue = initialValue_->toString();
  }

  // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    std::string value = field->toString();
    if(previousIsKnown && previousValue == value)
    {
      pmap.setNextField(false); // not in stream, use copy
    }
    else
    {
      pmap.setNextField(true);// value in stream
      if(!isMandatory())
      {
        encodeNullableBlob(destination, encoder, encoder.getWorkingBuffer(), value);
      }
      else
      {
        encodeBlob(destination, encoder.getWorkingBuffer(), value);
      }
      Messages::FieldCPtr newField(createField(value));
      fieldOp_->setDictionaryValue(encoder, newField);
    }
  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", *identity_);
      // if reportFatal returns we're being lax about the rules
      // let the copy happen.
      pmap.setNextField(false);
    }
    if(previousIsKnown && previousNotNull)
    {
      // we have to null the previous value to avoid copy
      pmap.setNextField(true);// value in stream
      destination.putByte(nullBlob);
      Messages::FieldCPtr newField = this->createEmptyField();
      fieldOp_->setDictionaryValue(encoder, newField);
    }
    else
    {
      pmap.setNextField(false);
    }
  }
}

void
FieldInstructionBlob::encodeDelta(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & fieldSet) const
{
    // declare a couple of variables...
  bool previousIsKnown = false;
  bool previousNotNull = false;
  std::string previousValue;

  // ... then initialize them from the dictionary
  Messages::FieldCPtr previousField;
  if(fieldOp_->findDictionaryField(encoder, previousField))
  {
    if(!previousField->isString())
    {
      encoder.reportFatal("[ERR D4]", "Previous value type mismatch.", *identity_);
    }
    previousIsKnown = true;
    previousNotNull = previousField->isDefined();
    if(previousNotNull)
    {
      previousValue = previousField->toString();
    }
  }
  if(!previousIsKnown && fieldOp_->hasValue())
  {
    previousIsKnown = true;
    previousValue = initialValue_->toString();
  }

  // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    std::string value = field->toString();
    size_t prefix = longestMatchingPrefix(previousValue, value);
    size_t suffix = longestMatchingSuffix(previousValue, value);
    int32 deltaCount = QuickFAST::uint32(previousValue.length() - prefix);
    std::string deltaValue = value.substr(prefix);
    if(prefix < suffix)
    {
      deltaCount = -int32(previousValue.length() - suffix);
      deltaCount -= 1; // allow +/- 0 values;
      deltaValue = value.substr(0, value.length() - suffix);
    }
    if(!isMandatory() && deltaCount >= 0)
    {
      deltaCount += 1;
    }
    encodeSignedInteger(destination, encoder.getWorkingBuffer(), deltaCount);
    encodeBlob(destination, encoder.getWorkingBuffer(), deltaValue);

    if(!previousIsKnown  || value != previousValue)
    {
      field = createField(value);
      fieldOp_->setDictionaryValue(encoder, field);
    }

  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", *identity_);
    }
    destination.putByte(nullBlob);
  }
}

void
FieldInstructionBlob::encodeTail(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & fieldSet) const
{
    // declare a couple of variables...
  bool previousIsKnown = false;
  bool previousNotNull = true;
  std::string previousValue;

  // ... then initialize them from the dictionary
  Messages::FieldCPtr previousField;
  if(fieldOp_->findDictionaryField(encoder, previousField))
  {
    if(!previousField->isString())
    {
      encoder.reportFatal("[ERR D4]", "Previous value type mismatch.", *identity_);
    }
    previousIsKnown = true;
    previousNotNull = previousField->isDefined();
    if(previousNotNull)
    {
      previousValue = previousField->toString();
    }
  }
  if(!previousIsKnown && fieldOp_->hasValue())
  {
    previousIsKnown = true;
    previousValue = initialValue_->toString();
  }

  // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    std::string value = field->toString();
    size_t prefix = longestMatchingPrefix(previousValue, value);
    std::string tailValue = value.substr(prefix);
    if(tailValue.empty())
    {
      pmap.setNextField(false);
    }
    else
    {
      pmap.setNextField(true);
      if(!isMandatory())
      {
        encodeNullableBlob(destination, encoder, encoder.getWorkingBuffer(), tailValue);
      }
      else
      {
        encodeBlob(destination, encoder.getWorkingBuffer(), tailValue);
      }
    }
    if(!previousIsKnown  || value != previousValue)
    {
      field = createField(value);
      fieldOp_->setDictionaryValue(encoder, field);
    }
  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", *identity_);
    }
    destination.putByte(nullBlob);
  }
}

void
FieldInstructionBlob::interpretValue(const std::string & value)
{
  initialValue_ = createField(value);
}
