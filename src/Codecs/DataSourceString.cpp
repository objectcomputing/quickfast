// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DataSourceString.h"
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

DataSourceString::DataSourceString(const char * cstring)
: string_(cstring)
, first_(true)
{
}

DataSourceString::DataSourceString(const std::string & string)
: string_(string)
, first_(true)
{
}

DataSourceString::~DataSourceString()
{
}
bool
DataSourceString::getBuffer(const uchar *& buffer, size_t & size)
{
  size = 0;
  if(first_)
  {
    first_ = false;
    buffer = reinterpret_cast<const uchar *>(string_.data());
    size = string_.size();
  }
  return size > 0;
}
