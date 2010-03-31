// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#pragma unmanaged
#include <Common/Logger.h>
#pragma managed

namespace QuickFAST
{
  namespace DotNet
  {
    class ImplFieldSet;

    /// Expose log levels to .NET
    enum LogLevel
    {
      QF_LOG_FATAL = Common::Logger::QF_LOG_FATAL,
      QF_LOG_SERIOUS = Common::Logger::QF_LOG_SERIOUS,
      QF_LOG_WARNING = Common::Logger::QF_LOG_WARNING,
      QF_LOG_INFO = Common::Logger::QF_LOG_INFO,
      QF_LOG_VERBOSE = Common::Logger::QF_LOG_VERBOSE
    };
    ref class DNMessageDeliverer;
    /// @brief link from unmanaged code to the managed DNMessageDeliverer
    class ImplMessageDeliverer
    {
    public:
      /// @brief construct with reference to the "real" deliverer
      ImplMessageDeliverer(DNMessageDeliverer ^ deliverer);
      ~ImplMessageDeliverer();
      /// @brief deliver a completed message (represented as a field set) to the application
      /// @param fieldSet is the message to be delivered.
      bool deliverMessage(ImplFieldSet & fieldSet);

      /// @brief report an "interesting" event
      /// @param level is the importance level. low numbers are more important.
      /// @param logMessage describes the event
      /// @returns true if decoding should continue; false to stop decoding
      bool logMessage(unsigned short level, const std::string & logMessage);

      /// @brief Report an error during the decoding process
      ///
      /// The message consumer should return false unless a recovery mechanism
      /// exists to resynchronize decoding with the input stream.
      /// @param errorMessage describes the error
      /// @returns true if decoding should continue; false to stop decoding
      bool reportDecodingError(const std::string & errorMessage);

      /// @brief Report a communication error
      ///
      /// Although a true return will attempt to continue, there is no guarantee
      /// that recovery is possible.
      /// @param errorMessage describes the error
      /// @returns true if decoding should continue; false to stop decoding
      bool reportCommunicationError(const std::string & errorMessage);

    private:
      gcroot<DNMessageDeliverer ^> deliverer_;
    };
  }
}

