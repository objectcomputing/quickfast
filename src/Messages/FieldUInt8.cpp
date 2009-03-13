// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldUInt8.h"
#include <Common/Exceptions.h>
#ifdef EXPERIMENT_WITH_CACHE
#include <Common/MemoryCache.h>
#endif //EXPERIMENT_WITH_CACHE
using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

#ifdef EXPERIMENT_WITH_CACHE
namespace
{
  MemoryCache<FieldUInt8> cache(500);
}
#endif //EXPERIMENT_WITH_CACHE

FieldCPtr FieldUInt8::nullField_ = new FieldUInt8;

FieldUInt8::FieldUInt8(uchar value)
  : Field(true)
  , value_(value)
{
}

FieldUInt8::FieldUInt8()
  : Field(false)
  , value_(0)
{
}

FieldUInt8::~FieldUInt8()
{
}

Field::FieldType
FieldUInt8::getType()const
{
  return Field::UINT8;
}

uchar
FieldUInt8::toUInt8() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return value_;
}

FieldCPtr
FieldUInt8::create(uchar value)
{
  return new
#ifdef EXPERIMENT_WITH_CACHE
    (cache.allocateUnconstructed())
#endif // EXPERIMENT_WITH_CACHE
    FieldUInt8(value);
}

void
FieldUInt8::freeField()const
{
#ifdef EXPERIMENT_WITH_CACHE
  this->~FieldUInt8();
  cache.freeDestroyed(const_cast<FieldUInt8 *>(this));
#else
  delete this;
#endif // EXPERIMENT_WITH_CACHE
}


FieldCPtr
FieldUInt8::createNull()
{
  return nullField_;
}
