// Copyright (c) 2011, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef ASYNCHSENDER_H
#define ASYNCHSENDER_H
#include <Common/QuickFAST_Export.h>
#include "AsynchSender_fwd.h"
#include <Communication/Sender.h>
#include <Communication/AsioService.h>
#include <Common/Types.h>
namespace QuickFAST
{
  namespace Communication
  {
    /// @brief Sender base class for sending outgoing data
    class QuickFAST_Export AsynchSender: public Sender
    {
    public:
      AsynchSender(
        BufferRecycler & recycler,
        const char * name);

      AsynchSender(
        boost::asio::io_service & ioService,
        BufferRecycler & recycler,
        const char * name);

      virtual ~AsynchSender();

      // Override Sender method
      virtual void stop();

      // Override Sender method
      virtual void close();

      virtual void runThreads(size_t threadCount = 0, bool useThisThread = true)
      {
        ioService_.runThreads(threadCount, useThisThread);
      }

      virtual void joinThreads()
      {
        ioService_.joinThreads();
      }

      virtual void resetService()
      {
        ioService_.resetService();
      }

      virtual void run()
      {
        ioService_.run();
      }

      virtual void run_one()
      {
        ioService_.run_one();
      }

      virtual size_t poll()
      {
        return ioService_.poll();
      }

      virtual size_t poll_one()
      {
        return ioService_.poll_one();
      }
    protected:
      void handleWrite(
        const boost::system::error_code& error,
        LinkedBuffer * buffer,
        size_t bytesWritten);

    protected:
      std::string name_;
      AsioService ioService_;
      boost::scoped_ptr<boost::asio::io_service::work> keepAlive_;
    };
  }
}
#endif // ASYNCHFILESENDER_H
