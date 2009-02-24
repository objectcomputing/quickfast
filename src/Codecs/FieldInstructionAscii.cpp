// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionAscii.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/DataDestination.h>
#include <Codecs/Encoder.h>
#include <Messages/Message.h>
#include <Messages/FieldAscii.h>

#include <Common/Profiler.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionAscii::FieldInstructionAscii(
  const std::string & name,
  const std::string & fieldNamespace)
: FieldInstruction(name, fieldNamespace)
{
  initialValue_ = Messages::FieldAscii::createNull();
}

FieldInstructionAscii::FieldInstructionAscii()
{
  initialValue_ = Messages::FieldAscii::createNull();
}

FieldInstructionAscii::~FieldInstructionAscii()
{
}

bool
FieldInstructionAscii::decodeFromSource(
  Codecs::DataSource & source,
  bool mandatory,
  WorkingBuffer & buffer,
  Messages::FieldCPtr & field) const
{
  PROFILE_POINT("ascii::decodeFromSource");
  if(!decodeAscii(source, buffer))
  {
    return false;
  }
  if(!mandatory)
  {
    if(checkNullAscii(buffer))
    {
      return true;
    }
  }
  if(checkEmptyAscii(buffer))
  {
    field = Messages::FieldAscii::create("");
    return true;
  }
  field = Messages::FieldAscii::create(buffer.begin(), buffer.size());
  return true;
}


bool
FieldInstructionAscii::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::FieldSet & fieldSet) const
{
  PROFILE_POINT("ascii::decodeNop");
  // note NOP never uses pmap.  It uses a null value instead for optional fields
  // so it's always safe to do the basic decode.
  Messages::FieldCPtr field;
  if(!decodeFromSource(source, isMandatory(), decoder.getWorkingBuffer(), field))
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
FieldInstructionAscii::decodeConstant(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::FieldSet & fieldSet) const
{
  PROFILE_POINT("ascii::decodeConstant");
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
FieldInstructionAscii::decodeDefault(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::FieldSet & fieldSet) const
{
  PROFILE_POINT("ascii::decodeDefault");
  if(pmap.checkNextField())
  {
    Messages::FieldCPtr field;
    if(!decodeFromSource(source, isMandatory(), decoder.getWorkingBuffer(), field))
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
      decoder.reportFatal("[ERR D5]", "Mandatory default operator with no value.");
    }
  }
  return true;
}

bool
FieldInstructionAscii::decodeCopy(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::FieldSet & fieldSet) const
{
  PROFILE_POINT("ascii::decodeCopy");

  if(pmap.checkNextField())
  {
    // field is in the stream, use it
    Messages::FieldCPtr field;
    if(!decodeFromSource(source, isMandatory(),decoder.getWorkingBuffer(), field))
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
        decoder.reportFatal("[ERR D6]", "No value available for mandatory copy field.");
      }
    }
  }
  return true;
}

bool
FieldInstructionAscii::decodeDelta(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::FieldSet & fieldSet) const
{
  PROFILE_POINT("ascii::decodeDelta");
  int32 deltaLength;
  decodeSignedInteger(source, decoder, deltaLength);
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
  if(!decodeFromSource(source, true, decoder.getWorkingBuffer(), deltaField))
  {
    return false;
  }
  const std::string & deltaValue = deltaField->toString();

  Messages::FieldCPtr previousField;
  if(!fieldOp_->findDictionaryField(decoder, previousField))
  {
    previousField = initialValue_;
  }
  const std::string previousValue = previousField->toString();
  size_t previousLength = previousValue.length();

  if( deltaLength < 0)
  {
    // operate on front of string
    // compensete for the excess -11 encoding that allows -0 != +0
    deltaLength = -(deltaLength + 1);
    // don't chop more than is there
    if(static_cast<uint32>(deltaLength) > previousLength)
    {
      decoder.reportError("[ERR D7]", "String head delta length exceeds length of previous string.");
      deltaLength = previousLength;
    }
    Messages::FieldCPtr field = Messages::FieldAscii::create(
      deltaValue + previousValue.substr(deltaLength));
    fieldSet.addField(
      identity_,
      field);
    fieldOp_->setDictionaryValue(decoder, field);
  }
  else
  { // operate on end of string
    // don't chop more than is there
    if(static_cast<uint32>(deltaLength) > previousLength)
    {
      decoder.reportError("[ERR D7]", "String tail delta length exceeds length of previous string.");
      deltaLength = previousLength;
    }
    Messages::FieldCPtr field = Messages::FieldAscii::create(
      previousValue.substr(0, previousLength - deltaLength) + deltaValue);
    fieldSet.addField(
      identity_,
      field);
    fieldOp_->setDictionaryValue(decoder, field);
  }
  return true;
}

