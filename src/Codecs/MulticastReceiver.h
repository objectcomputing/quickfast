// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTRECEIVER_H
#define MULTICASTRECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "MulticastReceiver_fwd.h"
#include <Common/BufferGenerator.h>
#include <Common/BufferConsumer.h>
#include <Common/AsioService.h>
//
#include <Common/LinkedBuffer.h>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Receive Multicast Packets and pass them to a packet handler
    class /*QuickFAST_Export*/ MulticastReceiver : public AsioService, public Common::BufferGenerator
    {
      // keep a shared_ptr to every buffer in a vector so that when this object
      // is destroyed, the destruction of the vector will trigger the release
      // of the buffers
      typedef boost::shared_ptr<Common::LinkedBuffer> BufferLifetime;
      typedef std::vector<BufferLifetime> BufferLifetimeManager;

    public:
      /// @brief Construct given multicast information.
      /// @param multicastGroupIP multicast address as a text string
      /// @param listenInterfaceIP listen address as a text string.
      ///        This identifies the network interface to be used.
      ///        0.0.0.0 means "let the system choose"
      /// @param portNumber port number
      MulticastReceiver(
        const std::string & multicastGroupIP,
        const std::string & listenInterfaceIP,
        unsigned short portNumber
        )
        : stopping_(false)
        , listenInterface_(boost::asio::ip::address::from_string(listenInterfaceIP))
        , multicastGroup_(boost::asio::ip::address::from_string(multicastGroupIP))
        , endpoint_(listenInterface_, portNumber)
        , socket_(ioService_)
        , readInProgress_(false)
        , noBufferAvailable_(0)
        , packetsReceived_(0)
        , errorPackets_(0)
        , emptyPackets_(0)
        , packetsQueued_(0)
        , batchesProcessed_(0)
        , packetsProcessed_(0)
        , bytesReceived_(0)
        , bytesProcessed_(0)
        , largestPacket_(0)
      {
      }

      /// @brief construct given shared io_service and multicast information
      /// @param ioService an ioService to be shared with other objects
      /// @param multicastGroupIP multicast address as a text string
      /// @param listenInterfaceIP listen address as a text string
      /// @param portNumber port number
      MulticastReceiver(
        boost::asio::io_service & ioService,
        const std::string & multicastGroupIP,
        const std::string & listenInterfaceIP,
        unsigned short portNumber
        )
        : AsioService(ioService)
        , stopping_(false)
        , listenInterface_(boost::asio::ip::address::from_string(listenInterfaceIP))
        , multicastGroup_(boost::asio::ip::address::from_string(multicastGroupIP))
        , endpoint_(listenInterface_, portNumber)
        , socket_(ioService_)
        , readInProgress_(false)
        , noBufferAvailable_(0)
        , packetsReceived_(0)
        , errorPackets_(0)
        , emptyPackets_(0)
        , packetsQueued_(0)
        , batchesProcessed_(0)
        , packetsProcessed_(0)
        , bytesReceived_(0)
        , bytesProcessed_(0)
        , largestPacket_(0)
      {
      }

      ~MulticastReceiver()
      {
      }

      /// @brief how many times were all buffers busy?
      /// @returns the number of times no buffers were available to receive packets.
      size_t noBufferAvailable() const
      {
        return noBufferAvailable_;
      }

      /// @brief How many packets have been received
      /// @returns the number of packets that have been received
      size_t packetsReceived() const
      {
        return packetsReceived_;
      }

      /// @brief How many packets have been queued for processing
      /// @returns the number of packets that have been received
      size_t packetsQueued() const
      {
        return packetsQueued_;
      }

      /// @brief How many batches of packets from the queue have been processed
      /// @returns the number of batches
      size_t batchesProcessed() const
      {
        return batchesProcessed_;
      }


      /// @brief How many packets have been processed
      /// @returns the number of packets that have been processed.
      size_t packetsProcessed() const
      {
        return packetsProcessed_;
      }

      /// @brief How many received packets had errors
      /// @returns the number of packets that have encountered communication errors
      size_t packetsWithErrors() const
      {
        return errorPackets_;
      }

      /// @brief How many received packets were empty
      /// @returns the number of packets that were empty
      size_t emptyPackets() const
      {
        return emptyPackets_;
      }

      /// @brief How many bytes have been received
      /// @returns the number of bytes that have been received
      size_t bytesReceived() const
      {
        return bytesReceived_;
      }

      /// @brief How many bytes have been processed
      /// @returns the number of bytes that have been processed
      size_t bytesProcessed() const
      {
        return bytesProcessed_;
      }

      /// @brief How big was the largest packet received
      /// @returns the number of bytes in the largest packet
      size_t largestPacket() const
      {
        return largestPacket_;
      }

      /// @brief Approximately how many bytes are waiting to be decoded
      size_t bytesReadable() const
      {
        // first check the OS stack
        boost::asio::ip::udp::socket::bytes_readable command;
        const_cast<boost::asio::ip::udp::socket &>(socket_).io_control(command);
        size_t bytesReadable = command.get();
        return bytesReadable + bytesReceived_ - bytesProcessed_;
      }

      /// @brief Start accepting packets.  Returns immediately
      /// @param bufferConsumer accepts and processes the filled buffers
      /// @param bufferSize determines the maximum size of an incoming packet
      /// @param bufferCount is the number of buffers to allocate to receive packets
      void start(
        Common::BufferConsumer & bufferConsumer,
        size_t bufferSize = 1400,
        size_t bufferCount = 2)
      {
        consumer_ = & bufferConsumer;

        socket_.open(endpoint_.protocol());
        socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket_.bind(endpoint_);

        consumer_->receiverStarted();
        if(consumer_->wantLog(Common::Logger::QF_LOG_INFO))
        {
          std::stringstream msg;
          msg << "Joining multicast group: " << multicastGroup_.to_string()
            << " via interface " << endpoint_.address().to_string()
            << ':' << endpoint_.port();
          consumer_->logMessage(Common::Logger::QF_LOG_INFO, msg.str());
        }
        // Join the multicast group.
        boost::asio::ip::multicast::join_group joinRequest(
          multicastGroup_.to_v4(),
          listenInterface_.to_v4());
        socket_.set_option(joinRequest);

        boost::mutex::scoped_lock lock(bufferMutex_);

        for(size_t nBuffer = 0; nBuffer < bufferCount; ++nBuffer)
        {
          BufferLifetime buffer(new Common::LinkedBuffer(bufferSize));
          /// bufferLifetimes_ is used only to clean up on object destruction
          bufferLifetimes_.push_back(buffer);
          idleBufferPool_.push(buffer.get());
        }
        startReceive(lock);
      }

      /// @brief add additional buffers on-the-fly
      /// @param bufferSize is how many bytes in each buffer (should match the number used in start)
      /// @param bufferCount is how many buffers to add
      void addBuffers(
        size_t bufferSize = 1400,
        size_t bufferCount = 1)
      {
        boost::mutex::scoped_lock lock(bufferMutex_);

        for(size_t nBuffer = 0; nBuffer < bufferCount; ++nBuffer)
        {
          BufferLifetime buffer(new Common::LinkedBuffer(bufferSize));
          /// bufferLifetimes_ is used only to clean up on object destruction
          bufferLifetimes_.push_back(buffer);
          idleBufferPool_.push(buffer.get());
        }
      }

      /// @brief Stop accepting packets
      ///
      /// Returns immediately, however decoding may continue until
      /// the decoder reaches a clean stopping point.  In particular
      /// the MessageConsumer may receive additional messages after
      /// stop is called.
      ///
      /// MessageConsumer::decodingStopped() will be called when
      /// the stop request is complete.
      void stop()
      {
        stopping_ = true;
        // attempt to cancel any receive requests in progress.
        socket_.cancel();
      }

      virtual void releaseBuffer(Common::LinkedBuffer * buffer)
      {
        idleBuffers_.push(buffer);
      }


    private:

      void startReceive(boost::mutex::scoped_lock&)
      {
        if( !readInProgress_ && !stopping_)
        {
          Common::LinkedBuffer *buffer = idleBufferPool_.pop();
          if(buffer != 0)
          {
            readInProgress_ = true;
            socket_.async_receive_from(
              boost::asio::buffer(buffer->get(), buffer->capacity()),
              senderEndpoint_,
              boost::bind(&MulticastReceiver::handleReceive,
              this,
              boost::asio::placeholders::error,
              buffer,
              boost::asio::placeholders::bytes_transferred)
              );
          }
          else
          {
            ++noBufferAvailable_;
          }
        }
      }

      void handleReceive(
        const boost::system::error_code& error,
        Common::LinkedBuffer * buffer,
        size_t bytesReceived)
      {
        // should this thread service the queue?
        bool service = false;
        {
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
            if(!consumer_->reportCommunicationError(error.message()))
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
          ++batchesProcessed_;

          Common::LinkedBuffer * buffer = queue_.serviceNext();
          while(buffer != 0)
          {
            ++packetsProcessed_;
            if(!stopping_)
            {
              try
              {
                bytesProcessed_ += buffer->used();
                if(!consumer_->consumeBuffer(*this, buffer))
                {
                  stop();
                }
              }
              catch(const std::exception &ex)
              {
                if(!consumer_->reportDecodingError(ex.what()))
                {
                  stop();
                }
              }
//              idleBuffers_.push(buffer);
            }
            buffer = queue_.serviceNext();
          }
          boost::mutex::scoped_lock lock(bufferMutex_);
          // add idle buffers to pool before trying to start a read.
          idleBufferPool_.push(idleBuffers_);
          startReceive(lock);
          // see if this thread is still needed to service the queue
          service = queue_.endService(!stopping_, lock);
        }
      }

    private:
      bool stopping_;
      boost::asio::ip::address listenInterface_;
      boost::asio::ip::address multicastGroup_;
      boost::asio::ip::udp::endpoint endpoint_;
      boost::asio::ip::udp::endpoint senderEndpoint_;
      boost::asio::ip::udp::socket socket_;
      Common::BufferConsumer * consumer_;

      BufferLifetimeManager bufferLifetimes_;

      boost::mutex bufferMutex_;
      Common::SingleServerBufferQueue queue_;

      // accumulate idle buffers while we process the queue
      // but don't add them back to idle pool until we're done
      // this avoids extra locking, and applies some backpressure to the
      // incoming communication stream (which of course is ignored for multicast)
      Common::SimpleBufferCollection idleBuffers_;

      Common::SimpleBufferCollection idleBufferPool_;
      bool readInProgress_;

      /////////////
      // Statistics
      size_t noBufferAvailable_;
      size_t packetsReceived_;
      size_t errorPackets_;
      size_t emptyPackets_;
      size_t packetsQueued_;
      size_t batchesProcessed_;
      size_t packetsProcessed_;
      size_t bytesReceived_;
      size_t bytesProcessed_;
      size_t largestPacket_;
    };
  }
}
#endif // MULTICASTRECEIVER_H
