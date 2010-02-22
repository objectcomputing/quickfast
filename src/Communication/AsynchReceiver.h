// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef ASYNCHRECEIVER_H
#define ASYNCHRECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "AsynchReceiver_fwd.h"
#include <Communication/Receiver.h>
#include <Communication/AsioService.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// @brief AsynchReceiver base class for receiving incoming data
    class AsynchReceiver
      : public Receiver
    {
    public:
      AsynchReceiver()
      {
      }

      /// @brief construct given shared io_service
      /// @param ioService an ioService to be shared with other objects
      AsynchReceiver(boost::asio::io_service & ioService)
        : ioService_(ioService)
      {
      }

      ~AsynchReceiver()
      {
      }

      //////
      // Implement Receiver public methods

      virtual void runThreads(size_t threadCount = 0, bool useThisThread = true)
      {
        ioService_.runThreads(threadCount, useThisThread);
      }

      virtual void joinThreads()
      {
        ioService_.joinThreads();
      }

      virtual void run()
      {
        ioService_.run();
      }

      virtual void run_one()
      {
        ioService_.run_one();
      }

      virtual size_t poll()
      {
        return ioService_.poll();
      }

      virtual size_t poll_one()
      {
        return ioService_.poll_one();
      }

      virtual void stop()
      {
        Receiver::stop();
        ioService_.stopService();
      }


    protected:

      /// @brief handle I/O completion from an asynchronous receiver
      ///
      /// Handle the asynchronous callback from I/O initiatied
      /// by fillBuffer().
      /// This is intended to be a callback from boost::asio
      /// For synchronous I/O use acceptFullBuffer() and
      /// tryServiceQueue();
      ///
      /// @param error indicates status of the receive
      /// @param buffer into which the receive happened
      /// @param bytesReceived How much data is in the buffer
      void handleReceive(
        const boost::system::error_code& error,
        LinkedBuffer * buffer,
        size_t bytesReceived)
      {
        // should this thread service the queue?
        bool service = false;
        { // Scope for lock
          boost::mutex::scoped_lock lock(bufferMutex_);
          readInProgress_ = false;
          ++packetsReceived_;
          if (!error)
          {
            // it's possible to receive empty packets.
            if(bytesReceived > 0)
            {
              ++packetsQueued_;
              bytesReceived_ += bytesReceived;
              largestPacket_ = std::max(largestPacket_, bytesReceived);
              buffer->setUsed(bytesReceived);
              if(queue_.push(buffer, lock))
              {
                // A true return from push means that no one is servicing the queue
                // Volunteer to service it. If it returns true, then the offer was
                // accepted.  We'll service the queue after releasing the lock.
                service = queue_.startService(lock);
              }
            }
            else
            {
              // empty buffer? just use it again
              ++emptyPackets_;
              idleBufferPool_.push(buffer);
            }
          }
          else
          {
            ++errorPackets_;
            // after an error, recover the buffer
            idleBufferPool_.push(buffer);
            // and let the consumer decide what to do
            if(!assembler_->reportCommunicationError(error.message()))
            {
              stop();
            }
          }
          // if possible fill another buffer while we process this one
          startReceive(lock);
          // end of scope for lock
        }

        while(service)
        {
          service = serviceQueue();
        }
      }
    protected:
      /// @brief a manager for the boost::io_service object
      AsioService ioService_;
    };
  }
}
#endif // ASYNCHRECEIVER_H
