// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGECONSUMER_H
#define MESSAGECONSUMER_H
#include "MessageConsumer_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Messages/DecodedFields_fwd.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief interface to be implemented by a consumer of decoded messages.
    class MessageConsumer
    {
    public:
      /// The importance of being a log message.
      enum LogLevels
      {
        LOG_FATAL = 0,
        LOG_SERIOUS = 1,
        LOG_WARNING = 2,
        LOG_INFO = 3,
        LOG_VERBOSE = 4
      };
      virtual ~MessageConsumer(){}
      /// @brief Accept a decoded message
      /// @param message is the decoded message, valid for the life of this call.
      /// @returns true if decoding should continue; false to stop decoding
      virtual bool consumeMessage(Messages::DecodedFields & message) = 0;

      /// @brief Does consumer wish to see logs with the given importance level.
      /// @param level is the importance level. low numbers are more important.
      virtual bool wantLog(unsigned short level) = 0;

      /// @brief report an "interesting" event
      /// @param level is the importance level. low numbers are more important.
      /// @param logMessage describes the event
      /// @returns true if decoding should continue; false to stop decoding
      virtual bool logMessage(unsigned short level, const std::string & logMessage) = 0;

      /// @brief Report an error during the decoding process
      ///
      /// The message consumer should return false unless a recovery mechanism
      /// exists to resynchronize decoding with the input stream.
      /// @param errorMessage describes the error
      /// @returns true if decoding should continue; false to stop decoding
      virtual bool reportDecodingError(const std::string & errorMessage) = 0;

      /// @brief Report a communication error
      ///
      /// Although a true return will attempt to continue, there is no guarantee
      /// that recovery is possible.
      /// @param errorMessage describes the error
      /// @returns true if decoding should continue; false to stop decoding
      virtual bool reportCommunicationError(const std::string & errorMessage) = 0;

      /// @brief notify consumer that decoding is stopped.
      /// No calls to consumeMessage will be generated after this call.
      virtual void decodingStopped() = 0;
    };
  }
}
#endif /* MESSAGECONSUMER_H */
