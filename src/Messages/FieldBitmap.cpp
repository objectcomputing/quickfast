// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldBitMap.h"
#include <Common/Exceptions.h>

using namespace QuickFAST;
using namespace QuickFAST::Messages;

FieldBitMap::FieldBitMap(const BitMap & value)
  : Field(true)
  , value_(value)
{
}

FieldBitMap::FieldBitMap(const uchar * value, size_t length)
  : Field(true)
  , value_(value, length)
{
}

FieldBitMap::FieldBitMap()
  : Field(false)
  , value_()
{
}

FieldBitMap::~FieldBitMap()
{
}

Field::FieldType
FieldBitMap::getType() const
{
  return Field::BITMAP;
}

const BitMap &
FieldBitMap::toBitMap() const
{
  if(!valid_)
  {
   throw FieldNotPresent("Field not present");
  }
  return value_;
}

FieldCPtr
FieldBitMap::create(const BitMap & value)
{
  return new FieldBitMap(value);
}

FieldCPtr
FieldBitMap::create(const uchar * buffer, size_t length)
{
  return new FieldBitMap(buffer, length);
}

FieldCPtr
FieldBitMap::createNull()
{
  return new FieldBitMap;
}

