// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "Assembler_fwd.h"
//#include <Common/QuickFAST_Export.h>
#include <Communication/Receiver_fwd.h>
#include <Codecs/TemplateRegistry_fwd.h>
#include <Codecs/Decoder.h>
#include <Communication/LinkedBuffer.h>
#include <Common/Logger.h>

namespace QuickFAST{
  namespace Communication
  {
    /// @brief A companion class to service the queue in Communication::Receiver
    ///
    /// The Receiver is responsible for filling buffers, adding them to the service queue
    /// then requesting queue service from an implementation of this interface.
    /// The implementation of this interface retrieves the buffers from the receiver and processes
    /// them as necessary.
    class Assembler : public Common::Logger
    {
    public:
      /// @brief Construct
      /// @param templateRegistry contains the templates to be used during decoding
      /// @param logger is used to log requests to a "real" logger
      Assembler(
          Codecs::TemplateRegistryPtr templateRegistry,
          Common::Logger & logger)
        : decoder_(templateRegistry)
        , logger_(logger)
        , strict_(true)
        , reset_(false)
      {
      }

      virtual ~Assembler(){}

      /// @brief notify queue service that receiver is started
      /// Will be called once before serviceQueue is called
      /// @param receiver identifies the Receiver of incoming buffers.
      virtual void receiverStarted(Receiver & receiver) = 0;

      /// @brief notify queue service that receiver is stopped
      /// @param receiver in case the consumer needs to release buffers.
      /// No calls to serviceQueue will be generated after this call.
      /// @param receiver identifies the Receiver of incoming buffers.
      virtual void receiverStopped(Receiver & receiver) = 0;

      /// @brief Service the queue via Receiver callbacks
      /// @param receiver contains the queue to be serviced.
      /// @returns true if receiving should continue; false to stop receiving
      virtual bool serviceQueue(Receiver & receiver) = 0;

      ///////////////////
      // Implement Logger
      virtual bool wantLog(unsigned short level)
      {
        return logger_.wantLog(level);
      }

      virtual bool logMessage(unsigned short level, const std::string & logMessage)
      {
        return logger_.logMessage(level, logMessage);
      }

      virtual bool reportDecodingError(const std::string & errorMessage)
      {
        return logger_.reportDecodingError(errorMessage);
      }

      virtual bool reportCommunicationError(const std::string & errorMessage)
      {
        return logger_.reportCommunicationError(errorMessage);
      }

      /// @brief Set flag to reset the decoder on every message.
      /// @param reset is true to enable message-level reset
      void setReset(bool reset = true)
      {
        reset_ = reset;
      }

      /// @brief set the flag to use strict decoding rules
      /// @param strict is true to enable strict decoding
      void setStrict(bool strict = true)
      {
        strict_ = strict;
      }

      /// @brief Provide direct access to the decoder.
      Codecs::Decoder & decoder()
      {
        return decoder_;
      }

    protected:
      /// The decoder that does the work.
      Codecs::Decoder decoder_;

      /// Log information is delegated to this logger
      Common::Logger & logger_;
      /// Strict decoding rules
      bool strict_;
      /// Reset the decoder for every message
      bool reset_;


    };
  }
}
#endif /* ASSEMBLER_H */
