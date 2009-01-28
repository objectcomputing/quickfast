// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DataDestinationString.h"
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

DestinationBufferString::~DestinationBufferString()
{
}
void
DestinationBufferString::putByte(uchar byte)
{
  buffer_.push_back(byte);
}

void
DestinationBufferString::reserve(size_t size)
{
  buffer_.reserve(size);
}

const std::string &
DestinationBufferString::value() const
{
  return buffer_;
}

DataDestinationString::DataDestinationString()
{
}

DataDestinationString::~DataDestinationString()
{
}


void
DataDestinationString::endMessage()
{
  value_.clear();
  for(BufferVector::const_iterator it = buffers_.begin();
    it != buffers_.end();
    ++it)
  {
    DestinationBufferString * dbs = static_cast<DestinationBufferString *>((*it).get());
    value_ += dbs->value();
  }
}

const std::string &
DataDestinationString::getValue()const
{
  return value_;
}

DestinationBufferPtr
DataDestinationString::allocateBuffer()
{
  DestinationBufferPtr result(new DestinationBufferString);
  return result;
}

