// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DataSourceStream.h"
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

DataSourceStream::DataSourceStream(std::istream & stream)
: stream_(stream)
{
}

DataSourceStream::~DataSourceStream()
{
}

bool
DataSourceStream::readByte(uchar & byte)
{
  if(stream_.good() && !stream_.eof())
  {
    byte = stream_.get();
  }
  return stream_.good() && !stream_.eof();
}

