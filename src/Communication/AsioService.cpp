// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "AsioService.h"
#include <Common/Logger.h>

using namespace QuickFAST;
using namespace Communication;

boost::asio::io_service AsioService::privateIoService_;

AsioService::AsioService()
  : stopping_(false)
  , threadCount_(0)
  , threadCapacity_(0)
  , ioService_(privateIoService_)
  , logger_(0)
{
}

AsioService::AsioService(boost::asio::io_service & ioService)
  : stopping_(false)
  , threadCount_(0)
  , threadCapacity_(0)
  , ioService_(ioService)
  , logger_(0)
{
}

AsioService::~AsioService()
{
}

void
AsioService::setLogger(Common::Logger & logger)
{
  logger_ = & logger;
}

void
AsioService::stopService()
{
  stopping_ = true;
  logger_ = 0;
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
      if(logger_ != 0)
      {
        logger_->reportCommunicationError(ex.what());
      }
      else
      {
        std::cerr << ex.what() << std::endl;
      }
    }
  }
}

