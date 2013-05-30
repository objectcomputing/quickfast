// Copyright (c) 2009, 2010, 2011, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef BUFFERRECYCLER_H
#define BUFFERRECYCLER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include <Communication/LinkedBuffer_fwd.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// @brief Buffer Recycler interface
    class BufferRecycler
    {
    public:
      virtual ~BufferRecycler(){};

      /// @brief accept an empty buffer for recycling
      /// @param emptyBuffer is a buffer ready to be reused
      virtual void recycle(LinkedBuffer * emptyBuffer) = 0;
    };
  }
}
#endif // BUFFERRECYCLER_H
