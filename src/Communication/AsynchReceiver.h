// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
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

      virtual void resetService()
      {
        ioService_.resetService();
        stopping_ = false;
        paused_ = false;
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

      virtual bool waitBuffer()
      {
        while(!stopping_)
        {
          if(queue_.peekOutgoing() != 0)
          {
            return true;
          }
          //std::ostringstream msg;
          //msg << "AR:{"<< (void *) this <<  "} waiting for a buffer" << std::endl;
          //std::cout << msg.str() << std::flush;

          bool empty = false;
          {
            boost::mutex::scoped_lock lock(bufferMutex_);
            //if(!idleBuffers_.isEmpty())
            //{
            //  std::ostringstream msg;
            //  msg << "AR:{"<< (void *) this <<  "} making idle buffers available" << std::endl;
            //  std::cout << msg.str() << std::flush;
            //}
            idleBufferPool_.push(idleBuffers_);
            // be sure we have a read request in progress
            startReceive(lock);
            // promote any ancoming messages to outgoing
            empty = !queue_.refresh(lock, ioService_.runningThreadCount() != 1 && !stopping_);
          }
          // if there is only one thread servicing the ioService
          // and there are no buffers waiting, then let the
          // ioService borrow this thread long enough to accept
          // the next incoming packet of data.
          // TODO: the test for one thread is not completely valid because more than one receiver can share
          // and I/O service.
          if(empty && ioService_.runningThreadCount() == 1 && !stopping_)
          {
            //std::ostringstream msg;
            //msg << "AR:{"<< (void *) this <<  "} no buffers: service one io service event{"<< (void *)(&ioService_) << "}. Thread count " << ioService_.runningThreadCount() << std::endl;
            //std::cout << msg.str() << std::flush;
            ioService_.poll_one();
          }
        }
        return false;
      }

      /// @brief post a completion handler for later processing
      /// @param handler is the completion handler to be posted
      template<typename CompletionHandler>
      void post(CompletionHandler handler)
      {
        ioService_.post(handler);
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
          --readsInProgress_;
          ++packetsReceived_;
          if (!error)
          {
            // it's possible to receive empty packets.
            if(bytesReceived <= 0)
            {
              // empty buffer? just use it again
              ++emptyPackets_;
              idleBufferPool_.push(buffer);
            }
            else if(paused_)
            {
              // We're paused.  Ignore incoming packets
              ++pausedPackets_;
              idleBufferPool_.push(buffer);
            }
            else
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
          }
          else
          {
            // after an error, recover the buffer
            idleBufferPool_.push(buffer);
            // ignore errors during state transitions
            if(!paused_ && !stopping_)
            {
              ++errorPackets_;
              // and let the consumer decide what to do
              if(!assembler_->reportCommunicationError(error.message()))
              {
                stop();
              }
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
