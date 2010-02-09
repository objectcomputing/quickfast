// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DataSourceBufferedBlockedStream.h"
#include "Common/Constants.h"
using namespace QuickFAST;
using namespace QuickFAST::Codecs;

DataSourceBufferedBlockedStream::DataSourceBufferedBlockedStream(std::istream & stream)
: bufferCapacity_(0)
, bufferPosition_(0)
, blockRemaining_(0)
{
  stream.seekg(0,std::ios::end);
  bufferCapacity_ = stream.tellg();
  stream.seekg(0,std::ios::beg);
  buffer_.reset(new unsigned char[bufferCapacity_]);
  stream.read(reinterpret_cast<char *>(buffer_.get()), bufferCapacity_);
}

DataSourceBufferedBlockedStream::~DataSourceBufferedBlockedStream()
{
}

bool
DataSourceBufferedBlockedStream::readByte(uchar & byte)
{
  bool ok = false;

  if(blockRemaining_ == 0 && bufferPosition_ < bufferCapacity_)
  {
    size_t blockSize = 0;
    uchar b = buffer_[bufferPosition_++];
    while((b & stopBit) == 0 && bufferPosition_ < bufferCapacity_)
    {
      // todo: overflow check
      blockSize <<= dataShift;
      blockSize += b;
      b = buffer_[bufferPosition_++];
    }

    blockSize <<= dataShift;
    blockSize += (b & dataBits);
    /// todo: sanity check on block size?
    if(blockSize + bufferPosition_ >= bufferCapacity_)
    {
      std::cout << "DataSourceBufferedBlockedStream: invalid block size. "
        << blockSize
        << " Remaining bytes: " << bufferCapacity_ - bufferPosition_
        << " at position " << bufferPosition_
        << " File size: " << bufferCapacity_
        << std::endl;
      return false;
    }
    blockRemaining_ = blockSize;
  }
  if(bufferPosition_ < bufferCapacity_)
  {
    byte = buffer_[bufferPosition_];
    ++bufferPosition_;
    --blockRemaining_;
    ok = true;
  }
  return ok;
}

