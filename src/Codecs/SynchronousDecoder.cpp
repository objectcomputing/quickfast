// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include "SynchronousDecoder.h"
#include <Codecs/DataSource.h>
#include <Codecs/MessageConsumer.h>
#include <Codecs/TemplateRegistry.h>
#include <Messages/Message.h>

using namespace QuickFAST;
using namespace Codecs;

SynchronousDecoder::SynchronousDecoder(Codecs::TemplateRegistryPtr templateRegistry)
: decoder_(templateRegistry)
, resetOnMessage_(false)
, messageCount_(0)
, messageCountLimit_(size_t(-1))
, maxFieldCount_(templateRegistry->maxFieldCount())
{
}

SynchronousDecoder::~SynchronousDecoder()
{
}

void
SynchronousDecoder::setResetOnMessage(bool reset)
{
  resetOnMessage_ = reset;
}


void
SynchronousDecoder::setVerboseOutput(std::ostream & out)
{
  decoder_.setVerboseOutput(out);
}

void
SynchronousDecoder::decode(
  Codecs::DataSource & source,
  Codecs::MessageConsumer & consumer)
{
  bool more = true;
  while(source.messageAvailable() > 0 && messageCount_ < messageCountLimit_)
  {
    Messages::Message message(maxFieldCount_);
    if(resetOnMessage_)
    {
      decoder_.reset();
    }
    if(!decoder_.decodeMessage(source, message))
    {
      std::cout << "EOF during decode." << std::endl;
    }
    more = consumer.consumeMessage(message);
    messageCount_ += 1;
  }
}

