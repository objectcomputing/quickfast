// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef BASEPACKETASSEMBLER_H
#define BASEPACKETASSEMBLER_H

#include "BasePacketAssembler_fwd.h"
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
    class QuickFAST_Export BasePacketAssembler
      : public Communication::Assembler
      , public DataSource
    {
    public:
      /// @brief Constuct the Assembler
      /// @param templateRegistry defines the decoding instructions for the decoder
      /// @param packetHeaderAnalyzer analyzes the header of each packet (if any)
      /// @param messageHeaderAnalyzer analyzes the header of each message (if any)
      /// @param builder receives the data from the decoder.
      BasePacketAssembler(
          TemplateRegistryPtr templateRegistry,
          HeaderAnalyzer & packetHeaderAnalyzer,
          HeaderAnalyzer & messageHeaderAnalyzer,
          Messages::ValueMessageBuilder & builder);

      virtual ~BasePacketAssembler();

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
      // Derived classes must implement serviceQueue
      // virtual bool serviceQueue(Communication::Receiver & receiver) = 0;

      ///////////////////////
      // Implement DataSource
      virtual bool getBuffer(const uchar *& buffer, size_t & size);

    protected:
      /// @brief Decode the contents of a memory buffer
      /// @param buffer points to the data.
      /// @param size is how many valid bytes of data are at *buffer.
      bool decodeBuffer(const unsigned char * buffer, size_t size);

    private:
      BasePacketAssembler & operator = (const BasePacketAssembler &);
      BasePacketAssembler(const BasePacketAssembler &);
      BasePacketAssembler();

    protected:
      /// Analyze the packet header.
      HeaderAnalyzer & packetHeaderAnalyzer_;
      /// Analyze the message header.
      HeaderAnalyzer & messageHeaderAnalyzer_;
      /// Receive the decoded information.
      Messages::ValueMessageBuilder & builder_;

      /// the current buffer being decoded.
      const unsigned char * currentBuffer_;
      /// the size of the current buffer.
      size_t currentSize_;

      /// How many buffers have been decoded.
      size_t messageCount_;
      /// How many bytes were in those buffers.
      size_t byteCount_;
      /// How many buffers should be decoded before stopping artificially.
      size_t messageLimit_;
    };

  }
}
#endif // BASEPACKETASSEMBLER_H
