// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DataSourceBlockedStream.h"
#include "Common/Constants.h"
using namespace QuickFAST;
using namespace QuickFAST::Codecs;

DataSourceBlockedStream::DataSourceBlockedStream(std::istream & stream)
: stream_(stream)
, bufferCapacity_(0)
, bufferUsed_(0)
, bufferPosition_(0)
{
}

DataSourceBlockedStream::~DataSourceBlockedStream()
{
}

bool
DataSourceBlockedStream::readByte(uchar & byte)
{
  bool ok = false;
  if(bufferPosition_ >= bufferUsed_)
  {
    std::cout << std::endl << "DataSourceBlockedStream reading block size." << std::endl;
    size_t blockSize = 0;
    uchar b = 0;
    b = uchar(stream_.get());
    while((b & stopBit) == 0 && stream_.good() && !stream_.eof())
    {
      // todo: overflow check
      std::cout << std::hex << std::setw(2) << std::setfill('0') << unsigned short(b) << std::dec << ' ' << std::endl;
      blockSize <<= dataShift;
      blockSize += b;
      b = uchar(stream_.get());
    }
    std::cout << std::hex << std::setw(2) << std::setfill('0') << unsigned short(b) << std::dec << std::setfill(' ') << std::endl;

    blockSize <<= dataShift;
    blockSize += (b & dataBits);
    /// todo: sanity check on block size?
    if(blockSize > bufferCapacity_)
    {
      buffer_.reset(new unsigned char[blockSize]);
      bufferCapacity_ = blockSize;
      std::cout << std::endl <<  "DataSourceBlockedStream: Grow buffer to " << bufferCapacity_ << " bytes." << std::endl;
    }
    if(stream_.good() && !stream_.eof())
    {
      stream_.read(reinterpret_cast<char *>(buffer_.get()), blockSize);
      bufferUsed_ = stream_.gcount();
      bufferPosition_ = 0;
      std::cout << std::endl <<  "DataSourceBlockedStream: Block contains " << bufferUsed_ << " bytes." << std::endl;
    }
  }
  if(bufferPosition_ < bufferUsed_)
  {
    byte = buffer_[bufferPosition_];
    bufferPosition_++;
    ok = true;
  }
  return ok;
}

