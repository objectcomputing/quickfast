// Copyright (c) 2011, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef ASYNCHFILESENDER_H
#define ASYNCHFILESENDER_H
#include <Common/QuickFAST_Export.h>
#include "AsynchFileSender_fwd.h"
#include <Communication/AsynchSender.h>
//#include <Common/Types.h>
namespace QuickFAST
{
  namespace Communication
  {
#if defined(_WIN32)
    /// @brief Sender that writes data to a fil asynchronously (WINDOWS ONLY)
    class QuickFAST_Export AsynchFileSender: public AsynchSender
    {
    public:
      /// @brief Construct
      /// @param recycler will receive empty buffers after their contents have been written
      /// @param fileName is the name of the file to be written
      /// @param additionalAttributes will be used in the Windows CreateFile call.
      AsynchFileSender(
        BufferRecycler & recycler,
        const char * fileName,
        unsigned long additionalAttributes = 0);

      /// @brief Construct
      /// @param ioService is a shared io_service.
      /// @param recycler will receive empty buffers after their contents have been written
      /// @param fileName is the name of the file to be written
      /// @param additionalAttributes will be used in the Windows CreateFile call.
      AsynchFileSender(
        boost::asio::io_service & ioService,
        BufferRecycler & recycler,
        const char * fileName,
        unsigned long additionalAttributes = 0);

      virtual ~AsynchFileSender();

      // Override Sender method
      virtual void open();
      // Override Sender method
      virtual void send(LinkedBuffer * buffer);

      /// @brief write a buffer to a specific location in the file
      ///
      /// relevant only to file I/O.  Does not make sense on other senders
      /// hence it's not virtual.
      ///
      /// Note that the send method delegates the write to this method after
      /// updating offset_.  This means the records will be written in the order
      /// the arrive at send, which is not necessarily the order in which they
      /// started in a multithreaded environment.  By allowing the caller to
      /// specify an offset, this method give the caller control over the order
      /// of data in the file.
      void sendAt(LinkedBuffer * buffer, long long offset);

      // Override Sender method
      virtual void close();


    private:
      uint32 additionalAttributes_; // consider: FILE_FLAG_NO_BUFFERING
      boost::asio::windows::random_access_handle handle_;
      long long volatile offset_;
    };
#endif // _WIN32
  }
}
#endif // ASYNCHFILESENDER_H
