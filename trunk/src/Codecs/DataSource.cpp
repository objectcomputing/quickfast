// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DataSource.h"
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

DataSource::DataSource()
: lookAhead_(0)
, lookedAhead_(false)
, echo_(0)
, raw_(false)
, hex_(true)
, verboseMessages_(true)
, verboseFields_(false)
, byteCount_(0)
{
}

DataSource::~DataSource()
{
}

int
DataSource::messageAvailable()
{
  // the default implementation assumes that any data at all means the next message will
  // appear when needed.
  return bytesAvailable();
}

int
DataSource::bytesAvailable()
{
  if(lookedAhead_)
  {
    return 1;
  }

  if(readByte(lookAhead_))
  {
    lookedAhead_ = true;
    return 1;
  }
  return -1;
}

bool
DataSource::getByte(uchar & byte)
{
  bool ok = true;
  if(lookedAhead_)
  {
    lookedAhead_ = false;
    byte = lookAhead_;
  }
  else
  {
    ok = readByte(byte);
  }

  if(ok)
  {
    ++byteCount_;
    if(echo_)
    {
      if(hex_)
      {
        (*echo_) << std::hex << std::setw(2) << std::setfill('0')
          << short(byte) << ' '
          << std::setfill(' ') << std::setw(0) << std::dec;
      }
      else if(raw_)
      {
        echo_->put(byte);
      }
    }
  }
  else
  {
    if(echo_)
    {
      if(verboseMessages_)
      {
        (*echo_) << "*** End of data @" << std::hex << byteCount_ << std::dec << "***" << std::endl;
      }
    }
  }
  return ok;
}

void
DataSource::setEcho(
  std::ostream & echo,
  const EchoType& echoType /* = HEX */,
  bool verboseMessages/* = true */,
  bool verboseFields/* = false */)
{
  echo_ = &echo;
  if (echoType == HEX)
  {
    raw_ = false;
    hex_ = true;
  }
  else if (echoType == RAW)
  {
    raw_ = true;
    hex_ = false;
  }
  else if (echoType == NONE)
  {
    raw_ = false;
    hex_ = false;
  }
  verboseMessages_ = verboseMessages;
  verboseFields_ = verboseFields;
}

void
DataSource::beginMessage()
{
  if(echo_ && verboseMessages_)
  {
    (*echo_) << std::endl << "***MESSAGE @" << std::hex << byteCount_ << std::dec << "***" << std::endl;
  }
}

void
DataSource::beginField(const std::string & name)
{
  if(echo_ && verboseFields_)
  {
    (*echo_) << std::endl << "***Field: " << name << " @" << std::hex<< byteCount_ << std::dec << "***" << std::endl;
  }
}
