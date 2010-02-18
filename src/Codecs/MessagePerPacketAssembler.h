// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MESSAGEPERPACKETASSEMBLER_H
#define MESSAGEPERPACKETASSEMBLER_H

#include "MessagePerPacketAssembler_fwd.h"
#include <Common/QuickFAST_Export.h>

#include <Communication/Assembler.h>
#include <Codecs/Decoder.h>
#include <Codecs/DataSource.h>
#include <Codecs/HeaderAnalyzer.h>
#include <Codecs/TemplateRegistry_fwd.h>
#include <Messages/ValueMessageBuilder_fwd.h>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Service a Receiver's Queue when expecting packet boundaries to match message boundaries (UDP or Multicast)
    /// with (or without) block headers.
    class QuickFAST_Export MessagePerPacketAssembler
      : public Communication::Assembler
      , public Codecs::DataSource
    {
    public:
      /// @brief Constuct the Assembler
      /// @param templateRegistry defines the decoding instructions for the decoder
      /// @param analyzer analyzes the header of each packet (if any)
      /// @param builder receives the data from the decoder.
      MessagePerPacketAssembler(
          TemplateRegistryPtr templateRegistry,
          HeaderAnalyzer & headerAnalyzer,
          Messages::ValueMessageBuilder & builder);

      virtual ~MessagePerPacketAssembler();

      /// @brief set the maximum number of messages to decode
      /// @param messageLimit is the number of messages to decode
      void setMessageLimit(size_t messageLimit)
      {
        messageLimit_ = messageLimit;
      }

      /// @brief Access the internal decoder
      /// @returns a reference to the internal decoder
      Codecs::Decoder & decoder()
      {
        return decoder_;
      }

      /// @brief Access the internal decoder (const)
      /// @returns a const reference to the internal decoder
      const Codecs::Decoder & decoder() const
      {
        return decoder_;
      }

      /// @brief How many messages have been processed
      /// @returns the message count
      size_t messageCount()const
      {
        return messageCount_;
      }

      /// @brief How many bytes have been processed
      /// @returns the byte count
      size_t byteCount()const
      {
        return byteCount_;
      }

      ///////////////////////////
      // Implement Assembler
      virtual void receiverStarted(Communication::Receiver & receiver);
      virtual void receiverStopped(Communication::Receiver & receiver);
      virtual bool serviceQueue(Communication::Receiver & receiver);

      ///////////////////////
      // Implement DataSource
      virtual bool getBuffer(const uchar *& buffer, size_t & size);

    private:
      bool consumeBuffer(const unsigned char * buffer, size_t size);
    private:
      MessagePerPacketAssembler & operator = (const MessagePerPacketAssembler &);
      MessagePerPacketAssembler(const MessagePerPacketAssembler &);
      MessagePerPacketAssembler();

    private:
      HeaderAnalyzer & headerAnalyzer_;
      Messages::ValueMessageBuilder & builder_;
      Decoder decoder_;

      const unsigned char * buffer_;
      size_t size_;

      size_t messageCount_;
      size_t byteCount_;
      size_t messageLimit_;
    };
  }
}
#endif // MESSAGEPERPACKETASSEMBLER_H
