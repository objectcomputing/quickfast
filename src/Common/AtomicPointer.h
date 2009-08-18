// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef ATOMICPOINTER_H
#define ATOMICPOINTER_H
#include <Common/AtomicOps.h>
namespace QuickFAST
{
  /// Pointer that can be updated atomically
  template<typename Target>
  class AtomicPointer
  {
  public:
    AtomicPointer(Target * value = 0)
      : pointer_(value)
    {
    }

    operator Target *() const
    {
      return pointer_;
    }

    Target * get() const
    {
      return pointer_;
    }

    bool CAS(Target * expected, Target * value)
    {
      // The cast-from-hell to convince c++ to convert from Target * volatile
      // to void * volatile without losing the volatile.
      return CASPtr(
        const_cast<void * volatile * >(reinterpret_cast<void **>(const_cast<Target **>(&pointer_))),
        expected,
        value);
    }
  private:
    Target * volatile pointer_;
  };
}


#endif // ATOMICPOINTER_H
