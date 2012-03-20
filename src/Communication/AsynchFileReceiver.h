// Copyright (c) 2011, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ASYNCHFILERECEIVER_H
#define ASYNCHFILERECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "AsynchFileReceiver_fwd.h"
#include <Communication/AsynchReceiver.h>
#include <Common/Types.h>
#include <Common/Exceptions.h>
namespace QuickFAST
{
  namespace Communication
  {
#if defined(_WIN32)
    /// @brief Read stream of FAST-encoded data from a file using asynchronous I/O
    class AsynchFileReceiver
      : public AsynchReceiver
    {
    public:
      /// @brief Construct given file name, and extra attributes
      /// @param fileName the file to read
      /// @param additionalAttributes for use in the Windows CreateFile function
      AsynchFileReceiver(
        const std::string & fileName,
        uint32 additionalAttributes = 0
        )
        : AsynchReceiver()
        , fileName_(fileName)
        , additionalAttributes_(additionalAttributes)
        , handle_(ioService_)
        , offset_(0LL)
      {
      }


      /// @brief Construct given ioservice, file name, and extra attributes
      /// @param ioService an ioService to be shared with other objects
      /// @param fileName the file to read
      /// @param additionalAttributes for use in the Windows CreateFile function
      AsynchFileReceiver(
        boost::asio::io_service & ioService,
        const std::string & fileName,
        uint32 additionalAttributes = 0
        )
        : AsynchReceiver(ioService)
        , fileName_(fileName)
        , additionalAttributes_(additionalAttributes)
        , handle_(ioService_)
        , offset_(0LL)
      {
      }

      ~AsynchFileReceiver()
      {
        if(handle_.is_open())
        {
          handle_.cancel();
          handle_.close();
        }
      }

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        if(assembler_->wantLog(Common::Logger::QF_LOG_INFO))
        {
          std::stringstream msg;
          msg << "Opening file: " << fileName_;
          assembler_->logMessage(Common::Logger::QF_LOG_INFO, msg.str());
        }
        HANDLE handle = ::CreateFile(
          fileName_.c_str(),
          GENERIC_READ,
          FILE_SHARE_READ,
          NULL,
          OPEN_EXISTING,
          FILE_FLAG_OVERLAPPED |  FILE_FLAG_SEQUENTIAL_SCAN | additionalAttributes_,
          NULL);

        if(handle == INVALID_HANDLE_VALUE)
        {
          return false;
        }
        handle_.assign(handle);
        return true;
      }

      // Implement Receiver method
      virtual void close()
      {
        if(handle_.is_open())
        {
          handle_.cancel();
          handle_.close();
        }
      }

      // Implement Receiver method
      virtual void stop()
      {
        close();

        // and then shut everything down for good.
        Receiver::stop();
      }

    private:

      void handleReceiveFromFile(
        const boost::system::error_code& error,
        LinkedBuffer * buffer,
        size_t bytesReceived)
      {
//        std::cout << "Read " << bytesReceived << " bytes into buffer " << (void *)(buffer->get()) << '[' << buffer->capacity() << ']' << std::endl;
        if(error == boost::asio::error::eof)
        {
          //std::ostringstream msg;
          //msg << fileName_ << " EOF: calling close" << std::endl;
          //std::cout << msg.str() << std::flush;
          idleBufferPool_.push(buffer);
          close();
          // Note that by not calling handle receive we avoid starting a new I/O request
          // thereby allowing the io_service to go idle eventually.
        }
        else
        {
          //std::ostringstream msg;
          //msg << fileName_ << " received " << bytesReceived << " @" << (void *) buffer << std::endl;
          //std::cout << msg.str() << std::flush;
          offset_ += bytesReceived;
          handleReceive(error, buffer, bytesReceived);
        }
      }

      bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock& lock)
      {
        //std::ostringstream msg;
        //msg << fileName_ << " read. @" << (void *) buffer << '[' << buffer->capacity() << ']' << std::endl;
        //std::cout << msg.str() << std::flush;

        handle_.async_read_some_at(
          offset_,
          boost::asio::buffer(buffer->get(), buffer->capacity()),
          boost::bind(&AsynchFileReceiver::handleReceiveFromFile,
            this,
            boost::asio::placeholders::error,
            buffer,
            boost::asio::placeholders::bytes_transferred)
          );
        return true;
      }

    private:
      std::string fileName_;
      uint32 additionalAttributes_; // consider: FILE_FLAG_NO_BUFFERING
      boost::asio::windows::random_access_handle handle_;
      boost::uint64_t offset_;
    };

#else // not _WIN32
    /// @brief Read stream of FAST-encoded data from a file using asynchronous I/O
    class AsynchFileReceiver
      : public AsynchReceiver
    {
    public:
      /// @brief Construct given file name, and extra attributes
      /// @param fileName the file to read
      /// @param additionalAttributes for use in the Windows CreateFile function
      AsynchFileReceiver(
        const std::string & fileName,
        uint32 additionalAttributes = 0
        )
        : AsynchReceiver()
      {
        throw UsageError("Platform Error", "Asynchronous File I/O is only supported on Windows.");
      }

      /// @brief Construct given ioservice, file name, and extra attributes
      /// @param ioService an ioService to be shared with other objects
      /// @param fileName the file to read
      /// @param additionalAttributes for use in the Windows CreateFile function
      AsynchFileReceiver(
        boost::asio::io_service & ioService,
        const std::string & fileName,
        uint32 additionalAttributes = 0
        )
        : AsynchReceiver()
      {
        throw UsageError("Platform Error", "Asynchronous File I/O is only supported on Windows.");
      }


      ~AsynchFileReceiver()
      {
      }

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        return false;
      }

    private:

      bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock& lock)
      {
        return false;
      }
    };

#endif // _WIN32
  }
}
#endif // ASYNCHFILERECEIVER_H
