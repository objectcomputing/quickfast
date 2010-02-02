// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MESSAGEPERPACKETBUFFERCONSUMER_H
#define MESSAGEPERPACKETBUFFERCONSUMER_H

#include <Communication/BufferConsumer.h>
#include <Codecs/Decoder_fwd.h>
#include <Messages/ValueMessageBuilder_fwd.h>

namespace QuickFAST
{
  namespace Codecs
  {
    class MessagePerPacketBufferConsumer : public Communication::BufferConsumer
    {
    public:
      MessagePerPacketBufferConsumer(
          Messages::ValueMessageBuilder & builder,
          Decoder & decoder,
          size_t & messageCount,
          size_t & messageLimit);

      virtual ~MessagePerPacketBufferConsumer();

      ///////////////////////////
      // Implement BufferConsumer
      virtual bool consumeBuffer(Communication::BufferGenerator & generator, Communication::LinkedBuffer * buffer);
      virtual void receiverStarted();
      virtual void receiverStopped();

      ///////////////////
      // Implement Logger
      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);

    private:
      MessagePerPacketBufferConsumer & operator = (const MessagePerPacketBufferConsumer &);
      MessagePerPacketBufferConsumer(const MessagePerPacketBufferConsumer &);
      MessagePerPacketBufferConsumer();

    private:
      Messages::ValueMessageBuilder & builder_;
      Decoder & decoder_;
      size_t & messageLimit_;
      size_t & messageCount_;
    };
  }
}
#endif MESSAGEPERPACKETBUFFERCONSUMER_H
