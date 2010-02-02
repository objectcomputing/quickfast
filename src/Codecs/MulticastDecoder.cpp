// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "MulticastDecoder.h"
#include <Common/LinkedBuffer.h>
#include <Codecs/MessagePerPacketBufferConsumer.h>

using namespace QuickFAST;
using namespace Codecs;

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
  bufferConsumer_.reset(new MessagePerPacketBufferConsumer(
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
