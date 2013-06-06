// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FastEncodedHeaderAnalyzer.h"
#include <Common/Types.h>
#include <Codecs/DataSource.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FastEncodedHeaderAnalyzer::FastEncodedHeaderAnalyzer(
  size_t prefix,
  size_t suffix,
  bool hasBlockSize
  )
: prefixCount_(prefix)
, suffixCount_(suffix)
, state_(ParsingIdle)
, hasBlockSize_(hasBlockSize)
, blockSize_(0)
, fieldCount_(0)
{
}



FastEncodedHeaderAnalyzer::~FastEncodedHeaderAnalyzer()
{
}

bool
FastEncodedHeaderAnalyzer::analyzeHeader(DataSource & source, size_t & blockSize, bool & skip)
{
  while(state_ != ParsingComplete)
  {
    switch(state_)
    {
    case ParsingIdle:
      {
        source.beginField("FAST_ENCODED_HEADER");
        state_ = ParsingPrefix;
        fieldCount_ = 0;
//        break;
      }
    case ParsingPrefix:
      {
        while(fieldCount_ < prefixCount_)
        {
          uchar next = 0;
          if(!source.getByte(next))
          {
            return false;
          }
          if((next & 0x80) != 0)
          {
            ++fieldCount_;
          }
        }
        state_ = ParsingBlockSize;
        blockSize_ = 0;
//        break;
      }
    case ParsingBlockSize:
      {
        if(!hasBlockSize_)
        {
          state_ = ParsingSuffix;
          fieldCount_ = 0;
        }
        while(state_ == ParsingBlockSize)
        {
          uchar next = 0;
          if(!source.getByte(next))
          {
            return false;
          }
          blockSize_ <<= 7;
          blockSize_ |= (next & 0x7f);
          if((next & 0x80) != 0)
          {
            state_ = ParsingSuffix;
            fieldCount_ = 0;
          }
        }
//        break;
      }
    case ParsingSuffix:
      {
        while(fieldCount_ < suffixCount_)
        {
          uchar next = 0;
          if(!source.getByte(next))
          {
            return false;
          }
          if((next & 0x80) != 0)
          {
            ++fieldCount_;
          }
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
  blockSize = blockSize_;
  skip = false;
  state_ = ParsingIdle;
  return true;
}
