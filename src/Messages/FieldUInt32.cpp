// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldUInt32.h"
#include <Common/Exceptions.h>
#ifdef EXPERIMENT_WITH_CACHE
#include <Common/MemoryCache.h>
#endif //EXPERIMENT_WITH_CACHE
using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

#ifdef EXPERIMENT_WITH_CACHE
namespace
{
  MemoryCache<FieldUInt32> cache(500);
}
#endif //EXPERIMENT_WITH_CACHE

FieldCPtr FieldUInt32::nullField_ = new FieldUInt32;

FieldUInt32::FieldUInt32(uint32 value)
  : Field(Field::UINT32, true)
  , value_(value)
{
}

FieldUInt32::FieldUInt32()
  : Field(Field::UINT32, false)
  , value_(0)
{
}

FieldUInt32::~FieldUInt32()
{
}

uint32
FieldUInt32::toUInt32() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return value_;
}

FieldCPtr
FieldUInt32::create(uint32 value)
{
  return new
#ifdef EXPERIMENT_WITH_CACHE
    (cache.allocateUnconstructed())
#endif // EXPERIMENT_WITH_CACHE
    FieldUInt32(value);
}

void
FieldUInt32::freeField()const
{
#ifdef EXPERIMENT_WITH_CACHE
  this->~FieldUInt32();
  cache.freeDestroyed(const_cast<FieldUInt32 *>(this));
#else
  delete this;
#endif // EXPERIMENT_WITH_CACHE
}


FieldCPtr
FieldUInt32::createNull()
{
  return nullField_;
}
