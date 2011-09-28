// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include "StreamingAssembler.h"
#include <Communication/Receiver.h>
#include <Messages/ValueMessageBuilder.h>
#include <Codecs/DataSourceBuffer.h>
#include <Codecs/Decoder.h>

using namespace QuickFAST;
using namespace Codecs;

StreamingAssembler::StreamingAssembler(
      TemplateRegistryPtr templateRegistry,
      HeaderAnalyzer & headerAnalyzer,
      Messages::ValueMessageBuilder & builder,
      bool waitForCompleteMessage)
  : Communication::Assembler(templateRegistry, builder)
  , headerAnalyzer_(headerAnalyzer)
  , builder_(builder)
  , stopping_(false)
  , waitForCompleteMessage_(waitForCompleteMessage)
  , receiver_(0)
  , currentBuffer_(0)
  , headerIsComplete_(false)
  , skipBlock_(false)
  , blockSize_(0)
  , inDecoder_(false)
  , messageCount_(0)
  , byteCount_(0)
  , messageLimit_(0)
{
}

StreamingAssembler::~StreamingAssembler()
{
}

bool
StreamingAssembler::serviceQueue(
  Communication::Receiver & receiver)
{
  // save the receiver so callbacks from the decoder can find it.
  receiver_ = &receiver;
  bool more = true;
  while(more && !stopping_)
  {
    if(!headerIsComplete_)
    {
      headerIsComplete_ = headerAnalyzer_.analyzeHeader(*this, blockSize_, skipBlock_);
    }
    more = headerIsComplete_ && !stopping_;

    if(more)
    {
      if(waitForCompleteMessage_ && blockSize_ > 0)
      {
        /// check # bytes available to see if there's a complete message to decode
        size_t available = currentBytesAvailable();

        // try for more bytes: false means don't wait if they aren't there
        if(available < blockSize_
          && !receiver_->needBytes(blockSize_ - available, false))
        {
          // need more data;  Return and continue receiving
          more = false;
        }
      }
    }
    if(more)
    {
      headerIsComplete_ = 0;
      blockSize_ = 0;
      if(skipBlock_)
      {
//not implemented yet        builder_.skipMessage();
      }
      skipBlock_ = false;
      if(messageAvailable() > 0)
      {
        // Set this to indicate we block during decoding
        inDecoder_ = true;
        try
        {
          if(reset_)
          {
            decoder_.reset();
          }
          decoder_.decodeMessage(*this, builder_);
        }
        catch(std::exception & ex)
        {
          more = builder_.reportDecodingError(ex.what());
          if(!more)
          {
            stopping_ = true;
            if(currentBuffer_ != 0)
            {
              receiver.releaseBuffer(currentBuffer_);
              currentBuffer_ = 0;
            }
          }
        }
        inDecoder_ = false;
      }
      else
      {
        more = false;
      }
    }
  }
  receiver_ = 0;
  return !stopping_;
}

void
StreamingAssembler::receiverStarted(Communication::Receiver & /*receiver*/)
{
  decoder_.setStrict(strict_);
  if(builder_.wantLog(Common::Logger::QF_LOG_INFO))
  {
    builder_.logMessage(Common::Logger::QF_LOG_INFO, "Start receiver.");
  }
}

void
StreamingAssembler::receiverStopped(Communication::Receiver & receiver)
{
  if(builder_.wantLog(Common::Logger::QF_LOG_INFO))
  {
    builder_.logMessage(Common::Logger::QF_LOG_INFO, "Stop receiver.");
  }

  stopping_ = true;
  if(currentBuffer_ != 0)
  {
    receiver.releaseBuffer(currentBuffer_);
    currentBuffer_ = 0;
  }
}


bool
StreamingAssembler::getBuffer(const uchar *& buffer, size_t & size)
{
  size = 0;
  if(currentBuffer_ != 0)
  {
    if(receiver_ == 0)
    {
      throw UsageError(
        "Internal Error",
        "StreamingAssembler::readByte called in the wrong scope.");
    }
    receiver_->releaseBuffer(currentBuffer_);
    currentBuffer_ = 0;
  }

  // Look for a new buffer.  If we're in the decoder, wait for it.
  if(inDecoder_)
  {
    receiver_->waitBuffer();
  }
  currentBuffer_ = receiver_->getBuffer(inDecoder_);
  if(currentBuffer_ != 0)
  {
    buffer = currentBuffer_->get();
    size = currentBuffer_->used();
  }
  return size > 0;
}

int
StreamingAssembler::messageAvailable()
{
  if(stopping_)
  {
    return -1;
  }
  return bytesAvailable();
}

