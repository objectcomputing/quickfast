// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DataDestinationString.h"
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

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
    value_ += (*it);
  }
}

const std::string &
DataDestinationString::getValue()const
{
  return value_;
}

void
DataDestinationString::allocateBuffer()
{
  std::string empty;
  buffers_.push_back(empty);
  capacity_ = buffers_.size();
}

void
DataDestinationString::putByte(BufferHandle handle, uchar byte)
{
  buffers_[handle].push_back(byte);
}

void
DataDestinationString::clear()
{
  for(BufferVector::iterator it = buffers_.begin();
    it != buffers_.end();
    ++it)
  {
    (*it).clear();
  }
  used_ = 0;
}

