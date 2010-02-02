// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include <Communication/BufferGenerator.h>
#include "MessagePerPacketBufferConsumer.h"
#include <Messages/ValueMessageBuilder.h>
#include <Codecs/DataSourceBuffer.h>
#include <Codecs/Decoder.h>

using namespace QuickFAST;
using namespace Codecs;

MessagePerPacketBufferConsumer::MessagePerPacketBufferConsumer(
    Messages::ValueMessageBuilder & builder,
    Decoder & decoder,
    size_t & messageCount,
    size_t & messageLimit)
  : builder_(builder)
  , decoder_(decoder)
  , messageLimit_(messageLimit)
  , messageCount_(messageCount)
{
  messageCount_ = 0;
}

MessagePerPacketBufferConsumer::~MessagePerPacketBufferConsumer()
{
}

bool
MessagePerPacketBufferConsumer::consumeBuffer(Communication::BufferGenerator & generator, Communication::LinkedBuffer * buffer)
{
  bool result = true;
  ++messageCount_;
  if(builder_.wantLog(Common::Logger::QF_LOG_VERBOSE))
  {
    std::stringstream message;
    message << "Received[" << messageCount_ << "]: " << buffer->used() << " bytes";
    builder_.logMessage(Common::Logger::QF_LOG_VERBOSE, message.str());
  }
  try
  {
    DataSourceBuffer source(buffer->get(), buffer->used());
    // Unreliable multicast.  Always reset the decoder
    decoder_.reset();
    while(source.bytesAvailable() > 0)
    {
      decoder_.decodeMessage(source, builder_);
    }
  }
  catch (const std::exception &ex)
  {
    result = builder_.reportDecodingError(ex.what());
  }
  if(result && messageCount_ > messageLimit_ && messageLimit_ != 0)
  {
    result = false;
  }
  generator.releaseBuffer(buffer);
  return result;
}

void
MessagePerPacketBufferConsumer::receiverStarted()
{
  /// todo Log?
}
void
MessagePerPacketBufferConsumer::receiverStopped()
{
  /// todo log?
}

bool
MessagePerPacketBufferConsumer::wantLog(unsigned short level)
{
  return builder_.wantLog(level);
}

bool
MessagePerPacketBufferConsumer::logMessage(unsigned short level, const std::string & logMessage)
{
  return builder_.logMessage(level, logMessage);
}

bool
MessagePerPacketBufferConsumer::reportDecodingError(const std::string & errorMessage)
{
  return builder_.reportDecodingError(errorMessage);
}

bool
MessagePerPacketBufferConsumer::reportCommunicationError(const std::string & errorMessage)
{
  return builder_.reportCommunicationError(errorMessage);
}

