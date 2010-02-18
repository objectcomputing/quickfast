// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DataSource.h"
#include <Common/Exceptions.h>
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

DataSource::DataSource()
: buffer_(0)
, size_(0)
, position_(0)
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
  if(position_ >= size_)
  {
    if(getBuffer(buffer_, size_))
    {
      position_ = 0;
    }
  }
  return size_ - position_;
}

bool
DataSource::getBuffer(const uchar *& buffer, size_t & size)
{
  // for backwards compatibility
  buffer = &byteBuffer_;
  size_ = 1;
  return readByte(byteBuffer_);
}

void
DataSource::doEcho(bool ok, uchar byte)
{
  if(ok)
  {
    ++byteCount_;
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
  else
  {
    if(verboseMessages_)
    {
      (*echo_) << "*** End of data @" << std::hex << byteCount_ << std::dec << "***" << std::endl;
    }
  }
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

bool
DataSource::readByte(uchar & byte)
{
  throw UsageError("Coding error", "Derived class must override DataSource::getBuffer or DataSource::readByte.");
}
