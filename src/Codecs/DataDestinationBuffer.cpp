// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DataDestinationBuffer.h"
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

DataDestinationBuffer::DataDestinationBuffer()
  : iovecCapacity_(0)
  , iovecUsed_(0)
{
}

DataDestinationBuffer::~DataDestinationBuffer()
{
}

void
DataDestinationBuffer::endMessage()
{
}

void
DataDestinationBuffer::getValue(IoVecArray & iovector, size_t count)const
{
  size_t size = buffers_.size();
  if( size >= iovecCapacity_)
  {
    iovector_.reset(new iovec[size]);
    iovecCapacity_ = size;
  }
  for(iovecUsed_ = 0; iovecUsed_ < size; ++iovecUsed_)
  {
    iovector_[iovecUsed_].iov_base = const_cast<uchar *>(buffers_[iovecUsed_].begin());
    iovector_[iovecUsed_].iov_len = buffers_[iovecUsed_].size();
  }
  iovector = iovector_.get();
  count = iovecUsed_;
}

void
DataDestinationBuffer::allocateBuffer()
{
  WorkingBuffer empty;
  empty.clear(false);
  buffers_.push_back(empty);
  capacity_ = buffers_.size();
}

void
DataDestinationBuffer::putByte_i(BufferHandle handle, uchar byte)
{
  buffers_[handle].push(byte);
#if 0 // handy when debugging
  std::cout << '[' << handle << ':' << buffers_[handle].size() << ']' << std::hex << std::setw(2) << std::setfill('0') << unsigned short(byte) << std::setfill(' ') << std::dec << ' ';
  static size_t mod = 0;
  if(++mod % 16 == 0) std::cout << std::endl;
#endif
}

void
DataDestinationBuffer::clear_i(BufferHandle handle)
{
  buffers_[handle].clear(false);
}

std::string
DataDestinationBuffer::toString()const
{
#if 0 // handy when debugging
  std::cout << std::endl << buffers_.size() << " buffers: ";
#endif
  std::string result;
  for(size_t pos = 0; pos < buffers_.size(); ++pos)
  {
#if 0 // handy when debugging
    std::cout << buffers_[pos].size() << ' ';
#endif
    result.append(reinterpret_cast<const char *>(buffers_[pos].begin()),
      buffers_[pos].size());
  }
#if 0 // handy when debugging
  std::cout << std::endl;
#endif
  return result;

}

