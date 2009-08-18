// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef ATOMICOPS_H
#define ATOMICOPS_H

// This header defines functions to execute atomic operations inline

#if defined(_WIN32)
# include "windows.h"
# if defined(_MSC_VER)
#   pragma intrinsic(_InterlockedCompareExchange)
// No intrinsic compare and swap pointer so we asm it below
# endif
#elif defined(__GNUC__)
#include <sys/atomic_op.h>
#else // something else.  Solaris maybe?
#include <sys/atomic.h>
#endif

namespace QuickFAST
{
  /// @brief compare and swap pointers.
  ///
  /// @param target the pointer to be updated
  /// @param ifeq the expected value before updating
  /// @param value the new value to be stored in target
  inline
  bool CASPtr(void * volatile * target, void * ifeq, void * value)
  {
#if defined(_WIN32)
#  if defined(_M_IX86) && (_M_IX86 >= 400) && ! defined(_M_IA64)
    /// 32 bit Intel 486 or better supports cmpxchg
    /// but msvc doesn't provide an intrinsic version, so
    /// define it using inline asm to avoid the OS call.
    //#pragma message("Assembly language CASPtr")
    __asm
    {
      mov eax, ifeq
      mov ecx, value
      mov edx, target
      lock cmpxchg [edx], ecx
      sete al
      xor ah, ah
    }
# else
    // non-x86/32 WIN32 platforms.
    // note the WIN32 function takes its arguments in an unusual order.
    // #pragma mesasge("function call CASPtr")
    return ifeq == InterlockedCompareExchangePointer(
      (PVOID volatile *)target, value, ifeq);
# endif
#elif defined(__GNUC__)
# if 0 // force ASM on gcc (assume x86 processor)
    return __sync_bool_compare_and_swap(target, ifeq, value);
# else // force ASM on gcc
    bool result;
    asm(
      "lock \n\t"
      "cmpxchgl %%ecx,(%%edx) \n\t"
      "setz %%al \n\t"
      "xor %%ah,%%ah"
      : "=a" (result)
      : "a" (ifeq) , "c" (value), "d"(target) /* */
      : "memory"
    );
    return result;
# endif // force asm on gcc
#else // otherwise we hope this is defind on your favorite platform
    return ifeq == atomic_cas_ptr(target, ifeq, value);
#endif
  }

  /// @brief compare and swap longs
  ///
  /// @param target the long integer to be updated
  /// @param ifeq the expected value before updating
  /// @param value the new value to be stored in target
  inline
  bool CASLong(long volatile * target, long ifeq, long value)
  {
#if defined(_WIN32)
#  if defined(_M_IX86) && (_M_IX86 >= 400) && ! defined(_M_IA64)
    /// 32 bit Intel 486 or better supports cmpxchg
    /// but msvc doesn't provide an intrinsic version, so
    /// define it using inline asm to avoid the OS call.
    //#pragma message("Assembly language CASLong")
    __asm
    {
      mov eax, ifeq
      mov ecx, value
      mov edx, target
      lock cmpxchg [edx], ecx
      sete al
      xor ah, ah
    }
# else // cpu type
    // non-x86/32 WIN32 platforms.
    return ifeq == _InterlockedCompareExchange(target, value, ifeq);
# endif // cpu type
#elif defined(__GNUC__)
# if 0 // force ASM on gcc
    return __sync_bool_`_swap(target, ifeq, value);
# else // force ASM on gcc
    bool result;
    asm(
      "lock \n\t"
      "cmpxchgl %%ecx,(%%edx) \n\t"
      "setz %%al \n\t"
      "xor %%ah,%%ah"
      : "=a" (result)
      : "a" (ifeq), "c" (value), "d"(target)
      : "memory"
    );
    return result;
# endif // force asm on gcc
#else
    return ifeq == atomic_cas_ulong(target, ifeq, value);
#endif
  }

  /// @brief Increment a long integer atomically
  ///
  /// @param target points to the long to be updated
  inline
  void atomic_increment_long(volatile long * target)
  {
#if defined(_WIN32)
    // this is intrinsic thanks to the pragma above
    (void)_InterlockedIncrement(target);
#elif defined(__GNUC__)
    (void)__sync_add_and_fetch(target, long(1));
#else
    atomic_inc_long(target)
#endif
  }

  /// @brief Increment a long integer atomically
  ///
  /// @param target points to the long to be updated
  inline
  void atomic_decrement_long(volatile long * target)
  {
#if defined(_WIN32)
    // this is intrinsic thanks to the pragma above
    (void)InterlockedDecrement(target);
#elif defined(__GNUC__)
    (void)__sync_sub_and_fetch(target, long(1));
#else
    atomic_dec_long(target)
#endif
  }

}
#endif // ATOMICOPS_H
