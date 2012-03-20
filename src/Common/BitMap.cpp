// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "BitMap.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;

BitMap::BitMap(const uchar * buffer, size_t length)
 : value_((const char *)buffer, length)
{
}

BitMap::BitMap(const BitMap & rhs)
  : value_(rhs.value_)
{
}

BitMap::BitMap()
{
}

BitMap::~BitMap()
{
}

void
BitMap::toString(std::string & value)const
{
  value = value_;
}

BitMap &
BitMap::operator = (const BitMap & rhs)
{
  BitMap temp(rhs);
  swap(temp);
  return *this;
}

void
BitMap::swap(BitMap & rhs)
{
  value_.swap(rhs.value_);
}

