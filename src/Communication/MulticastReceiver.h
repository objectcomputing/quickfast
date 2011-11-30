// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MULTICASTRECEIVER_H
#define MULTICASTRECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "MulticastReceiver_fwd.h"
#include <Communication/AsynchReceiver.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// @brief Receive Multicast Packets and pass them to a packet handler
    class MulticastReceiver
      : public AsynchReceiver
    {
    public:
      /// @brief Construct given multicast information.
      /// @param multicastGroupIP multicast address as a text string
      /// @param listenInterfaceIP listen address as a text string.
      ///        This identifies the network interface to be used.
      ///        0.0.0.0 means "let the system choose"
      /// @param bindIP the IP to be used to bind the socket.
      /// @param portNumber port number
      MulticastReceiver(
        const std::string & multicastGroupIP,
        const std::string & listenInterfaceIP,
        const std::string & bindIP,
        unsigned short portNumber
        )
        : AsynchReceiver()
        , listenInterface_(boost::asio::ip::address::from_string(listenInterfaceIP))
        , portNumber_(portNumber)
        , multicastGroup_(boost::asio::ip::address::from_string(multicastGroupIP))
        , bindAddress_(boost::asio::ip::address::from_string(bindIP))
        , endpoint_(listenInterface_, portNumber)
        , socket_(ioService_)
        , joined_(false)
      {
      }

      /// @brief construct given shared io_service and multicast information
      /// @param ioService an ioService to be shared with other objects
      /// @param multicastGroupIP multicast address as a text string
      /// @param listenInterfaceIP listen address as a text string
      /// @param bindIP the IP to be used to bind the socket.
      /// @param portNumber port number
      MulticastReceiver(
        boost::asio::io_service & ioService,
        const std::string & multicastGroupIP,
        const std::string & listenInterfaceIP,
        const std::string & bindIP,
        unsigned short portNumber
        )
        : AsynchReceiver(ioService)
        , listenInterface_(boost::asio::ip::address::from_string(listenInterfaceIP))
        , portNumber_(portNumber)
        , multicastGroup_(boost::asio::ip::address::from_string(multicastGroupIP))
        , bindAddress_(boost::asio::ip::address::from_string(bindIP))
        , endpoint_(listenInterface_, portNumber)
        , socket_(ioService_.ioService())
        , joined_(false)
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
        boost::asio::ip::udp::endpoint bindpoint(bindAddress_, portNumber_);
        socket_.bind(bindpoint);

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
        joined_ = true;
        return true;
      }

      virtual void stop()
      {
        // stop processing buffers first
        AsynchReceiver::pause();

        try
        {
          // leave the multicast group
          if(joined_)
          {
            boost::asio::ip::multicast::leave_group leaveRequest(
              multicastGroup_.to_v4(),
              listenInterface_.to_v4());
            socket_.set_option(leaveRequest);
          }
          // attempt to cancel any receive requests in progress.
          socket_.close();
        }
        catch(...)
        {
        }
        // and then shut everything down for good.
        AsynchReceiver::stop();
      }

      virtual void pause()
      {
        // Temporarily leave the group
        if(joined_)
        {
          boost::asio::ip::multicast::leave_group leaveRequest(
            multicastGroup_.to_v4(),
            listenInterface_.to_v4());
          socket_.set_option(leaveRequest);
          AsynchReceiver::pause();
          joined_ = false;
        }
      }

      virtual void resume()
      {
        AsynchReceiver::resume();
        if(!joined_)
        {
          // rejoin the multicast group
          boost::asio::ip::multicast::join_group joinRequest(
            multicastGroup_.to_v4(),
            listenInterface_.to_v4());
          socket_.set_option(joinRequest);
          joined_ = true;
        }
      }

      /// Provide direct access to the internal asio socket.
      boost::asio::ip::udp::socket & socket()
      {
        return socket_;
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
      unsigned short portNumber_;
      boost::asio::ip::address multicastGroup_;
      boost::asio::ip::address bindAddress_;
      boost::asio::ip::udp::endpoint endpoint_;
      boost::asio::ip::udp::endpoint senderEndpoint_;
      boost::asio::ip::udp::socket socket_;
      bool joined_;
    };
  }
}
#endif // MULTICASTRECEIVER_H
