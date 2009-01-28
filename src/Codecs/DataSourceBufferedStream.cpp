// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DataSourceBufferedStream.h"
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

DataSourceBufferedStream::DataSourceBufferedStream(std::istream & stream)
: pos_(0)
, end_(0)
{
  stream.seekg(0,std::ios::end);
  end_ = stream.tellg();
  stream.seekg(0,std::ios::beg);
  buffer_.reset(new unsigned char[end_]);
  stream.read(reinterpret_cast<char *>(buffer_.get()), end_);
}

DataSourceBufferedStream::~DataSourceBufferedStream()
{
}

bool
DataSourceBufferedStream::readByte(uchar & byte)
{
  if(pos_ < end_)
  {
    byte = buffer_[pos_++];
    return true;
  }
  return false;
}

