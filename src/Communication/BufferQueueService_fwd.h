// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef BUFFERQUEUESERVICE_FWD_H
#define BUFFERQUEUESERVICE_FWD_H
namespace QuickFAST{
  namespace Communication
  {
    class BufferQueueService;
    /// @brief smart pointer to a BufferConsumer
    typedef boost::shared_ptr<BufferQueueService> BufferQueueServicePtr;

  }
}
#endif /* BUFFERQUEUESERVICE_FWD_H */
