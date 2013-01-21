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
    private:
      /// @brief private internal class to represet a multicast feed
      class MulticastFeed
      {
      public:
        MulticastFeed(
          MulticastReceiver & parent,
          AsioService & ioService,
          const std::string & name,
          const std::string & multicastGroupIP,
          const std::string & listenInterfaceIP,
          const std::string & bindIP,
          unsigned short portNumber
          )
        : parent_(parent)
        , name_(name)
        , listenInterface_(boost::asio::ip::address::from_string(listenInterfaceIP))
        , portNumber_(portNumber)
        , multicastGroup_(boost::asio::ip::address::from_string(multicastGroupIP))
        , bindAddress_(boost::asio::ip::address::from_string(bindIP))
        , endpoint_(listenInterface_, portNumber)
        , socket_(ioService)
        , joined_(false)
        , readInProgress_(false)
        {
        }

        const std::string & name()const
        {
          return name_;
        }

        bool canStartRead() const
        {
          return !readInProgress_;
        }

        bool initializeReceiver()
        {
          socket_.open(endpoint_.protocol());
          socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
          boost::asio::ip::udp::endpoint bindpoint(bindAddress_, portNumber_);
          socket_.bind(bindpoint);

          // Join the multicast group
          boost::asio::ip::multicast::join_group joinRequest(
            multicastGroup_.to_v4(),
            listenInterface_.to_v4());
          socket_.set_option(joinRequest);
          joined_ = true;
          return true;
        }

        bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock &)
        {
          if(readInProgress_)
          {
            return false;
          }
          readInProgress_ = true;
//          std::cout << "Start read on feed: " << name_ << std::endl;
          socket_.async_receive_from(
            boost::asio::buffer(buffer->get(), buffer->capacity()),
            senderEndpoint_,
            boost::bind(&MulticastFeed::handleReceive,
              this,
              boost::asio::placeholders::error,
              buffer,
              boost::asio::placeholders::bytes_transferred)
            );
          return true;
        }

        void handleReceive(
          const boost::system::error_code& error,
          LinkedBuffer * buffer,
          size_t bytesReceived)
        {
//          std::cout << "Receive on feed: " << name_ << std::endl;
          assert(readInProgress_);
          readInProgress_ = false;
          parent_.handleReceive(error, buffer, bytesReceived);
          if(parent_.stopping_)
          {
            if(joined_)
            {
              // leave the multicast group
              boost::asio::ip::multicast::leave_group leaveRequest(
                multicastGroup_.to_v4(),
                listenInterface_.to_v4());
              socket_.set_option(leaveRequest);
            }
            socket_.close();
          }
        }

        void stop()
        {
          try
          {
            // attempt to cancel any receive requests in progress.
            socket_.cancel();
          }
          catch(...)
          {
          }
        }

        void pause()
        {
          // Temporarily leave the group
          if(joined_)
          {
            boost::asio::ip::multicast::leave_group leaveRequest(
              multicastGroup_.to_v4(),
              listenInterface_.to_v4());
            socket_.set_option(leaveRequest);
            joined_ = false;
          }
        }

        void resume()
        {
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

        boost::asio::ip::address listenInterface()const
        {
          return listenInterface_;
        }

        unsigned short portNumber()const
        {
          return portNumber_;
        }

        boost::asio::ip::address multicastGroup()const
        {
          return multicastGroup_;
        }

        boost::asio::ip::address bindAddress()const
        {
          return bindAddress_;
        }

        boost::asio::ip::udp::endpoint endpoint()const
        {
          return endpoint_;
        }

        boost::asio::ip::udp::endpoint senderEndpoint()const
        {
          return senderEndpoint_;
        }

        boost::asio::ip::udp::socket & socket()
        {
          return socket_;
        }

        bool joined()const
        {
          return joined_;
        }

        bool readInProgress()const
        {
          return readInProgress_;
        }
      private:
        MulticastFeed();
        MulticastFeed(const MulticastFeed &);
        MulticastFeed & operator =(const MulticastFeed &);
      private:
        MulticastReceiver & parent_;
        std::string name_;
        boost::asio::ip::address listenInterface_;
        unsigned short portNumber_;
        boost::asio::ip::address multicastGroup_;
        boost::asio::ip::address bindAddress_;
        boost::asio::ip::udp::endpoint endpoint_;
        boost::asio::ip::udp::endpoint senderEndpoint_;
        boost::asio::ip::udp::socket socket_;
        bool joined_;
        bool readInProgress_;
      };
      typedef boost::shared_ptr<MulticastFeed> MulticastFeedPtr;
      typedef std::vector<MulticastFeedPtr> MulticastFeedVector;

    public:

      /// @brief Construct
      MulticastReceiver()
        : AsynchReceiver()
      {
      }

      /// @brief construct given shared io_service
      MulticastReceiver(boost::asio::io_service & ioService)
        : AsynchReceiver(ioService)
      {
      }

      /// @brief Convenience constructor: Construct and configure a feed
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
      {
        addFeed(
         "default",
          multicastGroupIP,
          listenInterfaceIP,
          bindIP,
          portNumber
        );
      }

      /// @brief Convenience constructor: Construct and configure a feed w/ specific I/O service
      /// @param ioService to be used for this receiver
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
      {
        addFeed(
         "default",
          multicastGroupIP,
          listenInterfaceIP,
          bindIP,
          portNumber
        );
      }

      ~MulticastReceiver()
      {
      }

      /// @brief Add a new feed
      ///
      /// Warning: All feeds must be added before initializeReceiver is called.
      ///
      /// @param name to identitify this feed in display/log messages.
      /// @param multicastGroupIP multicast address as a text string
      /// @param listenInterfaceIP listen address as a text string.
      ///        This identifies the network interface to be used.
      ///        0.0.0.0 means "let the system choose"
      /// @param bindIP the IP to be used to bind the socket.
      /// @param portNumber port number
      void addFeed(
        const std::string & name,
        const std::string & multicastGroupIP,
        const std::string & listenInterfaceIP,
        const std::string & bindIP,
        unsigned short portNumber
        )
      {
        MulticastFeedPtr feed(new MulticastFeed(*this, ioService_, name, multicastGroupIP, listenInterfaceIP, bindIP, portNumber));
        feeds_.push_back(feed);
      }

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        bool ok = true;
        size_t nFeed = 0;
        try
        {
          for(nFeed = 0; ok && nFeed < feeds_.size(); ++nFeed)
          {
            if(assembler_->wantLog(Common::Logger::QF_LOG_INFO))
            {
              std::stringstream msg;
              msg << "Joining multicast group for feed " << feeds_[nFeed]->name()
                << ": " << feeds_[nFeed]->multicastGroup().to_string()
                << " via interface " << feeds_[nFeed]->endpoint().address().to_string()
                << ':' << feeds_[nFeed]->endpoint().port();
              assembler_->logMessage(Common::Logger::QF_LOG_INFO, msg.str());
            }
            ok = feeds_[nFeed]->initializeReceiver();
          }
        }
        catch (const std::exception & exception)
        {
          ok = false;
          std::stringstream msg;
          msg << "Error " << exception.what()
            << " joining multicast group ";
          if(nFeed < feeds_.size())
          {
            msg << " for feed " << feeds_[nFeed]->name()
                << ": " << feeds_[nFeed]->multicastGroup().to_string()
                << " via interface " << feeds_[nFeed]->endpoint().address().to_string()
                << ':' << feeds_[nFeed]->endpoint().port();
          }
          assembler_->logMessage(Common::Logger::QF_LOG_SERIOUS, msg.str());
        }
        if(!ok)
        {
          for(size_t nFeed = 0; nFeed < feeds_.size(); ++nFeed)
          feeds_[nFeed]->stop();
        }
        return ok;
      }

      virtual void stop()
      {
        // stop processing buffers first
        AsynchReceiver::pause();
        for(size_t nFeed = 0; nFeed < feeds_.size(); ++nFeed)
        {
          feeds_[nFeed]->stop();
        }
        // and then shut everything down for good.
        AsynchReceiver::stop();
      }

      virtual void pause()
      {
        for(size_t nFeed = 0; nFeed < feeds_.size(); ++nFeed)
        {
          feeds_[nFeed]->pause();
        }
        AsynchReceiver::pause();
      }

      virtual void resume()
      {
        AsynchReceiver::resume();
        for(size_t nFeed = 0; nFeed < feeds_.size(); ++nFeed)
        {
          feeds_[nFeed]->resume();
        }
      }

    private:
      bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock & lock)
      {
        // consider fairness/round robin
        for(size_t nFeed = 0; nFeed < feeds_.size(); ++nFeed)
        {
          if(feeds_[nFeed]->canStartRead())
          {
            return feeds_[nFeed]->fillBuffer(buffer, lock);
          }
        }
//        std::cout << "All " << feeds_.size() << " feed(s) busy. " << std::endl;
        assert(false); // we should never get here
        return false;
      }

      virtual bool canStartRead()
      {
        for(size_t nFeed = 0; nFeed < feeds_.size(); ++nFeed)
        {
          if(feeds_[nFeed]->canStartRead())
          {
//            std::cout << "Can start read on feed " << nFeed << ": " << feeds_[nFeed]->name() << std::endl;
            return true;
          }
        }
        return false;
      }

    private:
      MulticastFeedVector feeds_;
    };
  }
}
#endif // MULTICASTRECEIVER_H
