// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FixedSizeHeaderAnalyzer.h"
#include <Common/Types.h>
#include <Codecs/DataSource.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FixedSizeHeaderAnalyzer::FixedSizeHeaderAnalyzer(
  size_t sizeBytes,
  bool bigEndian,
  size_t prefixBytes,
  size_t suffixBytes,
  size_t sequenceOffset,
  size_t sequenceLength
  )
: prefixBytes_(prefixBytes)
, sizeBytes_(sizeBytes)
, suffixBytes_(suffixBytes)
, sequenceOffset_(sequenceOffset)
, sequenceLength_(sequenceLength)
, bigEndian_(bigEndian)
, swapNeeded_(ByteSwapper::isBigEndian() ? !bigEndian : bigEndian)
, state_(ParsingIdle)
, blockSize_(0)
, byteCount_(0)
, testSkip_(0)
, headersParsed_(0)
{
}

FixedSizeHeaderAnalyzer::~FixedSizeHeaderAnalyzer()
{
}

bool
FixedSizeHeaderAnalyzer::analyzeHeader(DataSource & source, size_t & blockSize, bool & skip)
{
  skip = false;
  blockSize = 0;
  while(state_ != ParsingComplete)
  {
    switch(state_)
    {
    case ParsingIdle:
      {
        source.beginField("FIXED_SIZE_HEADER");
        state_ = ParsingPrefix;
        byteCount_ = 0;
        break;
      }
    case ParsingPrefix:
      {
        while(byteCount_ < prefixBytes_)
        {
          uchar next = 0;
          if(!source.getByte(next))
          {
            return false;
          }
          ++byteCount_;
        }
        state_ = ParsingBlockSize;
        byteCount_ = 0;
        blockSize_ = 0;
        break;
      }
    case ParsingBlockSize:
      {
        while(byteCount_ < sizeBytes_)
        {
          uchar next = 0;
          if(!source.getByte(next))
          {
            return false;
          }
          if(swapNeeded_)
          {
            blockSize_ |= (next & 0xFF) << (byteCount_ * 8);
          }
          else
          {
            blockSize_ <<= 8;
            blockSize_ |= (next & 0xFF);
          }
          ++byteCount_;
        }
        state_ = ParsingSuffix;
        byteCount_ = 0;

        break;
      }
    case ParsingSuffix:
      {
        while(byteCount_ < suffixBytes_)
        {
          uchar next = 0;
          if(!source.getByte(next))
          {
            return false;
          }
          ++byteCount_;
        }
        state_ = ParsingComplete;
        break;
      }
    default:
      {
        break;
      }
    }
  }
  state_ = ParsingIdle;
  blockSize = blockSize_;
  blockSize_ = 0;
  byteCount_ = 0;
  if(testSkip_ != 0 && (++headersParsed_ % testSkip_ == 0))
  {
    std::cout << std::endl << "SKIPPING HEADER " << headersParsed_ << std::endl;
    skip = true;
  }
  return true;
}

bool
FixedSizeHeaderAnalyzer::supportsSequenceNumber()const
{
  return true;
}

uint32
FixedSizeHeaderAnalyzer::getSequenceNumber(const uchar * buffer) const
{
  uint32 value = 0;
  if(bigEndian_)
  {
    size_t nByte = 0;
    while(nByte < sequenceLength_)
    {
      value <<= 8;
      value |= buffer[sequenceOffset_ + nByte++];
    }
  }
  else
  {
    size_t nByte = sequenceLength_;
    while(nByte != 0)
    {
      value <<= 8;
      value |= buffer[sequenceOffset_ + --nByte];
    }
  }
  return value;
}


void
FixedSizeHeaderAnalyzer::reset()
{
  state_ = ParsingIdle;
  blockSize_ = 0;
  byteCount_ = 0;
}
