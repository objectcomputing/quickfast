// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ASIOSERVICE_H
#define ASIOSERVICE_H
#include "AsioService_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Common/Logger_fwd.h>
#include <Common/AtomicCounter.h>

// In gcc including asio.hpp in precompiled headers causes problems
#include <boost/asio.hpp>

namespace QuickFAST
{
  namespace Communication
  {
    /// @brief Base class to allow sharing a boost::io_service
    ///
    /// Normal case is for all classes derived from AsioService to share
    /// the same boost::io_service.  The alternate constructor gives the
    /// application more control if it is needed.
    class QuickFAST_Export AsioService
    {
    public:
      /// @brief Construct using the internal, common io service
      AsioService();

      /// @brief Construct using an external io service
      AsioService(boost::asio::io_service & ioService);

      ~AsioService();

      /// @brief define the logger to receive informative messages
      /// @param logger to which messages will be written
      void setLogger(Common::Logger & logger);

      /// @brief Run the event loop with this threads and threadCount additional threads.
      void runThreads(size_t threadCount = 0, bool useThisThread = true);

      /// @brief run the event loop in this thread
      ///
      /// Exceptions are caught, logged, and ignored.  The event loop continues.
      void run();

      /// @brief run the event loop until one event is handled.
      void run_one()
      {
        ioService_.run_one();
      }

      /// @brief execute any ready event handlers than return.
      size_t poll()
      {
        return ioService_.poll();
      }

      /// @brief execute at most one ready event handler than return.
      size_t poll_one()
      {
        return ioService_.poll_one();
      }

      /// @brief Allow external access (thereby blowing encapsulization)
      boost::asio::io_service & ioService()
      {
        return ioService_;
      }

      /// @brief create additional threads to run the event loop
      void startThreads(size_t threadCount)
      {
        runThreads(threadCount, false);
      }

      /// @brief join all additional threads after calling stopService()
      ///
      /// If stopService() has not been called, this will block "forever".
      void joinThreads();

      /// @brief reset the IO service
      ///
      /// should be called after joinThreads before calling run*, poll*, etc. again.
      void resetService()
      {
        ioService_.reset();
        stopping_ = false;
      }

      /// @brief stop the ioservice
      void stopService();

      /// @brief allow implicit cast to io_service
      operator boost::asio::io_service &()
      {
        return ioService_;
      }

      ///@brief Post a completion handler for later processing (usually in a different thread)
      /// @param handler is the handler to be posted
      template<typename CompletionHandler>
      void post(CompletionHandler handler)
      {
        ioService_.post(handler);
      }

      /// @brief Attempt to determine how many threads are available to ASIO
      /// @returns the number of threads.
      long runningThreadCount()const
      {
        if(usingSharedService_)
        {
          return sharedRunningThreadCount_;
        }
        return runningThreadCount_;
      }

    private:
      // if no io_service is specified, this one
      // will be used (shared among all users)
      static boost::asio::io_service sharedIoService_;
      static AtomicCounter sharedRunningThreadCount_;

      /// Pointer to a boost thread
      typedef boost::shared_ptr<boost::thread> ThreadPtr;
      bool stopping_;
      boost::scoped_array<ThreadPtr> threads_;
      size_t threadCount_;
      size_t threadCapacity_;

      AtomicCounter runningThreadCount_;
      boost::asio::io_service & ioService_;
      bool usingSharedService_;
      Common::Logger * logger_;
    };
  }
}
#endif // ASIOSERVICE_H
