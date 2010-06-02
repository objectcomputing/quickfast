// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Examples/ExamplesPch.h>
#include "MessageInterpreter.h"

using namespace QuickFAST;
using namespace Examples;

////////////////////////
// MessageInterpreter
MessageInterpreter::MessageInterpreter(std::ostream & out, bool silent)
  : formatter_(out)
  , out_(out)
  , indent_(0)
  , recordCount_(0)
  , logLevel_(Common::Logger::QF_LOG_WARNING)
  , silent_(silent)
{
}

MessageInterpreter::~MessageInterpreter()
{
}

bool
MessageInterpreter::wantLog(unsigned short level)
{
  return level <= logLevel_;
}

bool
MessageInterpreter::logMessage(unsigned short level, const std::string & logMessage)
{
  if(level <= logLevel_)
  {
    std::cerr << logMessage << std::endl;
  }
  return true;
}


bool
MessageInterpreter::reportDecodingError(const std::string & errorMessage)
{
  std::cerr << "Decoding error: " << errorMessage << std::endl;
  return false;
}

bool
MessageInterpreter::reportCommunicationError(const std::string & errorMessage)
{
  std::cerr << "Communication error: " << errorMessage << std::endl;
  return false;
}

void
MessageInterpreter::decodingStarted()
{
  // ignore this.
}

void
MessageInterpreter::decodingStopped()
{
  out_ << "End of data" << std::endl;
}


bool
MessageInterpreter::consumeMessage(Messages::Message & message)
{
  recordCount_ += 1;
  out_ << "Record #" << recordCount_ << ' ' << std::flush;
  if(!silent_)
  {
    formatter_.formatMessage(message);
  }
  out_ << std::endl;
  return true;
}
