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
  : identity_(name, fieldNamespace)
  , fieldOp_(new FieldOpNop)
  , presenceMapBitsUsed_(0)
  , mandatory_(true)
  , ignoreOverflow_(false)
{
}

FieldInstruction::FieldInstruction()
  : identity_()
  , fieldOp_(new FieldOpNop)
  , presenceMapBitsUsed_(0)
  , mandatory_(true)
  , ignoreOverflow_(false)
{
}

FieldInstruction::~FieldInstruction()
{
}

void
FieldInstruction::finalize(Codecs::TemplateRegistry & /*registry*/)
{
  presenceMapBitsUsed_ = 0;
  /// Note: do not use fieldOp_ directly here.  GetFieldOp may resolve to a "subfield"
  if(getFieldOp()->usesPresenceMap(isMandatory()))
  {
    presenceMapBitsUsed_ = 1;
  }
}

bool
FieldInstruction::isPossiblyRecursive() const
{
  return false;
}

void
FieldInstruction::setPresence(bool mandatory)
{
  mandatory_ = mandatory;
}

void
FieldInstruction::setIgnoreOverflow(bool ignoreOverflow)
{
  ignoreOverflow_ = ignoreOverflow;
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

FieldOpCPtr
FieldInstruction::getFieldOp() const
{
  if (fieldOp_->opType() == FieldOp::NOP)
  {
    // For sequences, use the field op associated with the length.
    SegmentBodyPtr segment;
    if(getSegmentBody(segment))
    {
      FieldInstructionCPtr length;
      if(segment->getLengthInstruction(length))
      {
        return length->getFieldOp();
      }
    }
  }
  return fieldOp_;
}

void
FieldInstruction::decodeConstant(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR S2]", "Const Field Operator not supported for this data type.", identity_);
}

void
FieldInstruction::decodeDefault(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR S2]", "Default Field Operator not supported for this data type.", identity_);
}

void
FieldInstruction::decodeCopy(
    Codecs::DataSource & /*source*/,
    Codecs::PresenceMap & /*pmap*/,
    Codecs::Decoder & decoder,
    Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR S2]", "Copy Field Operator not supported for this data type.", identity_);
}

void
FieldInstruction::decodeCopy(
  Codecs::DataSource & /*source*/,
  bool /*pmapValue*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR U93]", "Copy with specific presence map bit not supported for this data type.", identity_);
}


void
FieldInstruction::decodeDelta(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR S2]", "Delta Field Operator not supported for this data type.", identity_);
}

void
FieldInstruction::decodeIncrement(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR S2]", "Increment Field Operator not supported for this data type.", identity_);
}

void
FieldInstruction::decodeIncrement(
  Codecs::DataSource & /*source*/,
  bool /*pmapValue*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR U93]", "Increment with specific presence map bit not supported for this data type.", identity_);
}

void
FieldInstruction::decodeTail(
  Codecs::DataSource & /*source*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & /*fieldSet*/) const
{
  decoder.reportFatal("[ERR S2]", "Tail Field Operator not supported for this data type.", identity_);
}

void
FieldInstruction::encodeConstant(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & /*fieldSet*/) const
{
  encoder.reportFatal("[ERR S2]", "Constant Field Operator not supported for this data type.", identity_);
}


void
FieldInstruction::encodeDefault(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & /*fieldSet*/) const
{
  encoder.reportFatal("[ERR S2]", "Default Field Operator not supported for this data type.", identity_);
}


void
FieldInstruction::encodeCopy(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & /*fieldSet*/) const
{
  encoder.reportFatal("[ERR S2]", "Copy Field Operator not supported for this data type.", identity_);
}

void
FieldInstruction::encodeDelta(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & /*fieldSet*/) const
{
  encoder.reportFatal("[ERR S2]", "Delta Field Operator not supported for this data type.", identity_);
}

void
FieldInstruction::encodeIncrement(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & /*fieldSet*/) const
{
  encoder.reportFatal("[ERR S2]", "Increment Field Operator not supported for this data type.", identity_);
}

