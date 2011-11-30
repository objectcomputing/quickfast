// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef STREAMINGASSEMBLER_H
#define STREAMINGASSEMBLER_H

#include <Common/QuickFAST_Export.h>

#include <Communication/Assembler.h>
#include <Communication/LinkedBuffer.h>
#include <Codecs/DataSource.h>
#include <Codecs/HeaderAnalyzer.h>
#include <Codecs/Decoder.h>
#include <Codecs/TemplateRegistry_fwd.h>
#include <Messages/ValueMessageBuilder_fwd.h>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Service a Receiver's Queue when expecting streaming data (TCP/IP) with (or without) block headers.
    class QuickFAST_Export StreamingAssembler
      : public Communication::Assembler
      , public Codecs::DataSource
    {
    public:
      /// @brief Constuct the Assembler
      /// @param templateRegistry defines the decoding instructions for the decoder
      /// @param headerAnalyzer analyzes the header of each message (if any)
      /// @param builder receives the data from the decoder.
      /// @param waitForCompleteMessage if true cause decoding to be delayed (without thread blocking)
      ///        until a complete message is available.
      StreamingAssembler(
          TemplateRegistryPtr templateRegistry,
          HeaderAnalyzer & headerAnalyzer,
          Messages::ValueMessageBuilder & builder,
          bool waitForCompleteMessage = false);

      virtual ~StreamingAssembler();

      /// @brief set the maximum number of messages to decode
      /// @param messageLimit is the number of messages to decode
      void setMessageLimit(size_t messageLimit)
      {
        messageLimit_ = messageLimit;
      }

      ///////////////////////////
      // Implement Assembler
      virtual void receiverStarted(Communication::Receiver & receiver);
      virtual void receiverStopped(Communication::Receiver & receiver);
      virtual bool serviceQueue(Communication::Receiver & receiver);

      ///////////////////////
      // Implement DataSource
      //
      virtual bool getBuffer(const uchar *& buffer, size_t & size);
      virtual int messageAvailable();

      /// @brief Access the internal decoder
      /// @returns a reference to the internal decoder
      Codecs::Decoder & decoder()
      {
        return decoder_;
      }

    private:
      StreamingAssembler & operator = (const StreamingAssembler &);
      StreamingAssembler(const StreamingAssembler &);
      StreamingAssembler();

    private:
      HeaderAnalyzer & headerAnalyzer_;
      Messages::ValueMessageBuilder & builder_;
      bool stopping_;
      bool waitForCompleteMessage_;

      // Nonzero during call to consumeBuffer ->decoder
      // zero the rest of the time.
      Communication::Receiver * receiver_;

      // buffer from which data is being pulled
      Communication::LinkedBuffer * currentBuffer_;

      bool headerIsComplete_;
      bool skipBlock_;
      size_t blockSize_;

      bool inDecoder_;

      size_t messageCount_;
      size_t byteCount_;
      size_t messageLimit_;
    };
  }
}
#endif // STREAMINGASSEMBLER_H
