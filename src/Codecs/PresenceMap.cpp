// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "PresenceMap.h"
#include <Common/Exceptions.h>
#include <Common/Constants.h>
#include <Codecs/DataSource.h>
#include <Codecs/DataDestination.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

static const uchar startByteMask = '\x40';

PresenceMap::PresenceMap(size_t bits)
  : bitMask_(startByteMask)
  , bytePosition_(0)
  , byteLength_((bits + 7)/8)
  , bits_(new uchar[byteLength_])
  , vout_(0)
{
  std::fill(bits_.get(), bits_.get()+byteLength_, '\0');
}
void
PresenceMap::setRaw(const uchar * buffer, size_t byteLength)
{
  if(byteLength > byteLength_)
  {
    byteLength_ = byteLength;
    bits_.reset(new uchar[byteLength_]);
  }
  std::fill(bits_.get(), bits_.get()+byteLength_, 0);
  std::copy(buffer, buffer+byteLength, bits_.get());
  rewind();
}

void
PresenceMap::getRaw(const uchar *& buffer, size_t &byteLength)const
{
  buffer = &bits_[0];
  byteLength = byteLength_;
}

void
PresenceMap::grow()
{
  // todo: consider reporting this as a recoverable error due to performance impact
  boost::scoped_array<uchar> newBuffer(new uchar [byteLength_+1]);
  newBuffer[byteLength_] = 0;
  std::copy(bits_.get(), bits_.get()+byteLength_, newBuffer.get());
  bits_.swap(newBuffer);
  byteLength_ += 1;
}

void
PresenceMap::appendByte(size_t & pos, uchar byte)
{
  if(pos >= byteLength_)
  {
    assert(pos == byteLength_);
    grow();
  }
  bits_[pos++] = byte;
}

size_t
PresenceMap::encodeBytesNeeded()const
{
  if(bytePosition_ == 0 && bitMask_ == startByteMask)
  {
    return 0;
  }
  size_t bpos = bytePosition_;
  // if the last byte is unused, don't write it.
  if(bitMask_ == startByteMask)
  {
    bpos -= 1;
  }
  while(bpos > 0 && bits_[bpos] == 0)
  {
    bpos--;
  }
  return bpos + 1;
}

void
PresenceMap::encode(DataDestination & destination)
{
  if(bytePosition_ == 0 && bitMask_ == startByteMask)
  {
    return;
  }
  size_t bpos = bytePosition_;
  // if the last byte is unused, don't write it.
  if(bitMask_ == startByteMask)
  {
    bpos -= 1;
  }
  while(bpos > 0 && bits_[bpos] == 0)
  {
    bpos--;
  }
  bits_[bpos] |= stopBit;
  for(size_t pos = 0; pos <= bpos; ++pos)
  {
    destination.putByte(bits_[pos]);
  }
  if(vout_)
  {
    (*vout_) << "pmap["  <<  byteLength_ << "]->" << std::hex;
    for(size_t pos = 0; pos < byteLength_; ++pos)
    {
      (*vout_) << ' ' << std::setw(2) << static_cast<unsigned short>(bits_[pos]);
    }
    (*vout_) << std::dec << std::endl;
  }
}

bool
PresenceMap::decode(Codecs::DataSource & source)
{
  reset();

  uchar byte;
  if(!source.getByte(byte))
  {
    return false;
  }
  size_t pos = 0;
  while((byte & stopBit) == 0)
  {
    appendByte(pos, byte);
    if(!source.getByte(byte))
    {
      return false;
    }
  }
  appendByte(pos, byte);

  if(vout_)
  {
    (*vout_) << "pmap["  <<  byteLength_ << "]<-" << std::hex;
    for(size_t pos = 0; pos < byteLength_; ++pos)
    {
      (*vout_) << ' ' << std::setw(2) <<  static_cast<unsigned short>(bits_[pos]);
    }
    (*vout_) << std::dec << std::endl;
  }

  return true;
}

void
PresenceMap::rewind()
{
  bytePosition_ = 0;
  bitMask_ = startByteMask;
}

bool
PresenceMap::checkNextField()
{
  if(bytePosition_ >= byteLength_)
  {
    if(vout_)(*vout_) << "pmap:at end [" << bytePosition_ << "]" << std::endl;
    return false;
  }
  bool result = (bits_[bytePosition_] & bitMask_) != 0;
  if(vout_)(*vout_) << "check pmap[" << bytePosition_ << '/' <<  byteLength_ << ','
    << std::hex << static_cast<unsigned short>(bitMask_) << std::dec << ']' <<(result?'T' : 'F')
    << std::endl;
  bitMask_ >>= 1;
  if(bitMask_ == 0)
  {
    bitMask_ = startByteMask;
    bytePosition_ += 1;
  }
  return result;
}

bool
PresenceMap::checkSpecificField(size_t bit)
{
  size_t byte = bit / 7;
  if(byte >= byteLength_)
  {
    return false;
  }
  size_t bitNum = bit % 7;
  unsigned char bitmask = startByteMask >> bitNum;
  bool result = ((bits_[byte] & bitmask) != 0);
  if(vout_)(*vout_) << "check specific pmap[" << bit << " -> " << byte << '/' <<  byteLength_ << ','
    << std::hex << static_cast<unsigned short>(bitmask) << '&' << bits_[byte] << std::dec << ']' <<(result?'T' : 'F')
    << std::endl;
  return result;

}



void
PresenceMap::reset(size_t bitCount)
{
  if(bitCount > 0)
  {
    size_t bytes = (bitCount + 7)/8;
    if(bytes > byteLength_)
    {
      bits_.reset(new uchar[bytes]);
      byteLength_ = bytes;
    }
  }

  std::fill(bits_.get(), bits_.get() + byteLength_, 0);
  rewind();

}
void
PresenceMap::setNextField(bool present)
{
  if(bytePosition_ >= byteLength_)
  {
    grow();
  }
  if(present)
  {
    bits_[bytePosition_] |= bitMask_;
  }
  else
  {
    bits_[bytePosition_] &= ~bitMask_;
  }
  if(vout_)(*vout_) << "set pmap[" << bytePosition_ << '/' <<  byteLength_ << ','
    << std::hex << static_cast<unsigned short>(bitMask_) << std::dec << ']' <<(present?'T' : 'F')
    << std::endl;
  bitMask_ >>= 1;
  if(bitMask_ == 0)
  {
    bitMask_ = startByteMask;
    bytePosition_ += 1;
  }
}

bool
PresenceMap::operator == (const PresenceMap &  rhs)const
{
  if(bytePosition_ != rhs.bytePosition_) return false;
  if(bitMask_ != rhs.bitMask_) return false;
  for(size_t pos = 0; pos < bytePosition_; ++pos)
  {
    if(bits_[pos] != rhs.bits_[pos]) return false;
  }
  uchar mask = ((- bitMask_) << 1) & dataBits; // a bit of binary magic here ;-)
  if(((bits_[bytePosition_] ^ rhs.bits_[bytePosition_]) & mask) != 0) return false;
  return true;
}

