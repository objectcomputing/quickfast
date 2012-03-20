// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef BYTESWAPPER_H
#define BYTESWAPPER_H
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>

namespace QuickFAST
{
  /// @brief a function object that returns true if this is a big endian machine
  union BigEndian
  {
  private:
    unsigned short asShort;
    unsigned char asBytes[sizeof(unsigned short)];
  public:
    BigEndian()
      : asShort(1)
    {
    }
    /// @brief Is this a big endian machine?
    /// @returns true if this is a big endian machine
    bool operator() ()const
    {
      return asBytes[0] == 0;
    }
  };

  /// @brief a helper function object to access data that may need to be byte swapped.
  class ByteSwapper
  {
  public:
    /// @brief construct and enable or disable swapping
    ///
    /// @param swap if true swapping occurs.
    explicit ByteSwapper(bool swap = false)
      : swap_(swap)
    {
    }

    /// @brief change the state of swap after-the-fact
    ///
    /// @param swap if true swapping occurs.
    void setSwap(bool swap)
    {
      swap_ = swap;
    }

    /// @brief conditionally swap an unsigned 16 bit integer
    ///
    /// @param v the value to be swapped
    /// @returns the swapped value
    uint16 operator()(uint16 v) const
    {
      if(swap_)
      {
        return (v >> 8) | (v << 8);
      }
      return v;
    }


    /// @brief conditionally swap a signed 16 bit integer
    ///
    /// @param v the value to be swapped
    /// @returns the swapped value
    int16 operator()(int16 v) const
    {
      if(swap_)
      {
        return ((v >> 8) & 0xFF) | (v << 8);
      }
      return v;
    }


    /// @brief conditionally swap an unsigned 32 bit integer
    ///
    /// @param v the value to be swapped
    /// @returns the swapped value
    uint32 operator()(uint32 v) const
    {
      if(swap_)
      {
        return ((v >> 24) & 0x000000FF)
             | ((v >> 8)  & 0x0000FF00)
             | ((v << 8)  & 0x00FF0000)
             | ((v << 24) & 0xFF000000)
             ;
      }
      return v;
    }


    /// @brief conditionally swap a signed 32 bit integer
    ///
    /// @param v the value to be swapped
    /// @returns the swapped value
    int32 operator()(int32 v) const
    {
      if(swap_)
      {
        return ((v >> 24) & 0x000000FF)
             | ((v >> 8)  & 0x0000FF00)
             | ((v << 8)  & 0x00FF0000)
             | ((v << 24) & 0xFF000000)
             ;
      }
      return v;
    }

    /// @brief Test the endianness of this machine.
    /// @returns true if big-endian.
    static bool isBigEndian()
    {
      static BigEndian nativeBigEndian;
      return nativeBigEndian();
    }

  private:

    bool swap_;
  };

}
#endif // BYTESWAPPER_H
