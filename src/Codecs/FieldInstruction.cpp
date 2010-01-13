// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstruction.h"
#include <Codecs/DataSource.h>
#include <Codecs/DataDestination.h>
#include <Codecs/FieldOpNop.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstruction::FieldInstruction(
      const std::string & name, const std::string & fieldNamespace)
  : mutableIdentity_(new Messages::FieldIdentity(name, fieldNamespace))
  , identity_(mutableIdentity_)
  , fieldOp_(new FieldOpNop)
  , presenceMapBitsUsed_(0)
{
}

FieldInstruction::FieldInstruction()
  : mutableIdentity_(new Messages::FieldIdentity)
  , identity_(mutableIdentity_)
  , fieldOp_(new FieldOpNop)
  , presenceMapBitsUsed_(0)
{
}

FieldInstruction::~FieldInstruction()
{
}

void
FieldInstruction::finalize(Codecs::TemplateRegistry & /*registry*/)
{
  presenceMapBitsUsed_ = 0;
  if(fieldOp_->usesPresenceMap(isMandatory()))
  {
    presenceMapBitsUsed_ = 1;
  }
}

void
FieldInstruction::setPresence(bool mandatory)
{
  mutableIdentity_->setMandatory(mandatory);
}

void
FieldInstruction::setFieldOp(FieldOpPtr fieldOp)
{
  fieldOp_ = fieldOp;
  if(fieldOp->hasValue())
  {
    interpretValue(fieldOp->getValue());
  }
}

bool
FieldInstruction::getFieldOp(FieldOpCPtr & fieldOp) const
{
  if (!boost::dynamic_pointer_cast<const FieldOpNop>(fieldOp_))
  {
    fieldOp = fieldOp_;
    return true;
  }
  return false;
}

bool
FieldInstruction::decodeConstant(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR S2]", "Const Field Operator not supported for this data type.", *identity_);
  return false;
}

bool
FieldInstruction::decodeDefault(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR S2]", "Default Field Operator not supported for this data type.", *identity_);
  return false;
}

bool
FieldInstruction::decodeCopy(
    Codecs::DataSource & /*source*/,
    Codecs::PresenceMap & /*pmap*/,
    Codecs::Decoder & decoder,
    Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR S2]", "Copy Field Operator not supported for this data type.", *identity_);
  return false;
}

bool
FieldInstruction::decodeCopy(
  Codecs::DataSource & /*source*/,
  bool /*pmapValue*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR U93]", "Copy with specific presence map bit not supported for this data type.", *identity_);
  return false;
}


bool
FieldInstruction::decodeDelta(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR S2]", "Delta Field Operator not supported for this data type.", *identity_);
  return false;
}

bool
FieldInstruction::decodeIncrement(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR S2]", "Increment Field Operator not supported for this data type.", *identity_);
  return false;
}

bool
FieldInstruction::decodeIncrement(
  Codecs::DataSource & /*source*/,
  bool /*pmapValue*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR U93]", "Increment with specific presence map bit not supported for this data type.", *identity_);
  return false;
}

bool
FieldInstruction::decodeTail(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR S2]", "Tail Field Operator not supported for this data type.", *identity_);
  return false;
}

void
FieldInstruction::encodeConstant(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & /*fieldSet*/) const
{
  encoder.reportFatal("[ERR S2]", "Constant Field Operator not supported for this data type.", *identity_);
}


void
FieldInstruction::encodeDefault(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & /*fieldSet*/) const
{
  encoder.reportFatal("[ERR S2]", "Default Field Operator not supported for this data type.", *identity_);
}


void
FieldInstruction::encodeCopy(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & /*fieldSet*/) const
{
  encoder.reportFatal("[ERR S2]", "Copy Field Operator not supported for this data type.", *identity_);
}

void
FieldInstruction::encodeDelta(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & /*fieldSet*/) const
{
  encoder.reportFatal("[ERR S2]", "Delta Field Operator not supported for this data type.", *identity_);
}

void
FieldInstruction::encodeIncrement(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & /*fieldSet*/) const
{
  encoder.reportFatal("[ERR S2]", "Increment Field Operator not supported for this data type.", *identity_);
}

void
FieldInstruction::encodeTail(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & /*fieldSet*/) const
{
  encoder.reportFatal("[ERR S2]", "Tail Field Operator not supported for this data type.", *identity_);
}

bool
FieldInstruction::decodeAscii(
  Codecs::DataSource & source,
  WorkingBuffer & workingBuffer)
{
  workingBuffer.clear(false);
  uchar byte;
  if(!source.getByte(byte))
  {
    return false;
  }
  while((byte & stopBit) == 0)
  {
    workingBuffer.push(byte);
    if(!source.getByte(byte))
    {
      // todo: exception?
      return false;
    }
  }
  workingBuffer.push(byte & dataBits);
  return true;
}

bool
FieldInstruction::checkEmptyAscii(WorkingBuffer & workingBuffer)
{
  bool empty = false;
  // check for possible zeroPreamble on mandatory string
  if(workingBuffer.size() > 0 && workingBuffer[0] == 0)
  {
    workingBuffer.pop_front();
    empty = workingBuffer.size() == 0;
    // todo: if !empty check buffer[0] == 0
    // to detect overlong encoding [ERR R9]
  }
  return empty;
}

