// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTRECEIVER_H
#define MULTICASTRECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "MulticastReceiver_fwd.h"
#include <Communication/Receiver.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// @brief Receive Multicast Packets and pass them to a packet handler
    class MulticastReceiver
      : public Receiver
    {
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
        : Receiver()
        , listenInterface_(boost::asio::ip::address::from_string(listenInterfaceIP))
        , multicastGroup_(boost::asio::ip::address::from_string(multicastGroupIP))
        , endpoint_(listenInterface_, portNumber)
        , socket_(ioService_)
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
        : Receiver(ioService)
        , listenInterface_(boost::asio::ip::address::from_string(listenInterfaceIP))
        , multicastGroup_(boost::asio::ip::address::from_string(multicastGroupIP))
        , endpoint_(listenInterface_, portNumber)
        , socket_(ioService_)
      {
      }

      ~MulticastReceiver()
      {
      }

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        socket_.open(endpoint_.protocol());
        socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket_.bind(endpoint_);

        if(assembler_->wantLog(Common::Logger::QF_LOG_INFO))
        {
          std::stringstream msg;
          msg << "Joining multicast group: " << multicastGroup_.to_string()
            << " via interface " << endpoint_.address().to_string()
            << ':' << endpoint_.port();
          assembler_->logMessage(Common::Logger::QF_LOG_INFO, msg.str());
        }

        // Join the multicast group
        boost::asio::ip::multicast::join_group joinRequest(
          multicastGroup_.to_v4(),
          listenInterface_.to_v4());
        socket_.set_option(joinRequest);
        return true;
      }

      virtual void stop()
      {
        Receiver::stop();
        // attempt to cancel any receive requests in progress.
        try
        {
          socket_.close();
        }
        catch(...)
        {
        }
      }

    private:

      bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock& lock)
      {
        socket_.async_receive_from(
          boost::asio::buffer(buffer->get(), buffer->capacity()),
          senderEndpoint_,
          boost::bind(&MulticastReceiver::handleReceive,
          this,
          boost::asio::placeholders::error,
          buffer,
          boost::asio::placeholders::bytes_transferred)
          );
        return true;
      }

    private:
      boost::asio::ip::address listenInterface_;
      boost::asio::ip::address multicastGroup_;
      boost::asio::ip::udp::endpoint endpoint_;
      boost::asio::ip::udp::endpoint senderEndpoint_;
      boost::asio::ip::udp::socket socket_;
    };
  }
}
#endif // MULTICASTRECEIVER_H
