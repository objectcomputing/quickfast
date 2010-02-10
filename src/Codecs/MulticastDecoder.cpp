// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "MulticastDecoder.h"
#include <Communication/LinkedBuffer.h>
#include <Codecs/MessagePerPacketQueueService.h>

using namespace QuickFAST;
using namespace Codecs;

MulticastDecoder::MulticastDecoder(
  TemplateRegistryPtr templateRegistry,
  const std::string & multicastGroupIP,
  const std::string & listenAddressIP,
  unsigned short portNumber)
: receiver_(multicastGroupIP, listenAddressIP, portNumber)
, templateRegistry_(templateRegistry)
, messageLimit_(0)
, strict_(true)
, verboseOut_(0)
{
}

MulticastDecoder::MulticastDecoder(
  TemplateRegistryPtr templateRegistry,
  boost::asio::io_service & ioService,
  const std::string & multicastGroupIP,
  const std::string & listenAddressIP,
  unsigned short portNumber)
: receiver_(ioService, multicastGroupIP, listenAddressIP, portNumber)
, templateRegistry_(templateRegistry)
, messageLimit_(0)
, strict_(true)
, verboseOut_(0)
{
}

MulticastDecoder::~MulticastDecoder()
{
}

void
MulticastDecoder::setVerboseOutput(std::ostream & out)
{
  if(queueService_)
  {
    queueService_->decoder().setVerboseOutput(out);
  }
  verboseOut_ = &out;
}

void
MulticastDecoder::disableVerboseOutput()
{
  if(queueService_)
  {
    queueService_->decoder().disableVerboseOutput();
  }
  verboseOut_ = 0;
}

void
MulticastDecoder::setStrict(bool strict)
{
  if(queueService_)
  {
    queueService_->decoder().setStrict(strict);
  }
  strict_ = true;
}

bool
MulticastDecoder::getStrict()const
{
  return strict_;
}

size_t
MulticastDecoder::messageCount() const
{
  if(queueService_)
  {
    return queueService_->messageCount();
  }
  return 0;
}

void
MulticastDecoder::reset()
{
  if(queueService_)
  {
    queueService_->decoder().reset();
  }
}

void
MulticastDecoder::start(
  Messages::ValueMessageBuilder & builder,
  size_t bufferSize /*=1400*/,
  size_t bufferCount /*=2*/)
{
  builder_ = &builder;
  queueService_.reset(new MessagePerPacketQueueService(
    templateRegistry_,
    headerAnalyzer_,
    builder));
  queueService_->setMessageLimit(messageLimit_);
  queueService_->decoder().setStrict(strict_);
  if(verboseOut_ != 0)
  {
    queueService_->decoder().setVerboseOutput(*verboseOut_);
  }

  receiver_.start(*queueService_, bufferSize, bufferCount);
}

void
MulticastDecoder::start(
  Messages::ValueMessageBuilder & builder,
  HeaderAnalyzer & headerAnalyzer,
  size_t bufferSize /*=1400*/,
  size_t bufferCount /*=2*/)
{
  builder_ = &builder;
  queueService_.reset(new MessagePerPacketQueueService(
    templateRegistry_,
    headerAnalyzer,
    builder));
  queueService_->setMessageLimit(messageLimit_);
  queueService_->decoder().setStrict(strict_);
  if(verboseOut_ != 0)
  {
    queueService_->decoder().setVerboseOutput(*verboseOut_);
  }

  receiver_.start(*queueService_, bufferSize, bufferCount);
}

