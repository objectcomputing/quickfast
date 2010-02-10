// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include <Communication/Receiver.h>
#include "MessagePerPacketQueueService.h"
#include <Messages/ValueMessageBuilder.h>
#include <Codecs/Decoder.h>

using namespace QuickFAST;
using namespace Codecs;

MessagePerPacketQueueService::MessagePerPacketQueueService(
      TemplateRegistryPtr templateRegistry,
      Messages::ValueMessageBuilder & builder)
  : Communication::BufferQueueService(builder)
  , builder_(builder)
  , decoder_(templateRegistry)
  , messageCount_(0)
  , byteCount_(0)
  , messageLimit_(0)
{
}

MessagePerPacketQueueService::~MessagePerPacketQueueService()
{
}

bool
MessagePerPacketQueueService::serviceQueue(Communication::Receiver & receiver)
{
  bool result = true;
  Communication::LinkedBuffer * buffer = receiver.getBuffer(false);
  while(result && buffer != 0)
  {
    try
    {
      if(!consumeBuffer(buffer->get(), buffer->used()))
      {
        result = false;
      }
    }
    catch(const std::exception &ex)
    {
      if(!reportDecodingError(ex.what()))
      {
        result = false;
      }
    }
    receiver.releaseBuffer(buffer);
    buffer = 0;
    if(result)
    {
      buffer = receiver.getBuffer(false);
    }
  }
  return result;
}

bool
MessagePerPacketQueueService::consumeBuffer(const unsigned char * buffer, size_t size)
{
  bool result = true;
  ++messageCount_;
  ++byteCount_ += size;
  if(builder_.wantLog(Common::Logger::QF_LOG_VERBOSE))
  {
    std::stringstream message;
    message << "Received[" << messageCount_ << "]: " << size << " bytes";
    builder_.logMessage(Common::Logger::QF_LOG_VERBOSE, message.str());
  }
  try
  {
    buffer_ = buffer;
    size_ = size;
    // Unreliable multicast.  Always reset the decoder
    decoder_.reset();
    while(bytesAvailable() > 0)
    {
      decoder_.decodeMessage(*this, builder_);
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
  return result;
}

void
MessagePerPacketQueueService::receiverStarted(Communication::Receiver & /*receiver*/)
{
  if(builder_.wantLog(Common::Logger::QF_LOG_INFO))
  {
    builder_.logMessage(Common::Logger::QF_LOG_INFO, "Receiver started");
  }
}
void
MessagePerPacketQueueService::receiverStopped(Communication::Receiver & /*receiver*/)
{
  if(builder_.wantLog(Common::Logger::QF_LOG_INFO))
  {
    builder_.logMessage(Common::Logger::QF_LOG_INFO, "Receiver stopped");
  }
}

bool
MessagePerPacketQueueService::getBuffer(const uchar *& buffer, size_t & size)
{
  bool result = size_ > 0;
  buffer = buffer_;
  size = size_;
  size_ = 0;
  return result;
}

