// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef BUFFERGENERATOR_H
#define BUFFERGENERATOR_H
//#include <Common/QuickFAST_Export.h>
#include "BufferGenerator_fwd.h"
#include <Communication/LinkedBuffer.h>

namespace QuickFAST{
  namespace Communication{
    /// @brief interface to be implemented by a generator of incoming raw data buffers.
    class BufferGenerator : public boost::enable_shared_from_this<BufferGenerator>
    {
    public:
      virtual ~BufferGenerator()
      {
      }

      /// @brief Release empty buffer.
      /// WARNING: This should be called only within the scope of a call to
      /// BufferConsumer::consumeBuffer();
      /// @param buffer points to the buffer to be released.
      virtual void releaseBuffer(LinkedBuffer * buffer) = 0;
    };
  }
}
#endif /* BUFFERGENERATOR_H */