void
FieldInstruction::encodeTail(
  Codecs::DataDestination & /*destination*/,
  Codecs::PresenceMap & /*pmap*/,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & /*fieldSet*/) const
{
  encoder.reportFatal("[ERR S2]", "Tail Field Operator not supported for this data type.", identity_);
}

bool
FieldInstruction::decodeAscii(
  Codecs::DataSource & source,
  WorkingBuffer & workingBuffer)
{
  workingBuffer.clear(false);
  uchar byte = 0;
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
    identity_.getLocalName(),
    identity_.getNamespace());
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

#if 0 // Unrolled version is about 15% faster
namespace
{
  void
  encodeToWorkingBuffer(WorkingBuffer & buffer, int64 value)
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
  }
}

void
FieldInstruction::encodeSignedInteger(DataDestination & destination, WorkingBuffer & buffer, int64 value)
{
  encodeToWorkingBuffer(buffer, value);
  for(const uchar * it = buffer.begin();
    it != buffer.end();
    ++it)
  {
    destination.putByte(*it);
  }
}

#else
void
FieldInstruction::encodeSignedInteger(DataDestination & destination, WorkingBuffer & buffer, int64 value)
{
  if (value >= 0)
  {
    if (value < 0x0000000000000040LL)
    {
      destination.putByte(((value & 0x7F) | 0x80)); // .... .... .... ..7f
    }
    else if (value < 0x0000000000002000LL)
    {
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte(((value & 0x7F) | 0x80)); // .... .... .... ..7f
    }
    else if (value < 0x0000000000100000LL)
    {
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ..1F C...
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte(((value & 0x7F) | 0x80)); // .... .... .... ..7f
    }
    else if (value < 0x0000000008000000LL)
    {
      destination.putByte(((value >> 21)  & 0x7F)); // .... .... .FE. ....
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ..1F C...
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte(((value & 0x7F) | 0x80)); // .... .... .... ..7f
    }
    else if (value < 0x0000000400000000LL)
    {
      destination.putByte(((value >> 28)  & 0x7F)); // .... ...7 F... ....
      destination.putByte(((value >> 21)  & 0x7F)); // .... .... .FE. ....
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ..1F C...
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte(((value & 0x7F) | 0x80)); // .... .... .... ..7f
    }
    else if (value < 0x0000020000000000LL)
    {
      destination.putByte(((value >> 35)  & 0x7F)); // .... .3F8 .... ....
      destination.putByte(((value >> 28)  & 0x7F)); // .... ...7 F... ....
      destination.putByte(((value >> 21)  & 0x7F)); // .... .... .FE. ....
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ..1F C...
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte(((value & 0x7F) | 0x80)); // .... .... .... ..7f
    }
    else if (value < 0x0001000000000000LL)
    {
      destination.putByte(((value >> 42)  & 0x7F)); // ...1 FC.. .... ....
      destination.putByte(((value >> 35)  & 0x7F)); // .... .3F8 .... ....
      destination.putByte(((value >> 28)  & 0x7F)); // .... ...7 F... ....
      destination.putByte(((value >> 21)  & 0x7F)); // .... .... .FE. ....
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ..1F C...
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte(((value & 0x7F) | 0x80)); // .... .... .... ..7f
    }
    else if (value < 0x0080000000000000LL)
    {
      destination.putByte(((value >> 49)  & 0x7F)); // ..FE .... .... ....
      destination.putByte(((value >> 42)  & 0x7F)); // ...1 FC.. .... ....
      destination.putByte(((value >> 35)  & 0x7F)); // .... .3F8 .... ....
      destination.putByte(((value >> 28)  & 0x7F)); // .... ...7 F... ....
      destination.putByte(((value >> 21)  & 0x7F)); // .... .... .FE. ....
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ..1F C...
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte(((value & 0x7F) | 0x80)); // .... .... .... ..7f
    }
    else if (value < 0x4000000000000000LL)
    {
      destination.putByte(((value >> 56)  & 0x7F)); // 7F.. .... .... ....
      destination.putByte(((value >> 49)  & 0x7F)); // ..FE .... .... ....
      destination.putByte(((value >> 42)  & 0x7F)); // ...1 FC.. .... ....
      destination.putByte(((value >> 35)  & 0x7F)); // .... .3F8 .... ....
      destination.putByte(((value >> 28)  & 0x7F)); // .... ...7 F... ....
      destination.putByte(((value >> 21)  & 0x7F)); // .... .... .FE. ....
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ..1F C...
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte(((value & 0x7F) | 0x80)); // .... .... .... ..7f
    }
    else
    {
      destination.putByte(((value >> 63)  & 0x7F)); // 8... .... .... ....  (this will always be zero)
      destination.putByte(((value >> 56)  & 0x7F)); // 7F.. .... .... ....
      destination.putByte(((value >> 49)  & 0x7F)); // ..FE .... .... ....
      destination.putByte(((value >> 42)  & 0x7F)); // ...1 FC.. .... ....
      destination.putByte(((value >> 35)  & 0x7F)); // .... .3F8 .... ....
      destination.putByte(((value >> 28)  & 0x7F)); // .... ...7 F... ....
      destination.putByte(((value >> 21)  & 0x7F)); // .... .... .FE. ....
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ..1F C...
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte(((value & 0x7F) | 0x80)); // .... .... .... ..7f
    }
  }
  else
  {
    // using absolute value avoids tricky word length issues
    int64 absv = -value;
//    if(absv == value) // Apparently this is not a valid check on all compilers
    if((value << 1) == 0)
    {
      // encode the most negative possible number
      destination.putByte(0x7F);    // 8... .... .... ....
      destination.putByte(0x00);    // 7F.. .... .... ....
      destination.putByte(0x00);    // . FE .... .... ....
      destination.putByte(0x00);    // ...1 FC.. .... ....
      destination.putByte(0x00);    // .... .3F8 .... ....
      destination.putByte(0x00);    // .... ...7 F... ....
      destination.putByte(0x00);    // .... .... .FE. ....
      destination.putByte(0x00);    // .... .... ...1 FC..
      destination.putByte(0x00);    // .... .... .... 3F8.
      destination.putByte(0x80);    // .... .... .... ..7f
    }
    else if (absv <= 0x0000000000000040LL)
    {
      destination.putByte(value & 0xFF); // .... .... .... ..7f
    }
    else if (absv <= 0x0000000000002000LL)
    {
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte((value & 0x7F)  | 0x80);  // .... .... .... ..7f
    }
    else if (absv <= 0x0000000000100000LL)
    {
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ...1 FC..
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte((value & 0x7F)  | 0x80);  // .... .... .... ..7f
    }
    else if (absv <= 0x0000000008000000LL)
    {
      destination.putByte(((value >> 21)  & 0x7F)); // .... .... .FE. ....
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ...1 FC..
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte((value & 0x7F)  | 0x80);  // .... .... .... ..7f
    }
    else if (absv <= 0x0000000400000000LL)
    {
      destination.putByte(((value >> 28)  & 0x7F)); // .... ...7 F... ....
      destination.putByte(((value >> 21)  & 0x7F)); // .... .... .FE. ....
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ...1 FC..
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte((value & 0x7F)  | 0x80);  // .... .... .... ..7f
    }
    else if (absv <= 0x0000020000000000LL)
    {
      destination.putByte(((value >> 35)  & 0x7F)); // .... .3F8 .... ....
      destination.putByte(((value >> 28)  & 0x7F)); // .... ...7 F... ....
      destination.putByte(((value >> 21)  & 0x7F)); // .... .... .FE. ....
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ...1 FC..
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte((value & 0x7F)  | 0x80);  // .... .... .... ..7f
    }
    else if (absv <= 0x0001000000000000LL)
    {
      destination.putByte(((value >> 42)  & 0x7F));// ...1 FC.. .... ....
      destination.putByte(((value >> 35)  & 0x7F)); // .... .3F8 .... ....
      destination.putByte(((value >> 28)  & 0x7F)); // .... ...7 F... ....
      destination.putByte(((value >> 21)  & 0x7F)); // .... .... .FE. ....
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ...1 FC..
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte((value & 0x7F)  | 0x80);  // .... .... .... ..7f
    }
    else if (absv <= 0x0080000000000000LL)
    {
      destination.putByte(((value >> 49)  & 0x7F)); // ..FE .... .... ....
      destination.putByte(((value >> 42)  & 0x7F)); // ...1 FC.. .... ....
      destination.putByte(((value >> 35)  & 0x7F)); // .... .3F8 .... ....
      destination.putByte(((value >> 28)  & 0x7F)); // .... ...7 F... ....
      destination.putByte(((value >> 21)  & 0x7F)); // .... .... .FE. ....
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ...1 FC..
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte((value & 0x7F)  | 0x80);  // .... .... .... ..7f
    }
    else
    {
      destination.putByte(((value >> 56)  & 0x7F)); // 7F.. .... .... ....
      destination.putByte(((value >> 49)  & 0x7F)); // ..FE .... .... ....
      destination.putByte(((value >> 42)  & 0x7F)); // ...1 FC.. .... ....
      destination.putByte(((value >> 35)  & 0x7F)); // .... .3F8 .... ....
      destination.putByte(((value >> 28)  & 0x7F)); // .... ...7 F... ....
      destination.putByte(((value >> 21)  & 0x7F)); // .... .... .FE. ....
      destination.putByte(((value >> 14)  & 0x7F)); // .... .... ...1 FC..
      destination.putByte(((value >> 7)   & 0x7F)); // .... .... .... 3F8.
      destination.putByte((value & 0x7F)  | 0x80);  // .... .... .... ..7f
    }
  }
}
#endif
#if 0
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


