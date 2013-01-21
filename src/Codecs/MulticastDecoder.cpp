// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "MulticastDecoder.h"
#include <Communication/LinkedBuffer.h>
#include <Codecs/MessagePerPacketAssembler.h>

using namespace QuickFAST;
using namespace Codecs;

MulticastDecoder::MulticastDecoder(
  TemplateRegistryPtr templateRegistry,
  const std::string & multicastGroupIP,
  const std::string & listenAddressIP,
  const std::string & bindIP,
  unsigned short portNumber)
: receiver_(multicastGroupIP, listenAddressIP, bindIP, portNumber)
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
  const std::string & bindIP,
  unsigned short portNumber)
: receiver_(ioService, multicastGroupIP, listenAddressIP, bindIP, portNumber)
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
  if(assembler_)
  {
    assembler_->decoder().setVerboseOutput(out);
  }
  verboseOut_ = &out;
}

void
MulticastDecoder::disableVerboseOutput()
{
  if(assembler_)
  {
    assembler_->decoder().disableVerboseOutput();
  }
  verboseOut_ = 0;
}

void
MulticastDecoder::setStrict(bool strict)
{
  if(assembler_)
  {
    assembler_->decoder().setStrict(strict);
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
  if(assembler_)
  {
    return assembler_->messageCount();
  }
  return 0;
}

void
MulticastDecoder::reset()
{
  if(assembler_)
  {
    assembler_->decoder().reset();
  }
}

void
MulticastDecoder::start(
  Messages::ValueMessageBuilder & builder,
  size_t bufferSize /*=1500*/,
  size_t bufferCount /*=2*/)
{
  builder_ = &builder;
  assembler_.reset(new MessagePerPacketAssembler(
    templateRegistry_,
    packetHeaderAnalyzer_,
    messageHeaderAnalyzer_,
    builder));
  assembler_->setMessageLimit(messageLimit_);
  assembler_->decoder().setStrict(strict_);
  if(verboseOut_ != 0)
  {
    assembler_->decoder().setVerboseOutput(*verboseOut_);
  }

  receiver_.start(*assembler_, bufferSize, bufferCount);
}

void
MulticastDecoder::start(
  Messages::ValueMessageBuilder & builder,
  HeaderAnalyzer & packetHeaderAnalyzer,
  size_t bufferSize /*=1500*/,
  size_t bufferCount /*=2*/)
{
  builder_ = &builder;
  assembler_.reset(new MessagePerPacketAssembler(
    templateRegistry_,
    packetHeaderAnalyzer,
    messageHeaderAnalyzer_,
    builder));
  assembler_->setMessageLimit(messageLimit_);
  assembler_->decoder().setStrict(strict_);
  if(verboseOut_ != 0)
  {
    assembler_->decoder().setVerboseOutput(*verboseOut_);
  }

  receiver_.start(*assembler_, bufferSize, bufferCount);
}

void
MulticastDecoder::start(
  Messages::ValueMessageBuilder & builder,
  HeaderAnalyzer & packetHeaderAnalyzer,
  HeaderAnalyzer & messageHeaderAnalyzer,
  size_t bufferSize /*=1500*/,
  size_t bufferCount /*=2*/)
{
  builder_ = &builder;
  assembler_.reset(new MessagePerPacketAssembler(
    templateRegistry_,
    packetHeaderAnalyzer,
    messageHeaderAnalyzer,
    builder));
  assembler_->setMessageLimit(messageLimit_);
  assembler_->decoder().setStrict(strict_);
  if(verboseOut_ != 0)
  {
    assembler_->decoder().setVerboseOutput(*verboseOut_);
  }

  receiver_.start(*assembler_, bufferSize, bufferCount);
}

