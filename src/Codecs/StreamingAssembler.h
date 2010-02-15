// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef STREAMINGASSEMBLER_H
#define STREAMINGASSEMBLER_H

#include <Common/QuickFAST_Export.h>

#include <Communication/IncomingBufferAssembler.h>
#include <Communication/LinkedBuffer.h>
#include <Codecs/DataSource.h>
#include <Codecs/SynchronousDecoder.h>
#include <Codecs/TemplateRegistry_fwd.h>
#include <Messages/ValueMessageBuilder_fwd.h>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Service a Receiver's Queue when expecting streaming data (TCP/IP) with (or without) block headers.
    class QuickFAST_Export StreamingAssembler
      : public Communication::IncomingBufferAssembler
      , public Codecs::DataSource
    {
    public:
      /// @brief Expected type of header
      enum HeaderType {
        HEADER_FIXED,
        HEADER_FAST,
        HEADER_NONE};

    public:
      /// @brief Construct given the registry to be used by the decoder and the builder to receive data
      /// @param templateRegistry defines the messages to be decoded
      /// @param builder accepts data from the decoder for use by the application
      StreamingAssembler(
          TemplateRegistryPtr templateRegistry,
          Messages::ValueMessageBuilder & builder);

      virtual ~StreamingAssembler();


      /// @brief Define the header expected on each message
      /// @param type of header expected.
      /// @param prefix what comes before the message size field( (HEADER_FIXED: byte count, HEADER_FAST: field count)
      /// @param size number of bytes in message size field (HEADER_FIXED only)
      /// @param swap endian swap the size field (HEADER_FIXED only
      /// @param suffix what comes after the message size field( (HEADER_FIXED: byte count, HEADER_FAST: field count)
      void setHeader(
        HeaderType type,
        size_t prefix = 0,
        size_t size = 0,
        bool swap = false,
        size_t suffix = 0)
      {
        headerType_ = type;
        headerPrefix_ = prefix;
        blockSizeBytes_ = size;
        blockSizeSwap_ = swap;
        headerSuffix_ = suffix;
      }

      ///////////////////////////
      // Implement IncomingBufferAssembler
      virtual void receiverStarted(Communication::Receiver & receiver);
      virtual void receiverStopped(Communication::Receiver & receiver);
      virtual bool serviceQueue(Communication::Receiver & receiver);

      ///////////////////////
      // Implement DataSource
      //
      virtual bool getBuffer(const uchar *& buffer, size_t & size);
      virtual int messageAvailable();

    private:
      StreamingAssembler & operator = (const StreamingAssembler &);
      StreamingAssembler(const StreamingAssembler &);
      StreamingAssembler();

    private:
      Messages::ValueMessageBuilder & builder_;
      bool stopping_;

      /////////////////////////////////////
      // describe the block header (if any)

      HeaderType headerType_;
      // header prefix:
      // for fixed size headers: how many bytes preceed the message size field in the header
      // for FAST encoded headers: how many "simple" FAST fields proceed the message size (stop bit terminated)
      size_t headerPrefix_;

      // message size
      // for fixed size headers only: how many bytes in header
      size_t blockSizeBytes_;
      // for fixed size headers only: endian-swap message size?
      bool blockSizeSwap_;

      // header suffix:
      // for fixed size headers: how many bytes follow the message size field in the header
      // for FAST encoded headers: how many "simple" FAST fields follow the message size (stop bit terminated)
      size_t headerSuffix_;
      // end of block header description
      //////////////////////////////////

      Decoder decoder_;

      // Nonzero during call to consumeBuffer ->decoder
      // zero the rest of the time.
      Communication::Receiver * receiver_;

      // buffer from which data is being pulled
      Communication::LinkedBuffer * currentBuffer_;
      size_t pos_;

      // Message size extracted from header
      bool blockSizeIsValid_;
      bool parsingBlockSize_;
      size_t blockSize_;
      size_t headerPos_;
      bool inDecoder_;
      // end of busy_ protection
      //////////////////////////
    };
  }
}
#endif // STREAMINGASSEMBLER_H
