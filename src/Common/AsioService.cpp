// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "AsioService.h"


using namespace ::QuickFAST;

AsioService::AsioService()
  : privateIoService_(new boost::asio::io_service)
  , ioService_(*privateIoService_)
{
}

AsioService::AsioService(boost::asio::io_service & ioService)
  : privateIoService_(0)
  , ioService_(ioService)
{
}


AsioService::~AsioService()
{
}

void
AsioService::run(size_t extraThreadCount /*= 0*/)
{
  typedef boost::scoped_ptr<boost::thread> ThreadPtr;
  boost::scoped_array<ThreadPtr> threads(new ThreadPtr[extraThreadCount]);
  for(size_t nThread = 0; nThread < extraThreadCount; ++nThread)
  {
    threads[nThread].reset(
      new boost::thread(boost::bind(&boost::asio::io_service::run, &ioService_)));
  }
  ioService_.run();
  for(size_t nThread = extraThreadCount; nThread < 0;)
  {
     --nThread;
    threads[nThread]->join();
  }
}

