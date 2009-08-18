// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef ATOMICCOUNTER_H
#define ATOMICCOUNTER_H
#include <Common/AtomicOps.h>
namespace QuickFAST
{
  class AtomicCounter
  {
  public:
    AtomicCounter(long value = 0)
      : counter_(value)
    {
    }
    inline
    operator const long()const
    {
      return counter_;
    }
    inline
    void operator ++()
    {
      atomic_increment_long(&counter_);
    }
    inline
    void operator --()
    {
      atomic_decrement_long(&counter_);
    }
    inline
    bool CAS(long expected, long value)
    {
      return CASLong(&counter_, expected, value);
    }
  private:
    volatile long counter_;
  };
}
#endif // ATOMICCOUNTER_H
