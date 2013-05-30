// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Examples/ExamplesPch.h>
#include "MessageCounter.h"
#include <Messages/Message.h>
//#include <Messages/Field.h>
#include <Messages/Sequence.h>
#include <Messages/Group.h>

using namespace QuickFAST;
using namespace Examples;

MessageCounter::MessageCounter()
  : messageCount_(0)
{
}

MessageCounter::~MessageCounter()
{
}

bool
MessageCounter::consumeMessage(Messages::Message & message)
{
  messageCount_ += 1;
  return true;
}

bool
MessageCounter::wantLog(unsigned short level)
{
  return false;
}

bool
MessageCounter::logMessage(unsigned short level, const std::string & logMessage)
{
  return true;
}

bool
MessageCounter::reportDecodingError(const std::string & errorMessage)
{
  std::cerr << "Decoding error: " << errorMessage << std::endl;
  return false;
}

bool
MessageCounter::reportCommunicationError(const std::string & errorMessage)
{
  std::cerr << "Communication error: " << errorMessage << std::endl;
  return false;
}

void
MessageCounter::decodingStopped()
{
}


