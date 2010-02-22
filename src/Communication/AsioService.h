// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef ASIOSERVICE_H
#define ASIOSERVICE_H
#include <Common/QuickFAST_Export.h>
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

      /// @brief stop the ioservice
      void stopService();

      /// @brief allow implicit cast to io_service
      operator boost::asio::io_service &()
      {
        return ioService_;
      }

    private:
      static boost::asio::io_service privateIoService_;
      /// Pointer to a boost thread
      typedef boost::shared_ptr<boost::thread> ThreadPtr;
      bool stopping_;
      boost::scoped_array<ThreadPtr> threads_;
      size_t threadCount_;
      size_t threadCapacity_;
    protected:
      /// Protected reference to the io_service.
      boost::asio::io_service & ioService_;

    };
  }
}
#endif // ASIOSERVICE_H
