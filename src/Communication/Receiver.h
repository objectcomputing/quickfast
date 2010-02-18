// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef RECEIVER_H
#define RECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "Receiver_fwd.h"
#include <Communication/Assembler.h>
#include <Communication/AsioService.h>
#include <Communication/LinkedBuffer.h>
#include <Common/Exceptions.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// @brief Receiver base class for receiving incoming data
    class Receiver
      : public AsioService
    {
    public:
      Receiver()
        : bufferSize_(1400)
        , stopping_(false)
        , readInProgress_(false)
        , noBufferAvailable_(0)
        , packetsReceived_(0)
        , bytesReceived_(0)
        , errorPackets_(0)
        , emptyPackets_(0)
        , packetsQueued_(0)
        , batchesProcessed_(0)
        , packetsProcessed_(0)
        , bytesProcessed_(0)
        , largestPacket_(0)
      {
      }

      /// @brief construct given shared io_service
      /// @param ioService an ioService to be shared with other objects
      Receiver(boost::asio::io_service & ioService)
        : AsioService(ioService)
        , bufferSize_(1400)
        , stopping_(false)
        , readInProgress_(false)
        , noBufferAvailable_(0)
        , packetsReceived_(0)
        , bytesReceived_(0)
        , errorPackets_(0)
        , emptyPackets_(0)
        , packetsQueued_(0)
        , batchesProcessed_(0)
        , packetsProcessed_(0)
        , bytesProcessed_(0)
        , largestPacket_(0)
      {
      }

      ~Receiver()
      {
      }

      /// @brief Start accepting packets.  Returns immediately
      /// @param assembler accepts and processes the filled buffers
      /// @param bufferSize determines the maximum size of an incoming packet
      /// @param bufferCount is the number of buffers to allocate to receive packets
      /// @param returns true if service started successfully
      bool start(
        Assembler & assembler,
        size_t bufferSize = 1400,
        size_t bufferCount = 2)
      {
        bool result = false;
        assembler_ = & assembler;
        bufferSize_ = bufferSize;
        if(initializeReceiver())
        {
          assembler_->receiverStarted(*this);

          // Allocate initial set of buffers
          boost::mutex::scoped_lock lock(bufferMutex_);

          for(size_t nBuffer = 0; nBuffer < bufferCount; ++nBuffer)
          {
            BufferLifetime buffer(new LinkedBuffer(bufferSize));
            /// bufferLifetimes_ is used only to clean up on object destruction
            bufferLifetimes_.push_back(buffer);
            idleBufferPool_.push(buffer.get());
          }
          startReceive(lock);
          result = true;
        }
        return result;
      }

      ///////////////////////////////////////
      // Assembler support routines

      /// @brief get the next buffer if possible
      /// WARNING: This should be called only by a Assembler
      ///          during a call from this Receiver
      /// @param if true, will wait until a buffer is available
      ///        or the service is stopped)
      /// @return true pointer to the buffer or zero
      virtual LinkedBuffer * getBuffer(bool wait)
      {
        LinkedBuffer *next = queue_.serviceNext();
        bool more = true;
        while(more && next == 0)
        {
          more = wait;
          {
            boost::mutex::scoped_lock lock(bufferMutex_);
            // add any idle buffers to pool
            idleBufferPool_.push(idleBuffers_);
            startReceive(lock);
            queue_.refresh(lock, wait);
          }
          next = queue_.serviceNext();
        }
        if(next != 0)
        {
          ++packetsProcessed_;
          bytesProcessed_ += next->used();
        }
        return next;
      }

      /// @brief check to see if "needed" bytes are available
      /// WARNING: This should be called only by a Assembler
      ///          during a call from this Receiver
      /// @param needed number of bytes needed
      /// @wait true if we should wait for them to arrive;
      ///       false if we should just return false
      /// @returns true if needed bytes are available.
      bool needBytes(size_t needed, bool wait)
      {
        size_t available = 0;
        bool more = true;
        while(available < needed && more)
        {
          const LinkedBuffer *next = queue_.peekOutgoing();
          while(available < needed && next != 0)
          {
            available += next->used();
            next = next->link();
          }
          if(available < needed && wait)
          {
            boost::mutex::scoped_lock lock(bufferMutex_);
            // add any idle buffers to pool
            idleBufferPool_.push(idleBuffers_);
            startReceive(lock);
            queue_.refresh(lock, wait);
            available = 0;
          }
          else
          {
            more = false;
          }
        }
        return available >= needed;
      }

      /// @brief Release empty buffer.
      /// WARNING: This should be called only by a Assembler
      ///          during a call from this Receiver
      /// @param buffer points to the buffer to be released.
      void releaseBuffer(LinkedBuffer * buffer)
      {
        idleBuffers_.push(buffer);
      }

      //////////////////////////////////////////////////////////
      // methods to be implemented by specific types of receiver
    protected:
      /// @brief Do initial communications startup.
      ///
      /// Do not start I/O during this method.
      /// @returns true if started successfully
      virtual bool initializeReceiver() = 0;

      /// @brief Fill a buffer
      ///
      /// Asynchrounous Receivers should arrange for handleReceive()
      /// to be called at the completion of the read started during
      /// this method.  Normally an asynchronous receiver will return
      /// true from this method and report any errors asynchronously
      /// via the error parameter of handleReceive.
      ///
      /// Synchronous Receivers should call acceptFullBuffer() during
      /// this method.  Synchronous Receivers in particular should
      /// return false if data cannot be read.
      ///
      /// @param buffer to be filled
      /// @returns true if the buffer is, or will be filled.
      virtual bool fillBuffer(
        LinkedBuffer * buffer,
        boost::mutex::scoped_lock& lock) = 0;

    protected:

      void startReceiveUnlocked()
      {
        boost::mutex::scoped_lock lock(bufferMutex_);
        startReceive(lock);
      }
      /// @brief Receive a new buffer full if possible
      /// scoped_lock parameter means a mutex must be locked
      void startReceive(boost::mutex::scoped_lock& lock)
      {
        if( !readInProgress_ && !stopping_)
        {
          LinkedBuffer *buffer = idleBufferPool_.pop();
          if(buffer != 0)
          {
            readInProgress_ = true;
            if(!fillBuffer(buffer, lock))
            {
              idleBufferPool_.push(buffer);
              readInProgress_ = false;
              stop();
            }
          }
          else
          {
            ++noBufferAvailable_;
          }
        }
      }

      /// @brief Accept a buffer from a synchronous receiver
      ///
      /// The Receiver implementation should call this method
      /// if the buffer is filled during the fillBuffer() call.
      /// I.e. if synchronous I/O is used to fill the buffer.
      /// For asynchronous I/O use handleReceive() instead.
      ///
      /// The lock from the fill buffer call should be returned
      /// for this call.
      /// If this returns true then the caller shoul eventually
      /// call tryServiceQueue() -- but NOT from the fill buffer call.
      bool acceptFullBuffer(
        LinkedBuffer * buffer,
        size_t bytesReceived,
        boost::mutex::scoped_lock & lock
        )
      {
        bool needService = false;
        readInProgress_ = false;
        ++packetsReceived_;
        if(bytesReceived > 0)
        {
          ++packetsQueued_;
          largestPacket_ = std::max(largestPacket_, bytesReceived);
          buffer->setUsed(bytesReceived);
          needService = queue_.push(buffer, lock);
        }
        else
        {
          // empty buffer? just use it again
          ++emptyPackets_;
          idleBufferPool_.push(buffer);
        }
        return needService;
      }

      /// @brief service the queue from a synchronous receiver.
      void tryServiceQueue()
      {
        bool service = false;
        { // Scope for lock
          boost::mutex::scoped_lock lock(bufferMutex_);
          service = queue_.startService(lock);
        }
        while(service)
        {
          service = serviceQueue();
        }
      }

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

    private:
      /// @brief implement queue service
      bool serviceQueue()
      {
          ++batchesProcessed_;
          if(!assembler_->serviceQueue(*this))
          {
            stop();
          }
          boost::mutex::scoped_lock lock(bufferMutex_);
          // add idle buffers to pool before trying to start a read.
          idleBufferPool_.push(idleBuffers_);
          startReceive(lock);
          // see if this thread is still needed to service the queue
          return queue_.endService(!stopping_, lock);
      }
    public:
      /// @brief Stop accepting packets
      ///
      /// Returns immediately, however decoding may continue until
      /// the decoder reaches a clean stopping point.  In particular
      /// the Assembler may receive additional messages after
      /// stop is called.
      ///
      /// Assembler::decodingStopped() will be called when
      /// the stop request is complete.
      virtual void stop()
      {
        stopping_ = true;
      }

      /// @brief add additional buffers on-the-fly
      /// @param bufferCount is how many buffers to add
      void addBuffers(
        size_t bufferCount = 1)
      {
        boost::mutex::scoped_lock lock(bufferMutex_);

        for(size_t nBuffer = 0; nBuffer < bufferCount; ++nBuffer)
        {
          BufferLifetime buffer(new LinkedBuffer(bufferSize_));
          /// bufferLifetimes_ is used only to clean up on object destruction
          bufferLifetimes_.push_back(buffer);
          idleBufferPool_.push(buffer.get());
        }
      }

      /// @brief Statistic: How many times were all buffers busy?
      /// @returns the number of times no buffers were available to receive packets.
      size_t noBufferAvailable() const
      {
        return noBufferAvailable_;
      }

      /// @brief Statistic: How many packets have been received
      /// @returns the number of packets that have been received
      size_t packetsReceived() const
      {
        return packetsReceived_;
      }

      /// @brief Statistic: How many packets have been queued for processing
      /// @returns the number of packets that have been received
      size_t packetsQueued() const
      {
        return packetsQueued_;
      }

      /// @brief Statistic: How many batches of packets have been processed
      /// @returns the number of batches
      size_t batchesProcessed() const
      {
        return batchesProcessed_;
      }

      /// @brief Statistic: How many packets have been processed
      /// @returns the number of packets that have been processed.
      size_t packetsProcessed() const
      {
        return packetsProcessed_;
      }

      /// @brief Statistic: How many bytes have been processed
      /// @returns the number of bytes that have been processed.
      size_t bytesProcessed() const
      {
        return bytesProcessed_;
      }

      /// @brief Statistic: How many received packets had errors
      /// @returns the number of packets that have encountered communication errors
      size_t packetsWithErrors() const
      {
        return errorPackets_;
      }

      /// @brief Statistic: How many received packets were empty
      /// @returns the number of packets that were empty
      size_t emptyPackets() const
      {
        return emptyPackets_;
      }

      /// @brief Statistic: How many bytes have been received
      /// @returns the number of bytes that have been received
      size_t bytesReceived() const
      {
        return bytesReceived_;
      }

      /// @brief Statistic: How big was the largest packet received
      /// @returns the number of bytes in the largest packet
      size_t largestPacket() const
      {
        return largestPacket_;
      }

      /// @brief Approximately how many bytes are waiting to be decoded
      size_t bytesReadable() const
      {
        // todo: we *could* ask the socket how much data is waiting
        return bytesReceived_ - bytesProcessed_;
      }

    protected:
      Assembler * assembler_;

      BufferLifetimeManager bufferLifetimes_;

      boost::mutex bufferMutex_;
      SingleServerBufferQueue queue_;

      // accumulate idle buffers while we process the queue
      // but don't add them back to idle pool until we're done
      // this avoids extra locking, and applies some backpressure to the
      // incoming communication stream (which of course is ignored for multicast)
      BufferCollection idleBuffers_;

      BufferCollection idleBufferPool_;
      size_t bufferSize_;
      bool stopping_;
      bool readInProgress_;

      /////////////
      // Statistics
      size_t noBufferAvailable_;
      size_t packetsReceived_;
      size_t bytesReceived_;
      size_t errorPackets_;
      size_t emptyPackets_;
      size_t packetsQueued_;
      size_t batchesProcessed_;
      size_t packetsProcessed_;
      size_t bytesProcessed_;
      size_t largestPacket_;
    };
  }
}
#endif // RECEIVER_H
