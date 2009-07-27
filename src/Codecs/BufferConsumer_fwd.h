// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef BUFFERCONSUMER_FWD_H
#define BUFFERCONSUMER_FWD_H

namespace QuickFAST{
  namespace Codecs{
    class BufferConsumer;
    /// @brief smart pointer to a BufferConsumer
    typedef boost::shared_ptr<BufferConsumer> BufferConsumerPtr;
  }
}
#endif // BUFFERCONSUMER_FWD_H
