// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef QUICKFASTLOGGER_H
#define QUICKFASTLOGGER_H
#include "Logger_fwd.h"
namespace QuickFAST{
  namespace Common{
    /// @brief interface to be implemented by a consumer of decoded messages.
    class Logger
    {
    public:
      /// The importance of being a log message.
      typedef unsigned short LogLevel;
      /// @brief an irrecoverable error
      static const unsigned short QF_LOG_FATAL = 0;
      /// @brief a serious error from which we _might_ be able to recover
      static const unsigned short QF_LOG_SERIOUS = 1;
      /// @brief a warning that will not prevent further decoding/encoding
      static const unsigned short QF_LOG_WARNING = 2;
      /// @brief general information
      static const unsigned short QF_LOG_INFO = 3;
      /// @brief wordy messages -- usually to support debugging
      static const unsigned short QF_LOG_VERBOSE = 4;

      /// @brief a virtual destructor for form's sake.
      /// No one should ever delete a pointer to a logger, but just in case:
      virtual ~Logger(){}

      /// @brief Does consumer wish to see logs with the given importance level.
      /// @param level is the importance level. low numbers are more important.
      virtual bool wantLog(LogLevel level) = 0;

      /// @brief report an "interesting" event
      /// @param level is the importance level. low numbers are more important.
      /// @param message describes the event
      /// @returns true if decoding should continue; false to stop decoding
      virtual bool logMessage(LogLevel level, const std::string & message) = 0;

      /// @brief Report an error during the decoding process
      ///
      /// The message consumer should return false unless a recovery mechanism
      /// exists to resynchronize decoding with the input stream.
      /// @param message describes the error
      /// @returns true if decoding should continue; false to stop decoding
      virtual bool reportDecodingError(const std::string & message) = 0;

      /// @brief Report a communication error
      ///
      /// Although a true return will attempt to continue, there is no guarantee
      /// that recovery is possible.
      /// @param message describes the error
      /// @returns true if decoding should continue; false to stop decoding
      virtual bool reportCommunicationError(const std::string & message) = 0;

    };
  }
}
#endif /* QUICKFASTLOGGER_H */
