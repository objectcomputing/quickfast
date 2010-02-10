// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include "BlockedStreamQueueService.h"
#include <Communication/Receiver.h>
#include <Messages/ValueMessageBuilder.h>
#include <Codecs/DataSourceBuffer.h>
#include <Codecs/Decoder.h>

using namespace QuickFAST;
using namespace Codecs;

BlockedStreamQueueService::BlockedStreamQueueService(
  TemplateRegistryPtr templateRegistry,
  Messages::ValueMessageBuilder & builder)
  : Communication::BufferQueueService(builder)
  , builder_(builder)
  , stopping_(false)
  , headerType_(HEADER_FAST)
  , headerPrefix_(0)
  , blockSizeBytes_(0)
  , blockSizeSwap_(false)
  , headerSuffix_(0)
  , decoder_(templateRegistry)
  , receiver_(0)
  , currentBuffer_(0)
  , pos_(0)
  , blockSizeIsValid_(false)
  , parsingBlockSize_(false)
  , blockSize_(0)
  , headerPos_(0)
  , inDecoder_(false)
{
}

BlockedStreamQueueService::~BlockedStreamQueueService()
{
}

bool
BlockedStreamQueueService::serviceQueue(
  Communication::Receiver & receiver)
{
  bool result = true;
  // save the receiver so callbacks from the decoder can find it.
  receiver_ = &receiver;
  bool more = true;
  while(more && result && !stopping_)
  {
    if(messageAvailable () > 0)
    {
      inDecoder_ = true;
      try
      {
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
  receiver_ = 0;
  return result;
}

void
BlockedStreamQueueService::receiverStarted(Communication::Receiver & /*receiver*/)
{
  if(builder_.wantLog(Common::Logger::QF_LOG_INFO))
  {
    builder_.logMessage(Common::Logger::QF_LOG_INFO, "Start TCP/IP receiver.");
  }
}

void
BlockedStreamQueueService::receiverStopped(Communication::Receiver & receiver)
{
  if(builder_.wantLog(Common::Logger::QF_LOG_INFO))
  {
    builder_.logMessage(Common::Logger::QF_LOG_INFO, "Stop TCP/IP receiver.");
  }

  stopping_ = true;
  if(currentBuffer_ != 0)
  {
    receiver.releaseBuffer(currentBuffer_);
    currentBuffer_ = 0;
  }
}


bool
BlockedStreamQueueService::getBuffer(const uchar *& buffer, size_t & size)
{
  size = 0;
  if(currentBuffer_ != 0)
  {
    if(receiver_ == 0)
    {
      throw UsageError(
        "[U100]",
        "BlockedStreamQueueService::readByte called in the wrong scope.");
    }
    receiver_->releaseBuffer(currentBuffer_);

    // Look for a new buffer.  If we're in the decoder, wait for it.
    currentBuffer_ = receiver_->getBuffer(inDecoder_);
    if(currentBuffer_ != 0)
    {
      buffer = currentBuffer_->get();
      size = currentBuffer_->used();
    }
  }
  return size > 0;
}

int
BlockedStreamQueueService::messageAvailable()
{
  if(stopping_)
  {
    return -1;
  }

  while(!blockSizeIsValid_)
  {
    if(headerType_ == HEADER_NONE)
    {
      blockSizeIsValid_ = true;
      blockSize_ = 0;
    }
    else if(headerType_ == HEADER_FIXED)
    {
      if(!parsingBlockSize_)
      {
        parsingBlockSize_ = true;
        blockSize_ = 0;
        headerPos_ = 0;
      }
      uchar next;
      if(!getByte(next))
      {
        return 0;
      }
      if(headerPos_ < headerPrefix_)
      {
        // ignore this byte
      }
      else if(headerPos_ < headerPrefix_ + blockSizeBytes_)
      {
        blockSize_ <<= 8;
        blockSize_ |= next;
      }
      headerPos_ += 1;
      if(headerPos_ >= headerPrefix_ + blockSizeBytes_ + headerSuffix_)
      {
        blockSizeIsValid_ = true;
        parsingBlockSize_ = false;
        headerPos_ = 0;
      }
      //todo byte swap block size if necessary
    }
    else // FAST encoded block size
    {
      if(!parsingBlockSize_)
      {
        parsingBlockSize_ = true;
        blockSize_ = 0;
        headerPos_ = 0;
      }
      uchar next;
      if(!getByte(next))
      {
        return 0;
      }
      bool isStopByte = (next & 0x80) != 0;
      if(headerPos_ < headerPrefix_)
      {
        if(isStopByte)
        {
          headerPos_ += 1;
        }
      }
      else if(headerPos_ == headerPrefix_)
      {
        if(isStopByte)
        {
          headerPos_ += 1;
          if(headerSuffix_ == 0)
          {
            blockSizeIsValid_ = true;
            parsingBlockSize_ = false;
            headerPos_ = 0;
          }
        }
        blockSize_ <<= 7;
        blockSize_ |= (next & 0x7F);
      }
      else if(headerPos_ < headerPrefix_ + 1 + headerSuffix_)
      {
        if(isStopByte)
        {
          headerPos_ += 1;
          if(headerPos_ == headerPrefix_ + 1 + headerSuffix_)
          {
            blockSizeIsValid_ = true;
            parsingBlockSize_ = false;
            headerPos_ = 0;
          }
        }
      }
    }
  }

  /// we now have blockSize_.
  /// check # bytes available to see if there's a complete message to decode
  size_t available = 0;
  if(currentBuffer_ != 0)
  {
    available = currentBuffer_->used() - pos_;
    if(available >= blockSize_ || receiver_->needBytes(blockSize_ - available, false))
    {
      // we've used up this block size
      blockSizeIsValid_ = false;
      return 1;
    }
  }
  return 0;
}

