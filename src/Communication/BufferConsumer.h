// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef BUFFERCONSUMER_H
#define BUFFERCONSUMER_H
#include "BufferConsumer_fwd.h"
//#include <Common/QuickFAST_Export.h>
#include <Communication/BufferGenerator_fwd.h>
#include <Communication/LinkedBuffer.h>
#include <Common/Logger.h>

namespace QuickFAST{
  namespace Communication
  {
    /// @brief interface to be implemented by a consumer of incoming raw data buffers.
    class BufferConsumer : public Common::Logger
    {
    public:
      virtual ~BufferConsumer(){}

      /// @brief Accept a buffer
      /// @param buffer a contiguous array of bytes.  Valid for the duration of this call
      /// @param bufferSize the count of bytes used in the buffer
      /// @returns true if receiving should continue; false to stop receiving
      virtual bool consumeBuffer(BufferGenerator & generator, LinkedBuffer * buffer) = 0;

      /// @brief notify consumer that receiver is started
      /// Will be called once before consumeBuffer is called
      virtual void receiverStarted() = 0;

      /// @brief notify consumer that receiver is stopped
      /// No calls to consumeBuffer will be generated after this call.
      virtual void receiverStopped() = 0;
    };
  }
}
#endif /* BUFFERCONSUMER_H */