bool
FieldInstructionAscii::decodeTail(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::FieldSet & fieldSet) const
{
  PROFILE_POINT("ascii::decodeTail");
  if(pmap.checkNextField())
  {
    // field is in the stream, use it
    Messages::FieldCPtr tailField;
    if(!decodeFromSource(source, isMandatory(), decoder.getWorkingBuffer(), tailField))
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
      const std::string previousValue = previousField->toString();

      size_t tailLength = tailValue.length();
      size_t previousLength = previousValue.length();
      if(tailLength > previousLength)
      {
        tailLength = previousLength;
      }
      Messages::FieldCPtr field(Messages::FieldAscii::create(
        previousValue.substr(0, previousLength - tailLength) + tailValue));
      fieldSet.addField(
        identity_,
        field);
      fieldOp_->setDictionaryValue(decoder, field);
    }
    else // null
    {
      fieldOp_->setDictionaryValue(decoder,  Messages::FieldAscii::createNull());
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
        decoder.reportFatal("[ERR D6]", "No value available for mandatory copy field.");
      }
    }
  }
  return true;
}

void
FieldInstructionAscii::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::FieldSet & fieldSet) const
{
  // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    std::string value = field->toAscii();
    if(!isMandatory())
    {
      encodeNullableAscii(destination, value);
    }
    else
    {
      encodeAscii(destination, value);
    }
  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U9}", "Missing mandatory field.");
    }
    destination.putByte(nullAscii);
  }
}

void
FieldInstructionAscii::encodeConstant(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::FieldSet & fieldSet) const
{
  // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    const std::string & value = field->toAscii();
    const std::string & constant = initialValue_->toAscii();
    if(value != constant)
    {
      encoder.reportFatal("[ERR U10}", "Constant value does not match application data.");
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
      encoder.reportFatal("[ERR U9}", "Missing mandatory field.");
    }
    pmap.setNextField(false);
  }
}


void
FieldInstructionAscii::encodeDefault(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::FieldSet & fieldSet) const
{
  // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    std::string value = field->toAscii();
    const std::string & defaultValue = initialValue_->toAscii();
    if(value != defaultValue)
    {
      pmap.setNextField(false); // not in stream. use default
    }
    else
    {
      pmap.setNextField(true); // != default.  Send value
      if(!isMandatory())
      {
        encodeNullableAscii(destination, value);
      }
      else
      {
        encodeAscii(destination, value);
      }
    }
  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U9}", "Missing mandatory field.");
    }
    if(fieldOp_->hasValue())
    {
      pmap.setNextField(true);
      destination.putByte(nullAscii);
    }
    else
    {
      pmap.setNextField(false);
    }
  }
}


