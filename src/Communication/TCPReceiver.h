// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef TCPRECEIVER_H
#define TCPRECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "TCPReceiver_fwd.h"
#include <Communication/AsynchReceiver.h>
namespace QuickFAST
{
  namespace Communication
  {
    /// @brief Receive TCP Packets and pass them to a packet handler
    class TCPReceiver
      : public AsynchReceiver
    {
    public:
      /// @brief Construct given multicast information.
      /// @param hostName identifies the host to connect to
      /// @param port port service name or number
      TCPReceiver(
        const std::string & hostName,
        const std::string & port
        )
        : AsynchReceiver()
        , hostName_(hostName)
        , port_(port)
        , socket_(ioService_)
      {
      }

      /// @brief construct given shared io_service and multicast information
      /// @param ioService an ioService to be shared with other objects
      /// @param hostName is the name or dotted IP to connect to
      /// @param port port service name or number
      TCPReceiver(
        boost::asio::io_service & ioService,
        const std::string & hostName,
        const std::string & port
        )
        : AsynchReceiver(ioService)
        , hostName_(hostName)
        , port_(port)
        , socket_(ioService_)
      {
      }

      ~TCPReceiver()
      {
      }

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        bool ok = true;
        // generate a collection of possible endpoints for this host:port
        boost::asio::ip::tcp::resolver resolver(ioService_);
        boost::asio::ip::tcp::resolver::query query( hostName_, port_);
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

        // then iterate thru the collection until we find one that works.
        boost::system::error_code error;
        boost::asio::ip::tcp::resolver::iterator endIterator;
        while(error && iterator != endIterator)
        {
          socket_.connect(*iterator, error);
        }
        if(error)
        {
          ok = false;
          std::stringstream msg;
          msg << "Cannot connect to [" << hostName_ << ':' << port_ << "]: " << error;
          (void) assembler_->reportCommunicationError(msg.str());
        }
        else
        {
          if(assembler_->wantLog(Common::Logger::QF_LOG_INFO))
          {
            std::stringstream msg;
            msg << "Connected to: " << hostName_ << ':' << port_;
            assembler_->logMessage(Common::Logger::QF_LOG_INFO, msg.str());
          }
        }
        return ok;
      }

      // Implement Receiver method
      virtual void stop()
      {
        Receiver::stop();
        try
        {
          socket_.close();
        }
        catch(...){}
      }

      /// Provide direct access to the internal asio socket.
      boost::asio::ip::tcp::socket & socket()
      {
        return socket_;
      }

    private:

      bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock& lock)
      {
        socket_.async_receive(
          boost::asio::buffer(buffer->get(), buffer->capacity()),
          boost::bind(&TCPReceiver::handleReceive,
            this,
            boost::asio::placeholders::error,
            buffer,
            boost::asio::placeholders::bytes_transferred)
          );
        return true;
      }

    private:
      std::string hostName_;
      std::string port_;
      boost::asio::ip::tcp::socket socket_;
    };
  }
}
#endif // TCPRECEIVER_H
