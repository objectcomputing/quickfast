// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "AsioService.h"
#include <Common/Logger.h>

using namespace QuickFAST;
using namespace Communication;

boost::asio::io_service AsioService::sharedIoService_;
AtomicCounter AsioService::sharedRunningThreadCount_;

AsioService::AsioService()
  : stopping_(false)
  , threadCount_(0)
  , threadCapacity_(0)
  , ioService_(sharedIoService_)
  , usingSharedService_(true)
  , logger_(0)
{
}

AsioService::AsioService(boost::asio::io_service & ioService)
  : stopping_(false)
  , threadCount_(0)
  , threadCapacity_(0)
  , ioService_(ioService)
  , usingSharedService_(false)
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
  long tc = 0;
  if(usingSharedService_)
  {
    tc = ++sharedRunningThreadCount_;
  }
  else
  {
    tc = ++runningThreadCount_;
  }
  //std::ostringstream msg;
  //msg << '{' << (void *) this << "} Starting AsioService thread #" << tc << std::endl;
  //std::cout << msg.str();

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
  //std::ostringstream msg2;
  //msg2 << '{' << (void *) this << "} Stopping AsioService thread #" << tc << std::endl;
  //std::cout << msg2.str();

  --runningThreadCount_;
}

