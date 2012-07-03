// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionBlob.h"
#include <Codecs/DataSource.h>
#include <Codecs/DataDestination.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>
#include <Messages/ValueMessageBuilder.h>
#include <Messages/Field.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionBlob::FieldInstructionBlob(
  ValueType::Type type,
  const std::string & name,
  const std::string & fieldNamespace)
  : FieldInstruction(name, fieldNamespace)
  , type_(type)
{
}

FieldInstructionBlob::FieldInstructionBlob()
{
}

FieldInstructionBlob::~FieldInstructionBlob()
{
}

bool
FieldInstructionBlob::decodeBlobFromSource(
  Codecs::DataSource & source,
  Codecs::Context & context,
  bool mandatory,
  WorkingBuffer & buffer) const
{
  PROFILE_POINT("blob::decodeBlobFromSource");
  uint32 length;
  decodeUnsignedInteger(source, context, length, identity_.name());
  if(!mandatory)
  {
    if(checkNullInteger(length))
    {
      // optional and missing.  we're done
      return false;
    }
  }
  decodeByteVector(context, source, identity_.name(), buffer, length);
  return true;
}

void
FieldInstructionBlob::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & builder) const
{
  PROFILE_POINT("blob::decodeNop");
  // note NOP never uses pmap.  It uses a null value instead for optional fields
  // so it's always safe to do the basic decode.
  WorkingBuffer& buffer = decoder.getWorkingBuffer();
  if(decodeBlobFromSource(source, decoder, isMandatory(), buffer))
  {
    const uchar * value = buffer.begin();
    size_t valueSize = buffer.size();
    builder.addValue(identity_, type_, value, valueSize);
  }
}

void
FieldInstructionBlob::decodeConstant(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & /*decoder*/,
  Messages::ValueMessageBuilder & builder) const
{
  PROFILE_POINT("blob::decodeConstant");
  if(isMandatory())
  {
    const std::string & value = fieldOp_->getValue();
    builder.addValue(
      identity_,
      type_,
      reinterpret_cast<const uchar *>(value.c_str()), value.size());
  }
  else
  {
    if(pmap.checkNextField())
    {
      const std::string & value = fieldOp_->getValue();
      builder.addValue(
        identity_,
        type_,
        reinterpret_cast<const uchar *>(value.c_str()), value.size());
    }
    else
    {
      // not present. Nothing to do
    }
  }
}

void
FieldInstructionBlob::decodeDefault(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & builder) const
{
  PROFILE_POINT("blob::decodeDefault");
  if(pmap.checkNextField())
  {
    WorkingBuffer& buffer = decoder.getWorkingBuffer();
    if(decodeBlobFromSource(source, decoder, isMandatory(), buffer))
    {
      const uchar * value = buffer.begin();
      size_t valueSize = buffer.size();
      builder.addValue(
        identity_,
        type_,
        value,
        valueSize);
    }
  }
  else // pmap says nothing in stream
  {
    if(fieldOp_->hasValue())
    {
      const std::string & value = fieldOp_->getValue();
      builder.addValue(
        identity_,
        type_,
        reinterpret_cast<const uchar *>(value.c_str()), value.size());
    }
    else if(isMandatory())
    {
      decoder.reportFatal("[ERR D5]", "Mandatory default operator with no value.", identity_);
    }

  }
}