#else
void
FieldInstruction::encodeUnsignedInteger(DataDestination & destination, WorkingBuffer & buffer, uint64 value)
{
  if (value <      0x0000000000000080ULL)
  {
    destination.putByte(((value & 0x7f) | 0x80));
  }
  else if (value < 0x0000000000004000ULL)
  {
    destination.putByte(((value >> 7) & 0x7F));
    destination.putByte(((value & 0x7F) | 0x80));
  }
  else if (value < 0x0000000000200000ULL)
  {
    destination.putByte(((value >> 14) & 0x7F));
    destination.putByte(((value >> 7) & 0x7F));
    destination.putByte(((value & 0x7F) | 0x80));
  }
  else if (value < 0x0000000010000000ULL)
  {
    destination.putByte(((value >> 21) & 0x7F));
    destination.putByte(((value >> 14) & 0x7F));
    destination.putByte(((value >> 7) & 0x7F));
    destination.putByte(((value & 0x7F) | 0x80));
  }
  else if (value < 0x0000000800000000ULL) // 1000 0000 0000 0000 0000 0000 0000 0000 0000
  {
    destination.putByte(((value >> 28) & 0x7F));
    destination.putByte(((value >> 21) & 0x7F));
    destination.putByte(((value >> 14) & 0x7F));
    destination.putByte(((value >> 7) & 0x7F));
    destination.putByte(((value & 0x7F) | 0x80));
  }
  else if (value < 0x0000040000000000ULL) // 100 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
  {
    destination.putByte(((value >> 35) & 0x7F));
    destination.putByte(((value >> 28) & 0x7F));
    destination.putByte(((value >> 21) & 0x7F));
    destination.putByte(((value >> 14) & 0x7F));
    destination.putByte(((value >> 7) & 0x7F));
    destination.putByte(((value & 0x7F) | 0x80));
  }
  else if (value < 0x0002000000000000ULL) //
  {
    destination.putByte(((value >> 42) & 0x7F));
    destination.putByte(((value >> 35) & 0x7F));
    destination.putByte(((value >> 28) & 0x7F));
    destination.putByte(((value >> 21) & 0x7F));
    destination.putByte(((value >> 14) & 0x7F));
    destination.putByte(((value >> 7) & 0x7F));
    destination.putByte(((value & 0x7F) | 0x80));
  }
  else if (value < 0x0100000000000000ULL) // 1 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
  {
    destination.putByte(((value >> 49) & 0x7F));
    destination.putByte(((value >> 42) & 0x7F));
    destination.putByte(((value >> 35) & 0x7F));
    destination.putByte(((value >> 28) & 0x7F));
    destination.putByte(((value >> 21) & 0x7F));
    destination.putByte(((value >> 14) & 0x7F));
    destination.putByte(((value >> 7) & 0x7F));
    destination.putByte(((value & 0x7F) | 0x80));
  }
  else if (value < 0x8000000000000000ULL) //
 {
    destination.putByte(((value >> 56) & 0x7F));
    destination.putByte(((value >> 49) & 0x7F));
    destination.putByte(((value >> 42) & 0x7F));
    destination.putByte(((value >> 35) & 0x7F));
    destination.putByte(((value >> 28) & 0x7F));
    destination.putByte(((value >> 21) & 0x7F));
    destination.putByte(((value >> 14) & 0x7F));
    destination.putByte(((value >> 7) & 0x7F));
    destination.putByte(((value & 0x7F) | 0x80));
  }
  else
  {
    destination.putByte(((value >> 63) & 0x7F));
    destination.putByte(((value >> 56) & 0x7F));
    destination.putByte(((value >> 49) & 0x7F));
    destination.putByte(((value >> 42) & 0x7F));
    destination.putByte(((value >> 35) & 0x7F));
    destination.putByte(((value >> 28) & 0x7F));
    destination.putByte(((value >> 21) & 0x7F));
    destination.putByte(((value >> 14) & 0x7F));
    destination.putByte(((value >> 7) & 0x7F));
    destination.putByte(((value & 0x7F) | 0x80));
  }
}
#endif

