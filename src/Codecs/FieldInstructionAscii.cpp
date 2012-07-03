// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionAscii.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/DataDestination.h>
#include <Codecs/Encoder.h>
#include <Messages/ValueMessageBuilder.h>
#include <Messages/MessageAccessor.h>
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
FieldInstructionAscii::decodeAsciiFromSource(
  Codecs::DataSource & source,
  bool mandatory,
  WorkingBuffer & buffer) const
{
  PROFILE_POINT("ascii::decodeAsciiFromSource");
  decodeAscii(source, buffer);
  if(!mandatory)
  {
    if(checkNullAscii(buffer))
    {
      return false;
    }
  }
  if(checkEmptyAscii(buffer))
  {
    buffer.clear(true);
  }
  return true;
}

void
FieldInstructionAscii::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & builder) const
{
  PROFILE_POINT("ascii::decodeNop");
  // note NOP never uses pmap.  It uses a null value instead for optional fields
  // so it's always safe to do the basic decode.
  WorkingBuffer & buffer = decoder.getWorkingBuffer();
  if(decodeAsciiFromSource(source, isMandatory(), buffer))
  {
    builder.addValue(identity_, ValueType::ASCII, buffer.begin(), buffer.size());
  }
}

void
FieldInstructionAscii::decodeConstant(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & /*decoder*/,
  Messages::ValueMessageBuilder & builder) const
{
  PROFILE_POINT("ascii::decodeConstant");
  if(isMandatory())
  {
    builder.addValue(
      identity_,
      ValueType::ASCII,
      reinterpret_cast<const uchar *>(fieldOp_->getValue().c_str()),
      fieldOp_->getValue().size());
  }
  else
  {
    if(pmap.checkNextField())
    {
      builder.addValue(
        identity_,
        ValueType::ASCII,
        reinterpret_cast<const uchar *>(fieldOp_->getValue().c_str()),
        fieldOp_->getValue().size());
    }
    else
    {
      // not present. Nothing to do
    }
  }
}

void
FieldInstructionAscii::decodeDefault(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & builder) const
{
  PROFILE_POINT("ascii::decodeDefault");
  if(pmap.checkNextField())
  {
    WorkingBuffer & buffer = decoder.getWorkingBuffer();
    if(decodeAsciiFromSource(source, isMandatory(), buffer))
    {
      builder.addValue(
        identity_,
        ValueType::ASCII,
        buffer.begin(),
        buffer.size());
    }
  }
  else // pmap says nothing in stream
  {
    if(fieldOp_->hasValue())
    {
      builder.addValue(
        identity_,
        ValueType::ASCII,
        reinterpret_cast<const uchar *>(fieldOp_->getValue().c_str()),
        fieldOp_->getValue().size()
        );
    }
    else if(isMandatory())
    {
      decoder.reportFatal("[ERR D5]", "Mandatory default operator with no value.", identity_);
    }
  }
}

