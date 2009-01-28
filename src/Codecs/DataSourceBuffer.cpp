// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DataSourceBuffer.h"
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

DataSourceBuffer::DataSourceBuffer(const unsigned char * buffer, size_t length)
: buffer_(buffer)
, end_(length)
, pos_(0)
{
}

DataSourceBuffer::~DataSourceBuffer()
{
}

bool
DataSourceBuffer::readByte(uchar & byte)
{
  if(pos_ < end_)
  {
    byte = buffer_[pos_++];
    return true;
  }
  return false;
}

