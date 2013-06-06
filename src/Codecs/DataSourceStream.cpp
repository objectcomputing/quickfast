// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DataSourceStream.h"
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

DataSourceStream::DataSourceStream(std::istream & stream, size_t bufferSize)
: stream_(stream)
, pos_(0)
, end_(0)
, capacity_(bufferSize)
{
  stream.seekg(0,std::ios::end);
  end_ = (size_t)stream.tellg();
  stream.seekg(0,std::ios::beg);
  if(capacity_ > end_)
  {
    capacity_ = end_;
  }
  buffer_.reset(new unsigned char[capacity_]);
}

DataSourceStream::~DataSourceStream()
{
}

bool
DataSourceStream::getBuffer(const uchar *& buffer, size_t & size)
{
  size = 0;
  if(stream_.good() && !stream_.eof())
  {
    buffer = buffer_.get();
    stream_.read(reinterpret_cast<char *>(buffer_.get()), capacity_);
    size = (size_t)stream_.gcount();
    pos_ += size;
  }
  return size > 0;
}