void
FieldInstructionBlob::decodeCopy(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & builder) const
{
  PROFILE_POINT("blob::decodeCopy");
  if(pmap.checkNextField())
  {
    // field is in the stream, use it
  WorkingBuffer& buffer = decoder.getWorkingBuffer();
  if(decodeBlobFromSource(source, decoder, isMandatory(), buffer))
  {
    const uchar * value = buffer.begin();
    size_t valueSize = buffer.size();
      builder.addValue(
        identity_,
        type_,
        value,
        valueSize);
      fieldOp_->setDictionaryValue(decoder, value, valueSize);
    }
  }
  else // pmap says not in stream
  {
    const uchar * value = 0;
    size_t valueSize = 0;
    Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(decoder, value, valueSize);
    if(previousStatus == Context::OK_VALUE)
    {
       builder.addValue(identity_, type_, value, valueSize);
    }
    else if(fieldOp_->hasValue())
    {
      const std::string & initialValue = fieldOp_->getValue();
      builder.addValue(
        identity_,
        type_,
        reinterpret_cast<const uchar *>(initialValue.c_str()),
        initialValue.size());
      fieldOp_->setDictionaryValue(
        decoder,
        reinterpret_cast<const uchar *>(initialValue.c_str()),
        initialValue.size());
    }
    else
    {
      if(isMandatory())
      {
        decoder.reportFatal("[ERR D6]", "No value available for mandatory copy field.", identity_);
      }
    }
  }
}

void
FieldInstructionBlob::decodeDelta(
  Codecs::DataSource & source,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & builder) const
{
  PROFILE_POINT("blob::decodeDelta");
  int32 deltaLength;
  decodeSignedInteger(source, decoder, deltaLength, identity_.name());
  if(!isMandatory())
  {
    if(checkNullInteger(deltaLength))
    {
      // NULL delta does not clear previous
      // so there's nothing to do
      return;
    }
  }

  std::string deltaValue;
  WorkingBuffer& buffer = decoder.getWorkingBuffer();
  if(decodeBlobFromSource(source, decoder, true /*isMandatory()*/, buffer))
  {
    const uchar * value = buffer.begin();
    size_t valueSize = buffer.size();
    deltaValue = std::string(reinterpret_cast<const char *>(value), valueSize);
  }

  std::string previousValue;
  Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(decoder, previousValue);
  if(previousStatus == Context::UNDEFINED_VALUE)
  {
    if(fieldOp_->hasValue())
    {
      previousValue = fieldOp_->getValue();
    }
  }
  size_t previousLength = previousValue.size();

  if( deltaLength < 0)
  {
    // operate on front of string
    // compensete for the excess -1 encoding that allows -0 != +0
    deltaLength = -(deltaLength + 1);
    // don't chop more than is there
    if(static_cast<unsigned long>(deltaLength) > previousLength)
    {
      decoder.reportError("[ERR D7]", "String tail delta front length exceeds length of previous string.", identity_);
      deltaLength = QuickFAST::int32(previousLength);
    }
    std::string value = deltaValue + previousValue.substr(deltaLength);
    builder.addValue(
      identity_,
      type_,
      reinterpret_cast<const uchar *>(value.c_str()),
      value.size());
    fieldOp_->setDictionaryValue(decoder, value);
  }
  else
  { // operate on end of string
    // don't chop more than is there
    if(static_cast<size_t>(deltaLength) > previousLength)
    {
#if 0 // handy when debugging
      std::cout << "decode blob delta length: " << deltaLength << " previous: " << previousLength << std::endl;
#endif
      decoder.reportError("[ERR D7]", "String tail delta back length exceeds length of previous string.", identity_);
      deltaLength = QuickFAST::uint32(previousLength);
    }

    std::string value = previousValue.substr(0, previousLength - deltaLength) + deltaValue;
    builder.addValue(
      identity_,
      type_,
      reinterpret_cast<const uchar *>(value.c_str()),
      value.size());
    fieldOp_->setDictionaryValue(decoder, value);
  }
}

