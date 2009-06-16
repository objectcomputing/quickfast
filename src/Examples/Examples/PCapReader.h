// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef PCAPREADER_H
#define PCAPREADER_H

#include <Common/Types.h>

namespace QuickFAST
{
    class ByteSwapper
  {
  public:
    explicit ByteSwapper(bool swap = false)
      : swap_(swap)
    {
    }

    void setSwap(bool swap)
    {
      swap_ = swap;
    }

    uint16 operator()(uint16 v) const
    {
      if(swap_)
      {
        return (v >> 8) | (v << 8);
      }
      return v;
    }

    int16 operator()(int16 v) const
    {
      if(swap_)
      {
        return ((v >> 8) & 0xFF) | (v << 8);
      }
      return v;
    }

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

  private:
    bool swap_;
  };
  class PCapReader
  {
  public:
    PCapReader();
    bool open(const char * filename);
    bool rewind();
    bool isOk()const;
    bool read(const char *& buffer, size_t & size);
  private:
    boost::scoped_array<char> buffer_;
    size_t fileSize_;
    size_t pos_;
    bool ok_;
    ByteSwapper swap;
  };
}
#endif // PCAPREADER_H
