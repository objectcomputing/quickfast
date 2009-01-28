// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DataSourceString.h"
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

DataSourceString::DataSourceString(const char * cstring)
: string_(cstring)
, pos_(0)
{
}

DataSourceString::DataSourceString(const std::string & string)
: string_(string)
, pos_(0)
{
}

DataSourceString::~DataSourceString()
{
}

bool
DataSourceString::readByte(uchar & byte)
{
  if(pos_ < string_.size())
  {
    byte = static_cast<uchar>(string_[pos_++]);
    return true;
  }
  return false;
}