void
FieldInstructionBlob::decodeTail(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & builder) const
{
  PROFILE_POINT("blob::decodeTail");
  if(pmap.checkNextField())
  {
    // field is in the stream, use it
    WorkingBuffer& buffer = decoder.getWorkingBuffer();
    if(decodeBlobFromSource(source, decoder, isMandatory(), buffer))
    {
      size_t tailLength = buffer.size();
      std::string tailValue(reinterpret_cast<const char *>(buffer.begin()), tailLength);

      std::string previousValue;
      Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(decoder, previousValue);
      if(previousStatus == Context::UNDEFINED_VALUE)
      {
        if(fieldOp_->hasValue())
        {
          previousValue = fieldOp_->getValue();
          fieldOp_->setDictionaryValue(decoder, previousValue);
        }
      }
      size_t previousLength = previousValue.length();
      if(tailLength > previousLength)
      {
        tailLength = previousLength;
      }
      std::string value(previousValue.substr(0, previousLength - tailLength) + tailValue);
      builder.addValue(
        identity_,
        type_,
        reinterpret_cast<const uchar *>(value.c_str()),
        value.size());
      fieldOp_->setDictionaryValue(decoder, value);
    }
    else // null
    {
      fieldOp_->setDictionaryValueNull(decoder);
    }
  }
  else // pmap says not in stream
  {
    std::string previousValue;
    Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(decoder, previousValue);
    if(previousStatus == Context::OK_VALUE)
    {

      builder.addValue(
        identity_,
        type_,
        reinterpret_cast<const uchar *>(previousValue.c_str()),
        previousValue.size());
    }
    else if(fieldOp_->hasValue())
    {
      builder.addValue(identity_, type_, reinterpret_cast<const uchar *>(fieldOp_->getValue().c_str()), fieldOp_->getValue().size());
      fieldOp_->setDictionaryValue(decoder, fieldOp_->getValue());
    }
    else
    {
      if(isMandatory())
      {
        decoder.reportFatal("[ERR D6]", "No value available for mandatory copy field.", identity_);
      }
    }
  }
}

void
FieldInstructionBlob::encodeNullableBlob(
  Codecs::DataDestination & destination,
  Codecs::Context & /*context*/,
  WorkingBuffer & buffer,
  const StringBuffer & value) const
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
  const StringBuffer & value) const
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
  const Messages::MessageAccessor & accessor) const
{
    // get the value from the application data
  const StringBuffer * value;
  if(accessor.getString(identity_, type_, value))
  {
    if(!isMandatory())
    {
      encodeNullableBlob(destination, encoder, encoder.getWorkingBuffer(), *value);
    }
    else
    {
      encodeBlob(destination, encoder.getWorkingBuffer(), *value);
    }
  }
  else // not defined in accessor
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", identity_);
    }
    destination.putByte(nullBlob);
  }
}

void
FieldInstructionBlob::encodeConstant(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  if(!isMandatory())
  {
    // get the value from the application data
    const StringBuffer * value;
    if(accessor.getString(identity_, type_, value))
    {
      const std::string & constant = initialValue_->toString();
      if(*value != constant)
      {
        encoder.reportFatal("[ERR U10]", "Constant value does not match application data.", identity_);
      }
      pmap.setNextField(true);
    }
    else // not defined in accessor
    {
      pmap.setNextField(false);
    }
  }
}


void
FieldInstructionBlob::encodeDefault(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  const StringBuffer * value;
  if(accessor.getString(identity_, type_, value))
  {
    if(fieldOp_->hasValue() &&
      *value == initialValue_->toString())
    {
      pmap.setNextField(false); // not in stream. use default
    }
    else
    {
      pmap.setNextField(true); // != default.  Send value
      if(!isMandatory())
      {
        encodeNullableBlob(destination, encoder, encoder.getWorkingBuffer(), *value);
      }
      else
      {
        encodeBlob(destination,  encoder.getWorkingBuffer(), *value);
      }
    }
  }
  else // not defined in accessor
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", identity_);
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
  const Messages::MessageAccessor & accessor) const
{
  // Retrieve information from the dictionary
  std::string previousValue;
  Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(encoder, previousValue);
  if(previousStatus == Context::UNDEFINED_VALUE)
  {
    if(fieldOp_->hasValue())
    {
      previousValue = fieldOp_->getValue();
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

  const StringBuffer * value;
  if(accessor.getString(identity_, type_, value))
  {
    if(previousStatus == Context::OK_VALUE && *value == previousValue)
    {
      pmap.setNextField(false); // not in stream, use copy
    }
    else
    {
      pmap.setNextField(true);// value in stream
      if(!isMandatory())
      {
        encodeNullableBlob(destination, encoder, encoder.getWorkingBuffer(), *value);
      }
      else
      {
        encodeBlob(destination, encoder.getWorkingBuffer(), *value);
      }
      fieldOp_->setDictionaryValue(encoder, *value);
    }
  }
  else // not defined in accessor
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", identity_);
      // if reportFatal returns we're being lax about the rules
      // let the copy happen.
      pmap.setNextField(false);
    }
    else
    {
      if(previousStatus != Context::NULL_VALUE)
      {
        // we have to null the previous value to avoid copy
        pmap.setNextField(true);// value in stream
        destination.putByte(nullBlob);
      }
      else
      {
        pmap.setNextField(false);
      }
    }
  }
}

