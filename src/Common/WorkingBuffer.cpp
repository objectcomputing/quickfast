// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "WorkingBuffer.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
static const size_t initialCapacity = 20;
WorkingBuffer::WorkingBuffer()
: reverse_(false)
, capacity_(initialCapacity)
, startPos_(0)
, endPos_(0)
, buffer_(new uchar[initialCapacity])
{
}

WorkingBuffer::~WorkingBuffer()
{
}

void
WorkingBuffer::clear(bool reverse, size_t capacity)
{
  reverse_ = reverse;
  if(capacity > capacity_)
  {
    boost::scoped_array<uchar> newBuffer(new uchar[capacity]);
    buffer_.swap(newBuffer);
    capacity_ = capacity;
  }
  if(reverse_)
  {
    startPos_ = capacity_;
  }
  else
  {
    startPos_ = 0;
  }
  endPos_ = startPos_;
}

void
WorkingBuffer::pop_front()
{
  if(startPos_ < endPos_)
  {
    ++startPos_;
  }
  else
  {
    throw UsageError("pop_front on empty WorkingBuffer.", "");
  }
}

void
WorkingBuffer::push(uchar byte)
{
  if(reverse_)
  {
    if(startPos_ == 0)
    {
      grow();
    }
    assert(startPos_ > 0);
    buffer_[--startPos_] = byte;
  }
  else
  {
    if(endPos_ >= capacity_)
    {
      grow();
    }
    assert(endPos_ < capacity_);
    buffer_[endPos_++] = byte;
  }
}

void
WorkingBuffer::grow()
{
  size_t oldCapacity = capacity_;
  size_t newCapacity = capacity_ * 3 / 2; // todo: parameterize?
  if(newCapacity == 0)
  {
    newCapacity = initialCapacity;
  }
  boost::scoped_array<uchar> newBuffer(new uchar[newCapacity]);
  size_t delta = 0;
  if(reverse_)
  {
    delta = newCapacity - oldCapacity;
  }
  std::copy(buffer_.get(), buffer_.get()+capacity_, newBuffer.get() + delta);
  buffer_.swap(newBuffer);
  startPos_ += delta;
  endPos_ += delta;
  capacity_ = newCapacity;
}
