// Copyright (c) 2009, 2010, 2011, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ATOMICPOINTER_H
#define ATOMICPOINTER_H
#include <Common/AtomicOps.h>
namespace QuickFAST
{
  ///@brief Pointer that can be updated atomically
  template<typename Target>
  class AtomicPointer
  {
  public:
    ///@brief Construct with initial value
    explicit AtomicPointer(Target * value = 0)
      : pointer_(value)
    {
    }

    ///@brief Dereference the pointer via '*'
    operator Target *() const
    {
      return pointer_;
    }

    ///@brief Dereference the pointer via function call.
    Target * get() const
    {
      return pointer_;
    }

    ///@brief Atomically set the pointer assuming it hasn't changed from "expected"
    ///@param expected the value that the pointer must start with if this is to work
    ///@param value the new value to be stored in the pointer
    ///@returns true if the pointer was updated.
    bool CAS(Target * expected, Target * value)
    {
      return CASPtr(
        reinterpret_cast<void * volatile * >(&pointer_),
        expected,
        value);
    }
  private:
    Target * volatile pointer_;
  };
}


#endif // ATOMICPOINTER_H