void
FieldInstruction::encodeNullableAscii(DataDestination & destination, const StringBuffer & value)
{
  if(value.empty() || value[0] == '\0')
  {
    destination.putByte(nullableStringPreamble);
  }
  encodeAscii(destination, value);
}

void
FieldInstruction::encodeAscii(DataDestination & destination, const StringBuffer & value)
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
FieldInstruction::encodeBlobData(DataDestination & destination, const StringBuffer & value)
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
FieldInstruction::setDictionaryName(const std::string & /*name*/)
{
  throw TemplateDefinitionError("dictionary= attribute not allowed for this field instruction.");
}

void
FieldInstruction::setSegmentBody(Codecs::SegmentBodyPtr segment)
{
  throw TemplateDefinitionError("Group or Segment definition error.");
}

const Codecs::SegmentBodyPtr&
FieldInstruction::getSegmentBody()
{
  throw TemplateDefinitionError("Group or Segment definition error.");
}

size_t
FieldInstruction::fieldCount(const SegmentBody & /*parent*/)const
{
  return 1;
}

bool
FieldInstruction::getSegmentBody(SegmentBodyPtr & /*segment*/) const
{
  return false;
}

void
FieldInstruction::display(std::ostream & output, size_t indent) const
{
  std::string indentString(indent, ' ');

  std::string elementName = ValueType::typeName(fieldInstructionType());

  output << std::endl << indentString << "<" << elementName;
  identity_.display(output);
  if(!isMandatory())
  {
    output << " presence=\"optional\"";
  }
  if(ignoreOverflow_)
  {
    output << " ignoreOverflow=\"Y\"";
  }
  output << '>';
  output << std::endl << indentString << "  <!-- presence map bits = " << presenceMapBitsUsed_<< "  -->";
  FieldOp::OpType opType = fieldOp_->opType();
  if(opType != FieldOp::NOP)
  {
    const std::string & opName = FieldOp::opName(opType);
    output << std::endl << indentString << "  <" <<  opName;
    if(!fieldOp_->getValue().empty())
    {
      output << " value=\"" << fieldOp_->getValue() << "\"";
    }
    output << "/>";
  }
  displayBody(output, indent + 2);
  output << std::endl << indentString << "</" << elementName << "><!-- " << identity_.getLocalName() << "-->";
}

void
FieldInstruction::displayBody(std::ostream & output, size_t indent)const
{
  // most field instructions have no body.
}


