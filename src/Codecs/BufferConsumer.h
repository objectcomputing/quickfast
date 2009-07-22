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
#include <Messages/Logger.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief interface to be implemented by a consumer of decoded messages.
    class BufferConsumer : public Messages::Logger
    {
    public:
      virtual ~BufferConsumer(){}

      /// @brief Accept a buffer
      /// @param buffer a contiguous array of bytes.  Valid for the duration of this call
      /// @param bufferSize the count of bytes used in the buffer
      /// @returns true if receiving should continue; false to stop receiving
      virtual bool consumeBuffer(const unsigned char * buffer, size_t bufferSize) = 0;

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
