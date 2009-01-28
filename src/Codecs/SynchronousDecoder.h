// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef SYNCHRONOUSDECODER_H
#define SYNCHRONOUSDECODER_H

#include <Common/QuickFAST_Export.h>
#include <Codecs/Decoder.h>
#include <Codecs/MessageConsumer_fwd.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief Support Synchronous (blocking) decoding of a FAST data stream
    class QuickFAST_Export SynchronousDecoder
    {
    public:
      /// @brief Construct given a template registry
      /// @param templateRegistry contains the templates to be used during decoding.
      explicit SynchronousDecoder(TemplateRegistryPtr templateRegistry);
      ~SynchronousDecoder();

      /// @brief Set the flag to reset the decoder for every message
      ///
      /// Some data streams reset the Xcoder context at the beginning of each message
      /// This is particulary true when the data stream uses a datagram-per-message
      /// approach (UDP and Unreliable Multicast).
      /// @param reset true if the decoder should be reset for each message; default false
      void setResetOnMessage(bool reset);

      /// @brief Enable some debugging/diagnostic information to be written to an ostream
      /// @param out the ostream to receive the data
      void setVerboseOutput(std::ostream & out);

      /// @brief Clear the verbose output that was enabled by setVerboseOutput()
      void disableVerboseOutput();

      /// @brief Enable/disable strict checking of conformance to the FAST standard
      ///
      /// If strict is false some conditions such as integer overflow or inefficient encoding
      /// will be ignored.  The default is true -- strict checking is enabled.
      /// @param strict true to enable; false to disable strict checking
      void setStrict(bool strict)
      {
        decoder_.setStrict(strict);
      }

      /// @brief get the current status of the strict property.
      /// @returns true if strict checking is enabled.
      bool strict()const
      {
        return decoder_.strict();
      }

      /// @brief set an upper limit on the # of messages to be decoded
      /// @param messageCountLimit how many messages to decode (0 means "forever")
      void setLimit(size_t messageCountLimit)
      {
        messageCountLimit_ = messageCountLimit;
      }

      /// @brief How many messages have been decoded.
      /// @returns the number of messages that have been decoded.
      size_t messageCount() const
      {
        return messageCount_;
      }

      /// @brief Run the decoding process
      ///
      /// Runs until the DataSource reports end of data
      /// or the consumer returns false,
      /// or messageCount() messages have been decoded.
      void decode(
        DataSource & source,
        MessageConsumer & consumer);

    private:
      SynchronousDecoder();
      SynchronousDecoder(const SynchronousDecoder &);
      SynchronousDecoder & operator =(const SynchronousDecoder &);
    private:
      Decoder decoder_;
      bool resetOnMessage_;
      size_t messageCount_;
      size_t messageCountLimit_;
      size_t maxFieldCount_;
    };
  }
}

#endif /* SYNCHRONOUSDECODER_H */
