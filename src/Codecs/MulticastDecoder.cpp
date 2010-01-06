// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "MulticastDecoder.h"

using namespace QuickFAST;
using namespace Codecs;

namespace
{
  class GenericMessageBufferConsumer : public BufferConsumer
  {
  public:
    GenericMessageBufferConsumer(
        Messages::ValueMessageBuilder & builder,
        Decoder & decoder,
        size_t & messageCount,
        size_t & messageLimit);

    virtual ~GenericMessageBufferConsumer();

    ///////////////////////////
    // Implement BufferConsumer
    virtual bool consumeBuffer(const unsigned char * buffer, size_t bufferSize);
    virtual void receiverStarted();
    virtual void receiverStopped();

    ///////////////////
    // Implement Logger
    virtual bool wantLog(unsigned short level);
    virtual bool logMessage(unsigned short level, const std::string & logMessage);
    virtual bool reportDecodingError(const std::string & errorMessage);
    virtual bool reportCommunicationError(const std::string & errorMessage);

  private:
    GenericMessageBufferConsumer & operator = (const GenericMessageBufferConsumer &);
    GenericMessageBufferConsumer(const GenericMessageBufferConsumer &);
    GenericMessageBufferConsumer();

  private:
    Messages::ValueMessageBuilder & builder_;
    Decoder & decoder_;
    size_t & messageLimit_;
    size_t & messageCount_;
  };

}


GenericMessageBufferConsumer::GenericMessageBufferConsumer(
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

GenericMessageBufferConsumer::~GenericMessageBufferConsumer()
{
}

bool
GenericMessageBufferConsumer::consumeBuffer(const unsigned char * buffer, size_t bufferSize)
{
  ++messageCount_;
  if(builder_.wantLog(Logger::QF_LOG_VERBOSE))
  {
    std::stringstream message;
    message << "Received[" << messageCount_ << "]: " << bufferSize << " bytes";
    builder_.logMessage(MessageConsumer::QF_LOG_VERBOSE, message.str());
  }
  try
  {
    DataSourceBuffer source(buffer, bufferSize);
    // Unreliable multicast.  Always reset the decoder
    decoder_.reset();
    while(source.bytesAvailable() > 0)
    {
      decoder_.decodeMessage(source, builder_);
    }
  }
  catch (const std::exception &ex)
  {
    return builder_.reportDecodingError(ex.what());
  }
  if(messageCount_ > messageLimit_ && messageLimit_ != 0)
  {
    return false;
  }
  return true;
}

void
GenericMessageBufferConsumer::receiverStarted()
{
  /// todo Log?
}
void
GenericMessageBufferConsumer::receiverStopped()
{
  /// todo log?
}

bool
GenericMessageBufferConsumer::wantLog(unsigned short level)
{
  return builder_.wantLog(level);
}

bool
GenericMessageBufferConsumer::logMessage(unsigned short level, const std::string & logMessage)
{
  return builder_.logMessage(level, logMessage);
}

bool
GenericMessageBufferConsumer::reportDecodingError(const std::string & errorMessage)
{
  return builder_.reportDecodingError(errorMessage);
}

bool
GenericMessageBufferConsumer::reportCommunicationError(const std::string & errorMessage)
{
  return builder_.reportCommunicationError(errorMessage);
}


MulticastDecoder::MulticastDecoder(
  TemplateRegistryPtr templateRegistry,
  const std::string & multicastGroupIP,
  const std::string & listenAddressIP,
  unsigned short portNumber)
: receiver_(multicastGroupIP, listenAddressIP, portNumber)
, decoder_(templateRegistry)
, messageLimit_(0)
, messageCount_(0)
{
}

MulticastDecoder::MulticastDecoder(
  TemplateRegistryPtr templateRegistry,
  boost::asio::io_service & ioService,
  const std::string & multicastGroupIP,
  const std::string & listenAddressIP,
  unsigned short portNumber)
: receiver_(ioService, multicastGroupIP, listenAddressIP, portNumber)
, decoder_(templateRegistry)
, messageLimit_(0)
, messageCount_(0)
{
}

MulticastDecoder::~MulticastDecoder()
{
}

void
MulticastDecoder::setVerboseOutput(std::ostream & out)
{
  decoder_.setVerboseOutput(out);
}

void
MulticastDecoder::disableVerboseOutput()
{
  decoder_.disableVerboseOutput();
}

void
MulticastDecoder::setStrict(bool strict)
{
  decoder_.setStrict(strict);
}

bool
MulticastDecoder::getStrict()const
{
  return decoder_.getStrict();
}

void
MulticastDecoder::setLimit(size_t messageLimit)
{
  messageLimit_ = messageLimit;
}

size_t
MulticastDecoder::messageCount() const
{
  return messageCount_;
}

void
MulticastDecoder::reset()
{
  decoder_.reset();
}

void
MulticastDecoder::start(Messages::ValueMessageBuilder & builder, size_t bufferSize /*=1400*/, size_t bufferCount /*=2*/)
{
  builder_ = &builder;
  bufferConsumer_.reset(new GenericMessageBufferConsumer(
    builder,
    decoder_,
    messageLimit_,
    messageCount_));
  receiver_.start(*bufferConsumer_, bufferSize, bufferCount);
}

void
MulticastDecoder::run()
{
  receiver_.run();
}

void
MulticastDecoder::run(size_t threadCount, bool useThisThread)
{
  receiver_.runThreads(threadCount,useThisThread);
}

void
MulticastDecoder::stop()
{
  receiver_.stop();
}

void
MulticastDecoder::joinThreads()
{
  receiver_.joinThreads();
}
