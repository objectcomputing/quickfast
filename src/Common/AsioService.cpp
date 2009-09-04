// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "AsioService.h"


using namespace ::QuickFAST;

boost::asio::io_service AsioService::privateIoService_;

AsioService::AsioService()
  : stopping_(false)
  , threadCount_(0)
  , threadCapacity_(0)
  , ioService_(privateIoService_)
  , strand_(ioService_)
{
}

AsioService::AsioService(boost::asio::io_service & ioService)
  : stopping_(false)
  , threadCount_(0)
  , threadCapacity_(0)
  , ioService_(ioService)
  , strand_(ioService_)
{
}

AsioService::~AsioService()
{
}

void
AsioService::stopService()
{
  stopping_ = true;
  ioService_.stop();
}

void
AsioService::joinThreads()
{
  while(threadCount_ > 0)
  {
     --threadCount_;
    threads_[threadCount_]->join();
    threads_[threadCount_].reset();
  }
}

void
AsioService::runThreads(size_t threadCount /*= 0*/, bool useThisThread /* = true*/)
{
  if(threadCount > threadCapacity_)
  {
    boost::scoped_array<ThreadPtr> newThreads(new ThreadPtr[threadCount]);
    for(size_t nThread = 0; nThread < threadCount_; ++nThread)
    {
      newThreads[nThread] = threads_[nThread];
    }
    threads_.swap(newThreads);
    threadCapacity_ = threadCount;
  }
  while(threadCount_ < threadCount)
  {
    threads_[threadCount_].reset(
      new boost::thread(boost::bind(&AsioService::run, this)));
    ++threadCount_;
  }
  if(useThisThread)
  {
    run();
    joinThreads();
  }
}

void
AsioService::run()
  {
  while(! stopping_)
  {
    try
    {
      size_t count = ioService_.run();
      if(count == 0)
      {
        return;
      }
    }
    catch (const std::exception & ex)
    {
      // todo: logging?
      std::cerr << ex.what() << std::endl;
    }
  }
}

