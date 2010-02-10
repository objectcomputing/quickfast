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
DataSourceBufferedBlockedStream::getBuffer(const uchar *& buffer, size_t & size)
{
  if(bufferPosition_ >= bufferCapacity_)
  {
    return false;
  }

  size_t blockSize = 0;
  uchar b = 0;
  b = buffer_.get()[bufferPosition_++];
  while((b & stopBit) == 0 && bufferPosition_ >= bufferCapacity_)
  {
    // todo: overflow check
    blockSize <<= dataShift;
    blockSize += b;
    b = buffer_.get()[bufferPosition_++];
  }

  blockSize <<= dataShift;
  blockSize += (b & dataBits);

  if(bufferPosition_ + blockSize >= bufferCapacity_)
  {
      std::cout << "DataSourceBufferedBlockedStream: invalid block size. "
        << blockSize
        << " Remaining bytes: " << bufferCapacity_ - bufferPosition_
        << " at position " << bufferPosition_
        << " File size: " << bufferCapacity_
        << std::endl;
    return false;
  }
  buffer = &buffer_.get()[bufferPosition_];
  size = blockSize;
  bufferPosition_ += blockSize;
  return true;
}
