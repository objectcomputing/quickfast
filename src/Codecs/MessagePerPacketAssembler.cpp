// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include <Communication/Receiver.h>
#include "MessagePerPacketAssembler.h"
#include <Messages/ValueMessageBuilder.h>
#include <Codecs/Decoder.h>

using namespace QuickFAST;
using namespace Codecs;

MessagePerPacketAssembler::MessagePerPacketAssembler(
      TemplateRegistryPtr templateRegistry,
      HeaderAnalyzer & headerAnalyzer,
      Messages::ValueMessageBuilder & builder)
  : Communication::Assembler(templateRegistry, builder)
  , headerAnalyzer_(headerAnalyzer)
  , builder_(builder)
  , messageCount_(0)
  , byteCount_(0)
  , messageLimit_(0)
{
}

MessagePerPacketAssembler::~MessagePerPacketAssembler()
{
}

bool
MessagePerPacketAssembler::serviceQueue(Communication::Receiver & receiver)
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
MessagePerPacketAssembler::consumeBuffer(const unsigned char * buffer, size_t size)
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
    size_t blockSize = 0;
    bool skipBlock = false;
    if(!headerAnalyzer_.analyzeHeader(*this, blockSize, skipBlock))
    {
      builder_.reportDecodingError("Invalid header in packet.  Ignoring packet.");
      return true;
    }
    if(!skipBlock)
    {
      // note we apply reset at the packet level. If there are multiple messages per packet
      // the decoder is NOT reset for each one.
      if(reset_)
      {
        decoder_.reset();
      }
      while(bytesAvailable() > 0)
      {
        decoder_.decodeMessage(*this, builder_);
      }
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
MessagePerPacketAssembler::receiverStarted(Communication::Receiver & /*receiver*/)
{
  decoder_.setStrict(strict_);
  if(builder_.wantLog(Common::Logger::QF_LOG_INFO))
  {
    builder_.logMessage(Common::Logger::QF_LOG_INFO, "Receiver started");
  }
}
void
MessagePerPacketAssembler::receiverStopped(Communication::Receiver & /*receiver*/)
{
  if(builder_.wantLog(Common::Logger::QF_LOG_INFO))
  {
    builder_.logMessage(Common::Logger::QF_LOG_INFO, "Receiver stopped");
  }
}

bool
MessagePerPacketAssembler::getBuffer(const uchar *& buffer, size_t & size)
{
  bool result = size_ > 0;
  buffer = buffer_;
  size = size_;
  size_ = 0;
  return result;
}

