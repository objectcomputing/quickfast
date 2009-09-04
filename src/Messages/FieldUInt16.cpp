// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldUInt16.h"
#include <Common/Exceptions.h>
#ifdef EXPERIMENT_WITH_CACHE
#include <Common/MemoryCache.h>
#endif //EXPERIMENT_WITH_CACHE
using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

#ifdef EXPERIMENT_WITH_CACHE
namespace
{
  MemoryCache<FieldUInt16> cache(500);
}
#endif //EXPERIMENT_WITH_CACHE

FieldCPtr FieldUInt16::nullField_ = new FieldUInt16;

FieldUInt16::FieldUInt16(uint16 value)
  : Field(Field::UINT16, true)
{
  unsignedInteger_ = value;
}

FieldUInt16::FieldUInt16()
  : Field(Field::UINT16, false)
{
}

FieldUInt16::~FieldUInt16()
{
}

uint16
FieldUInt16::toUInt16() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return static_cast<uint16>(unsignedInteger_);
}

FieldCPtr
FieldUInt16::create(uint16 value)
{
  return new
#ifdef EXPERIMENT_WITH_CACHE
    (cache.allocateUnconstructed())
#endif // EXPERIMENT_WITH_CACHE
    FieldUInt16(value);
}

void
FieldUInt16::freeField()const
{
#ifdef EXPERIMENT_WITH_CACHE
  this->~FieldUInt16();
  cache.freeDestroyed(const_cast<FieldUInt16 *>(this));
#else
  delete this;
#endif // EXPERIMENT_WITH_CACHE
}


FieldCPtr
FieldUInt16::createNull()
{
  return nullField_;
}
