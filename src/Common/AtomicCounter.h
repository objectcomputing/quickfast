// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ATOMICCOUNTER_H
#define ATOMICCOUNTER_H
#include <Common/AtomicOps.h>
namespace QuickFAST
{
  ///@brief A counter that can be updated atomically.
  class AtomicCounter
  {
  public:
    ///@brief Construct with initial value.
    ///
    /// @param value is initial value of counter.
    AtomicCounter(long value = 0)
      : counter_(value)
    {
    }
    /// @brief Access current value as long
    /// Warning, not synchronized so you know this counter had the returned
    /// value at some point, but not necessarily when.
    inline
    operator const long()const
    {
      return counter_;
    }

    /// @brief Increment atomically
    inline
    long operator ++()
    {
      return atomic_increment_long(&counter_);
    }

    /// @brief Decrement atomically
    inline
    long operator --()
    {
      return atomic_decrement_long(&counter_);
    }

    /// @brief Atomically set the value assuming it hasn't changed from "expected"
    /// @param expected the value that the counter must start with if this is to work
    /// @param value the new value to be stored in the counter
    /// @returns true if the counter was updated.
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
