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
, first_(true)
{
}

DataSourceBuffer::~DataSourceBuffer()
{
}


bool
DataSourceBuffer::getBuffer(const uchar *& buffer, size_t & size)
{
  if(!first_)
  {
    return false;
  }
  first_ = false;
  buffer = buffer_;
  size = end_;
  return true;
}
