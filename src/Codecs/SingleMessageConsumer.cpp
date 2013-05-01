// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "SingleMessageConsumer.h"
using namespace QuickFAST;
using namespace Codecs;

SingleMessageConsumer::SingleMessageConsumer()
: message_(1)
{
}
SingleMessageConsumer::~SingleMessageConsumer()
{
}

bool
SingleMessageConsumer::consumeMessage(Messages::Message & message)
{
  message_.swap(message);
  return true;
}

bool
SingleMessageConsumer::wantLog(unsigned short /*level*/)
{
  return true;
}

bool
SingleMessageConsumer::logMessage(unsigned short /*level*/, const std::string & logMessage)
{
  std::cout << logMessage << std::endl;
  return true;
}

bool
SingleMessageConsumer::reportDecodingError(const std::string & errorMessage)
{
  std::cout << errorMessage << std::endl;
  return true;
}

bool
SingleMessageConsumer::reportCommunicationError(const std::string & errorMessage)
{
  std::cout << errorMessage << std::endl;
  return true;
}

void
SingleMessageConsumer::decodingStarted()
{
}

void
SingleMessageConsumer::decodingStopped()
{

}

Messages::Message &
SingleMessageConsumer::message()
{
  return message_;
}
