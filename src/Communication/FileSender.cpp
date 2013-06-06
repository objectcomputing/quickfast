// Copyright (c) 2009, 2010, 2011, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include "FileSender.h"
#include <Communication/LinkedBuffer.h>
#include <Common/Exceptions.h>
using namespace QuickFAST;
using namespace Communication;


FileSender::FileSender(
  BufferRecycler & recycler,
  const char * fileName,
  bool binary)
    : Sender(recycler)
    , fileName_(fileName)
    , binary_(binary)
    , file_(0)
{
}
FileSender::~FileSender()
{
  if(file_ != 0)
  {
    std::fclose(file_);
    file_ = 0;
  }
}

void
FileSender::open()
{
  file_ = std::fopen(fileName_.c_str(), binary_? "rb":"r");
  if(file_ == 0)
  {
    std::stringstream msg;
    msg << "Error opening file " << fileName_;
    throw CommunicationError(msg.str());
  }
}

void
FileSender::send(LinkedBuffer * buffer)
{
  size_t count = buffer->used();
  size_t written = std::fwrite(buffer->get(), 1, count, file_);
  recycle(buffer);
  if(written != count)
  {
    std::stringstream msg;
    msg
      << "Error writing to file " << fileName_
      << " at " << std::ftell(file_)
      << ". Tried to write " << count
      << " bytes.  Actually wrote " << written << " bytes.";
    throw CommunicationError(msg.str());
  }
}

void
FileSender::close()
{
  std::fclose(file_);
  file_ = 0;
}
