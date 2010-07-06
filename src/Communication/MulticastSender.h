// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTSENDER_H
#define MULTICASTSENDER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "MulticastSender_fwd.h"
#include <Communication/AsynchReceiver.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// @brief Receive Multicast Packets and pass them to a packet handler
    class MulticastSender
    {
    public:
      /// @brief Construct given multicast information.
      /// @param sendAddress multicast address as a text string
      /// @param portNumber port number
      MulticastSender(
        const std::string & sendAddress,
        unsigned short portNumber
        )
        : sendAddress_(sendAddress)
        , portNumber_(portNumber)
        , socket_(ioService_)
      {
      }

      MulticastSender(
        boost::asio::io_service & ioService,
        const std::string & sendAddress,
        unsigned short portNumber
        )
        : ioService_(ioService)
        , sendAddress_(sendAddress)
        , portNumber_(portNumber)
        , socket_(ioService_)
      {
      }

      ~MulticastSender()
      {
      }

      bool initializeSender()
      {
        multicastAddress_ = boost::asio::ip::address::from_string(sendAddress_);
        endpoint_ = boost::asio::ip::udp::endpoint(multicastAddress_, portNumber_);
        socket_.open(endpoint_.protocol());
        return true;
      }

      void stop()
      {
        try
        {
          socket_.close();
        }
        catch(...)
        {
        }
      }

      /// Provide direct access to the internal asio socket.
      boost::asio::ip::udp::socket & socket()
      {
        return socket_;
      }

      /// @brief send data to the socket (synchronous)
      ///
      /// Hint: DataDestination makes a good ConstBufferSequence
      /// @param buffers the source of the data.
      /// @returns the number of bytes transmitted.
      template<typename ConstBufferSequence>
      std::size_t send(const ConstBufferSequence & buffers)
      {
        return socket_.send_to(buffers, endpoint_);
      }

      /// @brief send data to the socket (synchronous)
      ///
      /// Hint: DataDestination makes a good ConstBufferSequence
      /// @param buffers the source of the data.
      /// @param flags see boost documentation for flags available
      /// @param[out] ec set to the error if any.
      /// @returns the number of bytes transmitted.
      template<typename ConstBufferSequence>
      std::size_t send(
        const ConstBufferSequence & buffers,
        boost::asio::ip::udp::socket::message_flags flags,
        boost::system::error_code & ec)
      {
        return socket_.send_to(buffers, flags, endpoint_);
      }

      /// @brief send data to the socket (asynchronous)
      ///
      /// Hint: DataDestination makes a good ConstBufferSequence
      /// Important: ConstBufferSequence must not be changed until the send completes.
      /// @param buffers the source of the data.
      /// @param handler is called on I/O completion
      template<typename ConstBufferSequence, typename WriteHandler>
      void asyncSend(
        const ConstBufferSequence & buffers,
        WriteHandler handler)
      {
        socket_.async_send_to(buffers, handler, endpoint_);
      }

      /// @brief send data to the socket (asynchronous)
      ///
      /// Hint: DataDestination makes a good ConstBufferSequence
      /// Important: ConstBufferSequence must not be changed until the send completes.
      /// @param buffers the source of the data.
      /// @param flags see boost documentation for flags available
      /// @param handler is called on I/O completion
      template<typename ConstBufferSequence, typename WriteHandler>
      void asyncSend(
        const ConstBufferSequence & buffers,
        boost::asio::ip::udp::socket::message_flags flags,
        WriteHandler handler)
      {
        socket_.async_send_to(buffers, flags, handler, endpoint_);
      }

    private:
      AsioService ioService_;
      const std::string & sendAddress_;
      unsigned short portNumber_;
      boost::asio::ip::address multicastAddress_;
      boost::asio::ip::udp::endpoint endpoint_;
      boost::asio::ip::udp::socket socket_;
    };
  }
}
#endif // MULTICASTSENDER_H
