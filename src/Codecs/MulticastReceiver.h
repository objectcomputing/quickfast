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
      typedef boost::scoped_array<unsigned char> Buffer;
    public:
      /// @brief construct given multicast information and a consumer
      /// @param multicastAddressName multicast address as a text string
      /// @param listenAddressName listen address as a text string
      /// @param portNumber port number
      MulticastReceiver(
        const std::string & multicastAddressName,
        const std::string & listenAddressName,
        unsigned short portNumber
        )
      : stopping_(false)
      , packetCount_(0)
      , listenAddress_(boost::asio::ip::address::from_string(listenAddressName))
      , multicastAddress_(boost::asio::ip::address::from_string(multicastAddressName))
      , endpoint_(listenAddress_, portNumber)
      , socket_(ioService_)
      , bufferSize_(0)
      {
      }

      /// @brief construct given shared io_service, multicast information, and a consumer
      /// @param templateRegistry the templates to use for decoding
      /// @param ioService an ioService to be shared with other objects
      /// @param multicastAddressName multicast address as a text string
      /// @param listenAddressName listen address as a text string
      /// @param portNumber port number
      MulticastReceiver(
        boost::asio::io_service & ioService,
        const std::string & multicastAddressName,
        const std::string & listenAddressName,
        unsigned short portNumber
        )
      : AsioService(ioService)
      , stopping_(false)
      , packetCount_(0)
      , listenAddress_(boost::asio::ip::address::from_string(listenAddressName))
      , multicastAddress_(boost::asio::ip::address::from_string(multicastAddressName))
      , endpoint_(listenAddress_, portNumber)
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

      /// @brief Start accepting packets.  Returns immediately
      /// @param bufferConsumer accepts and processes the filled buffers
      /// @param bufferSize determines the maximum size of an incoming packet
      void start(
        BufferConsumerPtr  bufferConsumer,
        size_t bufferSize = 5000,
        size_t bufferCount = 2)
      {
        consumer_ = bufferConsumer;
        bufferSize_ = bufferSize;

        // todo configure # buffers/ honor bufferCount
        buffer1_.reset(new unsigned char[bufferSize_]);
        buffer2_.reset(new unsigned char[bufferSize_]);

        socket_.open(endpoint_.protocol());
        socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket_.bind(endpoint_);

        consumer_->receiverStarted();

        // Join the multicast group.
        socket_.set_option(
          boost::asio::ip::multicast::join_group(multicastAddress_));
        startReceive(&buffer1_, &buffer2_);
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
        Buffer * buffer,
        size_t bytesReceived,
        Buffer * altBuffer)
      {
        if(stopping_)
        {
          return;
        }
        if (!error)
        {
          // accept data into the other buffer while we process this buffer
          startReceive(altBuffer, buffer);
          ++packetCount_;
          if(!consumer_->consumeBuffer(buffer->get(), bytesReceived))
          {
            stopping_ = true;
            socket_.cancel();
            return;
          }
        }
        else
        {
          if(!consumer_->reportCommunicationError(error.message()))
          {
            stopping_ = true;
            socket_.cancel();
          }
        }
      }

      void startReceive(Buffer * buffer, Buffer * altBuffer)
      {
        socket_.async_receive_from(
          boost::asio::buffer(buffer->get(), bufferSize_),
          senderEndpoint_,
          strand_.wrap(
            boost::bind(&MulticastReceiver::handleReceive,
              this,
              boost::asio::placeholders::error,
              buffer,
              boost::asio::placeholders::bytes_transferred,
              altBuffer)
              )
            );
      }

    private:
      bool stopping_;
      size_t packetCount_;
      boost::asio::ip::address listenAddress_;
      boost::asio::ip::address multicastAddress_;
      boost::asio::ip::udp::endpoint endpoint_;
      boost::asio::ip::udp::endpoint senderEndpoint_;
      boost::asio::ip::udp::socket socket_;
      BufferConsumerPtr consumer_;

      size_t bufferSize_;
        // todo configure # buffers
      Buffer buffer1_;
      Buffer buffer2_;
    };
  }
}
#endif // MULTICASTRECEIVER_H
