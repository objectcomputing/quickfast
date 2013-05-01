// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef BUFFERRECEIVER_H
#define BUFFERRECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "BufferReceiver_fwd.h"
#include <Communication/SynchReceiver.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// A Receiver that reads input from a memory buffer
    class BufferReceiver
      : public SynchReceiver
    {
    public:
      /// @brief A receiver that can accept data buffered in memory.
      ///
      BufferReceiver()
        : buffer_(0)
        , used_(0)
      {
      }

      ~BufferReceiver()
      {
      }

      virtual void receiveBuffer(
        const unsigned char * buffer,
        size_t used
        )
      {
        buffer_ = buffer;
        used_ = used;
        stopping_ = false;
        startReceiveUnlocked();
        while(!stopping_)
        {
          tryServiceQueue();
        }
      }

    private:

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        return true;
      }

      // Implement Receiver method
      bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock& lock)
      {
        bool result = false;
        if(used_ > 0)
        {
          size_t bytes = used_;
          used_ = 0;
          buffer->setExternal(buffer_, bytes);
          acceptFullBuffer(buffer, bytes, lock);
          result = true;
        }
        return result;
      }

      // Implement Receiver method
      virtual void resetService()
      {
        return;
      }
    private:
      const unsigned char * buffer_;
      size_t used_;
    };
  }
}
#endif // BUFFERRECEIVER_H