void
FieldInstructionAscii::decodeCopy(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & builder) const
{
  PROFILE_POINT("ascii::decodeCopy");
  if(pmap.checkNextField())
  {
    // field is in the stream, use it
    WorkingBuffer & buffer = decoder.getWorkingBuffer();
    if(decodeAsciiFromSource(source, isMandatory(), buffer))
    {
      builder.addValue(
        identity_,
        ValueType::ASCII,
        buffer.begin(),
        buffer.size()
        );
      fieldOp_->setDictionaryValue(decoder, buffer.begin(), buffer.size());
    }
    else
    {
      fieldOp_->setDictionaryValueNull(decoder);
    }
  }
  else // pmap says not in stream
  {
    const uchar * value = 0;
    size_t valueSize = 0;
    Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(decoder, value, valueSize);
    if(previousStatus == Context::OK_VALUE)
    {
      builder.addValue(identity_, ValueType::ASCII, value, valueSize);
    }
    else if(previousStatus == Context::UNDEFINED_VALUE && fieldOp_->hasValue())
    {
      builder.addValue(
        identity_,
        ValueType::ASCII,
        reinterpret_cast<const uchar *>(fieldOp_->getValue().c_str()),
        fieldOp_->getValue().size());
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
FieldInstructionAscii::decodeDelta(
  Codecs::DataSource & source,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & builder) const
{
  PROFILE_POINT("ascii::decodeDelta");
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
  WorkingBuffer & buffer = decoder.getWorkingBuffer();
  if(decodeAsciiFromSource(source, true, buffer))
  {
    deltaValue = std::string(
      reinterpret_cast<const char *>(buffer.begin()),
      buffer.size());
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

  size_t previousLength = previousValue.length();
  if( deltaLength < 0)
  {
    // operate on front of string
    // compensete for the excess -1 encoding that allows -0 != +0
    deltaLength = -(deltaLength + 1);
    // don't chop more than is there
    if(static_cast<unsigned long>(deltaLength) > previousLength)
    {
      decoder.reportError("[ERR D7]", "ASCII tail delta front length exceeds length of previous string.", identity_);
      deltaLength = QuickFAST::int32(previousLength);
    }
    std::string value = deltaValue + previousValue.substr(deltaLength);
    builder.addValue(
      identity_,
      ValueType::ASCII,
      reinterpret_cast<const uchar *>(value.c_str()),
      value.size());
    fieldOp_->setDictionaryValue(decoder, value);
  }
  else
  { // operate on end of string
    // don't chop more than is there
    if(static_cast<unsigned long>(deltaLength) > previousLength)
    {
#if 0 // handy when debugging
      std::cout << "decode ascii delta length: " << deltaLength << " previous: " << previousLength << std::endl;
#endif
      decoder.reportError("[ERR D7]", "ASCII tail delta back length exceeds length of previous string.", identity_);
      deltaLength = QuickFAST::uint32(previousLength);
    }
    std::string value = previousValue.substr(0, previousLength - deltaLength) + deltaValue;
    builder.addValue(
      identity_,
      ValueType::ASCII,
      reinterpret_cast<const uchar *>(value.c_str()),
      value.size());
    fieldOp_->setDictionaryValue(decoder, value);
  }
}

void
FieldInstructionAscii::decodeTail(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & builder) const
{
  PROFILE_POINT("ascii::decodeTail");
  if(pmap.checkNextField())
  {
    // field is in the stream, use it
    WorkingBuffer & buffer = decoder.getWorkingBuffer();
    if(decodeAsciiFromSource(source, isMandatory(), buffer))
    {
      const std::string tailValue(reinterpret_cast<const char *>(buffer.begin()), buffer.size());
      size_t tailLength = tailValue.length();
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
        ValueType::ASCII,
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
      Messages::FieldCPtr field = Messages::FieldAscii::create(previousValue);
      builder.addValue(identity_,
        ValueType::ASCII,
        reinterpret_cast<const uchar *>(previousValue.c_str()),
        previousValue.size());
    }
    else if(fieldOp_->hasValue())
    {
      builder.addValue(
        identity_,
        ValueType::ASCII,
        reinterpret_cast<const uchar *>(fieldOp_->getValue().c_str()),
        fieldOp_->getValue().size());
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
FieldInstructionAscii::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  // get the value from the application data
  const StringBuffer * value;
  if(accessor.getString(identity_, ValueType::ASCII, value))
  {
    if(!isMandatory())
    {
      encodeNullableAscii(destination, *value);
    }
    else
    {
      encodeAscii(destination, *value);
    }
  }
  else // not defined in accessor
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", identity_);
    }
    destination.putByte(nullAscii);
  }
}

void
FieldInstructionAscii::encodeConstant(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  if(!isMandatory())
  {
    // get the value from the application data
    const StringBuffer * value;
    if(accessor.getString(identity_, ValueType::ASCII, value))
    {
      const std::string & constant = initialValue_->toAscii();
      if(*value != constant)
      {
        encoder.reportFatal("[ERR U10}", "Constant value does not match application data.", identity_);
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
FieldInstructionAscii::encodeDefault(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  // get the value from the application data
  const StringBuffer * value;
  if(accessor.getString(identity_, ValueType::ASCII, value))
  {
//    std::cout << "EncodeAsciiDefault: in record: \"" << value->c_str() << "\"" << std::endl;
    if(initialValue_->isDefined() &&
      initialValue_->toAscii() == *value)
    {
//      std::cout << "  ... matches default. Set PMAP false." << std::endl;
      pmap.setNextField(false); // not in stream. use default
    }
    else
    {
//      std::cout << "  ... does not match default. Set PMAP true" << std::endl;
      pmap.setNextField(true); // != default.  Send value
      if(!isMandatory())
      {
//        std::cout << "   and send nullable value" << std::endl;
        encodeNullableAscii(destination, *value);
      }
      else
      {
//        std::cout << "   and send normal value" << std::endl;
        encodeAscii(destination, *value);
      }
    }
  }
  else // not defined in accessor
  {
//    std::cout << "EncodeAsciiDefault: NOT in record:" << std::endl;
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", identity_);
    }
    if(fieldOp_->hasValue())
    {
//      std::cout << "          Send a NULL to override default value." << std::endl;
      pmap.setNextField(true);
      destination.putByte(nullAscii);
    }
    else
    {
//      std::cout << "       No default value so clear pmap meaning field does not exist." << std::endl;
      pmap.setNextField(false);
    }
  }
}


void
FieldInstructionAscii::encodeCopy(
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

  // get the value from the application data
  const StringBuffer * value;
  if(accessor.getString(identity_, ValueType::ASCII, value))
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
        encodeNullableAscii(destination, *value);
      }
      else
      {
        encodeAscii(destination, *value);
      }
      fieldOp_->setDictionaryValue(encoder, *value);
    }
  }
  else // not defined by accessor
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory ascii field.", identity_);
      // if reportFatal returns we're being lax about the rules
      // let the copy happen.
      pmap.setNextField(false);
    }
    else
    {
      // Missing optional field.  If we have a previous, non-null value
      // we need to explicitly null it out.  Otherwise just don't send it.
      if(previousStatus != Context::NULL_VALUE)
      {
        pmap.setNextField(true);// value in stream
        destination.putByte(nullAscii);
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
FieldInstructionAscii::encodeDelta(
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
  // get the value from the application data
  const StringBuffer * valueBuffer;
  if(accessor.getString(identity_, ValueType::ASCII, valueBuffer))
  {
    std::string value(*valueBuffer);
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
#if 0 // handy when debugging
    std::cout << "Encode ascii delta prefix: " << prefix  << " suffix: " << suffix << std::endl;
    std::cout << "Previous["<<previousValue <<"]" << std::endl;
    std::cout << "   value[" << value <<      "]" << std::endl;
    std::cout << "count:" << deltaCount << " Delta["<< deltaValue << ']' << std::endl;
#endif
    encodeSignedInteger(destination, encoder.getWorkingBuffer(), deltaCount);
    encodeAscii(destination, deltaValue);

    if(previousStatus != Context::OK_VALUE  || value != previousValue)
    {
      fieldOp_->setDictionaryValue(encoder, value);
    }
  }
  else // not defined in accessor
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory field.", identity_);
    }
    destination.putByte(nullAscii);
  }
}

void
FieldInstructionAscii::encodeTail(
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

  // get the value from the application data
  const StringBuffer * valueBuffer;
  if(accessor.getString(identity_, ValueType::ASCII, valueBuffer))
  {
    std::string value(*valueBuffer);
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
    if(previousStatus != Context::OK_VALUE  || value != previousValue)
    {
      fieldOp_->setDictionaryValue(encoder, value);
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
FieldInstructionAscii::interpretValue(const std::string & value)
{
  initialValue_ = Messages::FieldAscii::create(value);
}

ValueType::Type
FieldInstructionAscii::fieldInstructionType()const
{
  return ValueType::ASCII;
}