void
FieldInstructionBlob::encodeDelta(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  // get information from the dictionary
  std::string previousValue;
  Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(encoder, previousValue);
  if(previousStatus == Context::UNDEFINED_VALUE)
  {
    if(fieldOp_->hasValue())
    {
      previousValue = fieldOp_->getValue();
      fieldOp_->setDictionaryValue(encoder, previousValue);
    }
  }

  const StringBuffer * value;
  if(accessor.getString(identity_, type_, value))
  {
    size_t prefix = longestMatchingPrefix(previousValue, *value);
    size_t suffix = longestMatchingSuffix(previousValue, *value);
    int32 deltaCount = QuickFAST::uint32(previousValue.length() - prefix);
    // Performance: add substr to StringBuffer
    std::string deltaValue = static_cast<std::string>(*value).substr(prefix);
    if(prefix < suffix)
    {
      deltaCount = -int32(previousValue.length() - suffix);
      deltaCount -= 1; // allow +/- 0 values;
      deltaValue = static_cast<std::string>(*value).substr(0, value->length() - suffix);
    }
    if(!isMandatory() && deltaCount >= 0)
    {
      deltaCount += 1;
    }
#if 0 // handy when debugging
    std::cout << "Encode blob delta prefix: " << prefix  << " suffix: " << suffix << std::endl;
    std::cout << "Previous["<<previousValue <<"]" << std::endl;
    std::cout << "   value[" << value <<      "]" << std::endl;
    std::cout << "count:" << deltaCount << " Delta["<< deltaValue << ']' << std::endl;
#endif
    encodeSignedInteger(destination, encoder.getWorkingBuffer(), deltaCount);
    encodeBlob(destination, encoder.getWorkingBuffer(), deltaValue);

    if(previousStatus != Context::OK_VALUE  || *value != previousValue)
    {
      fieldOp_->setDictionaryValue(encoder, *value);
    }

  }
  else // not defined in accessor
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", identity_);
    }
    destination.putByte(nullBlob);
  }
}

void
FieldInstructionBlob::encodeTail(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  // get information from the dictionary
  std::string previousValue;
  Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(encoder, previousValue);
  if(previousStatus == Context::UNDEFINED_VALUE)
  {
    if(fieldOp_->hasValue())
    {
      previousValue = fieldOp_->getValue();
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

  const StringBuffer * value;
  if(accessor.getString(identity_, type_, value))
  {
    size_t prefix = longestMatchingPrefix(previousValue, *value);
    // performance: add substr method to StringBuffer
    std::string tailValue = static_cast<std::string>(*value).substr(prefix);
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
    if(previousStatus != Context::OK_VALUE  || *value != previousValue)
    {
      fieldOp_->setDictionaryValue(encoder, *value);
    }
  }
  else // not defined in accessor
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", identity_);
    }
    if(previousStatus != Context::NULL_VALUE)
    {
      pmap.setNextField(true);
      destination.putByte(nullAscii);
      // note: tail operator does not null dictionary value here
    }
    else
    {
      pmap.setNextField(false);
    }
  }
}

void
FieldInstructionBlob::interpretValue(const std::string & value)
{
  initialValue_ = createField(value);
}

ValueType::Type
FieldInstructionBlob::fieldInstructionType()const
{
  return type_;
}

void
FieldInstructionBlob::addLengthInstruction(FieldInstructionPtr & field)
{
  // length instruction is legal, but has no impact on the encoding.
  // Ignore it for now.
}
