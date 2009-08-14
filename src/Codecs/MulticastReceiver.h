// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTRECEIVER_H
#define MULTICASTRECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "MulticastReceiver_fwd.h"
#include <Common/AsioService.h>
#include <Codecs/BufferConsumer.h>

namespace QuickFAST{
  namespace Codecs {

    /// @brief Receive Multicast Packets and pass them to a packet handler
    class /*QuickFAST_Export*/ MulticastReceiver : public AsioService
    {
      typedef boost::shared_array<unsigned char> Buffer;
      typedef std::vector<Buffer> BufferPool;

    public:
      /// @brief construct given multicast information and a consumer
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
      , packetCount_(0)
      , listenInterface_(boost::asio::ip::address::from_string(listenInterfaceIP))
      , multicastGroup_(boost::asio::ip::address::from_string(multicastGroupIP))
      , endpoint_(listenInterface_, portNumber)
      , socket_(ioService_)
      , bufferSize_(0)
      {
      }

      /// @brief construct given shared io_service, multicast information, and a consumer
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
      , packetCount_(0)
      , listenInterface_(boost::asio::ip::address::from_string(listenInterfaceIP))
      , multicastGroup_(boost::asio::ip::address::from_string(multicastGroupIP))
      , endpoint_(listenInterface_, portNumber)
      , socket_(ioService_)
      , bufferSize_(0)
      {
      }

      ~MulticastReceiver()
      {
      }

      /// @brief How many packetd have been decoded.
      /// @returns the number of messages that have been decoded.
      size_t packetCount() const
      {
        return packetCount_;
      }

      /// @brief estimate how many bytes are waiting to be processed
      /// @returns a very rough estimate!
      size_t bytesReadable() const
      {
        boost::asio::ip::udp::socket::bytes_readable command;
        const_cast<boost::asio::ip::udp::socket &>(socket_).io_control(command);
        return command.get();
      }


      /// @brief Start accepting packets.  Returns immediately
      /// @param bufferConsumer accepts and processes the filled buffers
      /// @param bufferSize determines the maximum size of an incoming packet
      /// @param bufferCount is the number of buffers to allocate to receive packets
      void start(
        BufferConsumer & bufferConsumer,
        size_t bufferSize = 1600,
        size_t bufferCount = 2)
      {
        consumer_ = & bufferConsumer;
        bufferSize_ = bufferSize;

        socket_.open(endpoint_.protocol());
        socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket_.bind(endpoint_);

        consumer_->receiverStarted();
        if(consumer_->wantLog(BufferConsumer::LOG_INFO))
        {
          std::stringstream msg;
          msg << "Joining multicast group: " << multicastGroup_.to_string()
            << " via interface " << endpoint_.address().to_string() << ':' << endpoint_.port();
          consumer_->logMessage(BufferConsumer::LOG_INFO, msg.str());
        }
        // Join the multicast group.
        boost::asio::ip::multicast::join_group joinRequest(multicastGroup_.to_v4(), listenInterface_.to_v4());
        socket_.set_option(joinRequest);

// temporarily single buffer        for(size_t nBuffer = 0; nBuffer < bufferCount; ++nBuffer)
        {
          Buffer buffer(new unsigned char[bufferSize_]);
          /// buffers_ is used only to clean up on object destruction
          buffers_.push_back(buffer);
          startReceive(buffer.get());
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
        // attempt to cancel any requests in progress.
        socket_.cancel();
      }

    private:
      void handleReceive(
        const boost::system::error_code& error,
        unsigned char * buffer,
        size_t bytesReceived)
      {
        if(consumer_->wantLog(BufferConsumer::LOG_INFO))
        {
          std::stringstream msg;
          msg << "Incoming packet containing " << bytesReceived << " bytes." << std::endl;
          consumer_->logMessage(BufferConsumer::LOG_INFO, msg.str());
        }
        if(stopping_)
        {
          return;
        }
        if (!error)
        {
          // during shutdown it's possible to receive empty packets.
          if(bytesReceived > 0)
          {
            boost::mutex::scoped_lock lock(handlerMutex_);
            ++packetCount_;
            if(!consumer_->consumeBuffer(buffer, bytesReceived))
            {
              stop();
            }
          }
        }
        else
        {
          if(!consumer_->reportCommunicationError(error.message()))
          {
            stop();
          }
        }
        if(!stopping_)
        {
          startReceive(buffer);
        }
      }

      void startReceive(unsigned char * buffer)
      {
        socket_.async_receive_from(
          boost::asio::buffer(buffer, bufferSize_),
          senderEndpoint_,
            boost::bind(&MulticastReceiver::handleReceive,
              this,
              boost::asio::placeholders::error,
              buffer,
            boost::asio::placeholders::bytes_transferred)
            );
      }

    private:
      bool stopping_;
      size_t packetCount_;
      boost::asio::ip::address listenInterface_;
      boost::asio::ip::address multicastGroup_;
      boost::asio::ip::udp::endpoint endpoint_;
      boost::asio::ip::udp::endpoint senderEndpoint_;
      boost::asio::ip::udp::socket socket_;
      BufferConsumer * consumer_;

      size_t bufferSize_;
      BufferPool buffers_;
      boost::mutex handlerMutex_;
    };
  }
}
#endif // MULTICASTRECEIVER_H