void
FieldInstructionAscii::encodeCopy(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::FieldSet & fieldSet) const
{
  // declare a couple of variables...
  bool previousIsKnown = false;
  bool previousNotNull = false;
  std::string previousValue;

  // ... then initialize them from the dictionary
  Messages::FieldCPtr previousField;
  if(fieldOp_->findDictionaryField(encoder, previousField))
  {
    if(!previousField->isType(Messages::Field::ASCII))
    {
      encoder.reportFatal("[ERR D4]", "Previous value type mismatch.");
    }
    previousIsKnown = true;
    previousNotNull = previousField->isDefined();
    if(previousNotNull)
    {
      previousValue = previousField->toAscii();
    }
  }

  // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    std::string value = field->toAscii();
    if(previousIsKnown && previousValue == value)
    {
      pmap.setNextField(false); // not in stream, use copy
    }
    else
    {
      pmap.setNextField(true);// value in stream
      if(!isMandatory())
      {
        encodeNullableAscii(destination, value);
      }
      else
      {
        encodeAscii(destination, value);
      }
      fieldOp_->setDictionaryValue(encoder, Messages::FieldAscii::create(value));
    }
  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U9}", "Missing mandatory field.");
    }
    if((previousIsKnown && previousNotNull)
      || !previousIsKnown)
    {
      // we have to null the previous value to avoid copy
      pmap.setNextField(true);// value in stream
      destination.putByte(nullAscii);
      fieldOp_->setDictionaryValue(encoder, Messages::FieldAscii::createNull());
    }
    else
    {
      pmap.setNextField(false);
    }
  }
}
void
FieldInstructionAscii::encodeDelta(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::FieldSet & fieldSet) const
{
    // declare a couple of variables...
  bool previousIsKnown = false;
  bool previousNotNull = false;
  std::string previousValue;

  // ... then initialize them from the dictionary
  Messages::FieldCPtr previousField;
  if(fieldOp_->findDictionaryField(encoder, previousField))
  {
    if(!previousField->isType(Messages::Field::ASCII))
    {
      encoder.reportFatal("[ERR D4]", "Previous value type mismatch.");
    }
    previousIsKnown = true;
    previousNotNull = previousField->isDefined();
    if(previousNotNull)
    {
      previousValue = previousField->toAscii();
    }
  }

  // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    std::string value = field->toAscii();
    size_t prefix = longestMatchingPrefix(previousValue, value);
    size_t suffix = longestMatchingSuffix(previousValue, value);
    int32 deltaCount = previousValue.length() - prefix;
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
    encodeAscii(destination, deltaValue);

    if(!previousIsKnown  || value != previousValue)
    {
      field = Messages::FieldAscii::create(value);
      fieldOp_->setDictionaryValue(encoder, field);
    }

  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U9}", "Missing mandatory field.");
    }
    destination.putByte(nullAscii);
  }
}

void
FieldInstructionAscii::encodeTail(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::FieldSet & fieldSet) const
{
    // declare a couple of variables...
  bool previousIsKnown = false;
  bool previousNotNull = true;
  std::string previousValue;

  // ... then initialize them from the dictionary
  Messages::FieldCPtr previousField;
  if(fieldOp_->findDictionaryField(encoder, previousField))
  {
    if(!previousField->isType(Messages::Field::ASCII))
    {
      encoder.reportFatal("[ERR D4]", "Previous value type mismatch.");
    }
    previousIsKnown = true;
    previousNotNull = previousField->isDefined();
    if(previousNotNull)
    {
      previousValue = previousField->toAscii();
    }
  }

  // get the value from the application data
  Messages::FieldCPtr field;
  if(fieldSet.getField(identity_->name(), field))
  {
    std::string value = field->toAscii();
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
        encodeNullableAscii(destination, tailValue);
      }
      else
      {
        encodeAscii(destination, tailValue);
      }
    }
    if(!previousIsKnown  || value != previousValue)
    {
      field = Messages::FieldAscii::create(value);
      fieldOp_->setDictionaryValue(encoder, field);
    }
  }
  else // not defined in fieldset
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U9}", "Missing mandatory field.");
    }
    destination.putByte(nullAscii);
  }
}

void
FieldInstructionAscii::interpretValue(const std::string & value)
{
  initialValue_ = Messages::FieldAscii::create(value);
}