bool
FieldInstruction::checkNullAscii(WorkingBuffer & workingBuffer)
{
  // check for possible zeroPreamble on mandatory string
  if(workingBuffer.size() > 0 && workingBuffer[0] == 0)
  {
    workingBuffer.pop_front();
    // todo: check size == 0 || buffer[0] != 0
    // to detect overlong encoding [ERR R9]
  }
  return workingBuffer.size() == 0;
}


void
FieldInstruction::decodeByteVector(
  Codecs::Context & decoder,
  Codecs::DataSource & source,
  const std::string & name,
  WorkingBuffer & buffer,
  size_t length)
{
  buffer.clear(false, length);
  for(size_t pos = 0;
    pos < length;
    ++pos)
  {
    uchar byte = 0;
    if(!source.getByte(byte))
    {
      decoder.reportFatal("[ERR U03]", "End of file: Too few bytes in ByteVector.", name);
    }
    buffer.push(byte);
  }
}

void
FieldInstruction::indexDictionaries(
  DictionaryIndexer & indexer,
  const std::string & dictionaryName,
  const std::string & typeName,
  const std::string & typeNamespace)
{
  fieldOp_->indexDictionaries(
    indexer,
    dictionaryName,
   typeName,
    typeNamespace,
    identity_->getLocalName(),
    identity_->getNamespace());
}

void
FieldInstruction::setDefaultValueNop()
{
}

void
FieldInstruction::setDefaultValueConstant()
{
}

void
FieldInstruction::setDefaultValueDefault()
{
}

void
FieldInstruction::setDefaultValueCopy()
{
}

void
FieldInstruction::setDefaultValueDelta()
{
}

void
FieldInstruction::setDefaultValueIncrement()
{
}

void
FieldInstruction::setDefaultValueTail()
{
}

void
FieldInstruction::encodeSignedInteger(DataDestination & destination, WorkingBuffer & buffer, int64 value)
{
  buffer.clear(true);
  int64 until = 0LL;
  uchar sign = 0;
  if(value < 0)
  {
    until = -1LL;
    sign = signBit;
  }

  // force at least one byte to be stored
  uchar prevByte = ~sign;
  uchar byte = stopBit;
  while(value != until || (prevByte & signBit) != sign)
  {
    byte |= static_cast<uchar>(value & dataBits);
    value >>= dataShift;
    buffer.push(byte);
    prevByte = byte;
    byte = 0;
  }
  for(const uchar * it = buffer.begin();
    it != buffer.end();
    ++it)
  {
    destination.putByte(*it);
  }
}

void
FieldInstruction::encodeNullableAscii(DataDestination & destination, const std::string & value)
{
  if(value.empty() || value[0] == '\0')
  {
    destination.putByte(nullableStringPreamble);
  }
  encodeAscii(destination, value);
}

void
FieldInstruction::encodeAscii(DataDestination & destination, const std::string & value)
{
  if(value.empty())
  {
    destination.putByte(emptyString);
  }
  else
  {
    if (value[0] == '\0')
    {
      destination.putByte(leadingZeroBytePreamble);
    }
    for(size_t pos = 0; pos + 1 < value.size(); ++pos)
    {
      destination.putByte(static_cast<uchar>(value[pos]));
    }
    destination.putByte(value[value.size() - 1] | stopBit);
  }
}


void
FieldInstruction::encodeBlobData(DataDestination & destination, const std::string & value)
{
  for(size_t pos = 0; pos < value.size(); ++pos)
  {
    destination.putByte(static_cast<uchar>(value[pos]));
  }
}

size_t
FieldInstruction::longestMatchingPrefix(
  const std::string & previous,
  const std::string & value)
{
  size_t plen = previous.length();
  size_t vlen = value.length();
  size_t len = std::min(plen, vlen);
  size_t result = 0;
  while(result < len && previous[result] == value[result])
  {
    ++result;
  }
  return result;
}

size_t
FieldInstruction::longestMatchingSuffix(
  const std::string & previous,
  const std::string & value)
{
  size_t ppos = previous.length();
  size_t vpos = value.length();
  size_t result = 0;
  while(ppos > 0 && vpos > 0 && previous[ppos-1] == value[vpos - 1])
  {
    ++result;
    --ppos;
    --vpos;
  }
  return result;
}



void
FieldInstruction::encodeUnsignedInteger(DataDestination & destination, WorkingBuffer & buffer, uint64 value)
{
  buffer.clear(true);
  uint64 until = 0ULL;
  uchar byte = stopBit;
  while(value != until || byte != 0)
  {
    byte |= static_cast<uchar>(value & dataBits);
    value >>= dataShift;
    buffer.push(byte);
    byte = 0;
  }
  for(const uchar * it = buffer.begin();
    it != buffer.end();
    ++it)
  {
    destination.putByte(*it);
  }
}

void
FieldInstruction::setDictionaryName(const std::string & /*name*/)
{
  throw TemplateDefinitionError("dictionary= attribute not allowed for this field instruction.");
}

void
FieldInstruction::setSegmentBody(Codecs::SegmentBodyPtr segment)
{
  throw TemplateDefinitionError("Group or Segment definition error.");
}

size_t
FieldInstruction::fieldCount(const SegmentBody & /*parent*/)const
{
  return 1;
}
