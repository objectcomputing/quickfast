// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef STOPWATCH_H
#define STOPWATCH_H

namespace QuickFAST{
  class StopWatch
  {
  public:
    StopWatch()
      : start_(boost::posix_time::microsec_clock::universal_time())
      , running_(true)
    {
    }

    ~StopWatch()
    {
    }

    unsigned long freeze()
    {
      running_ = true;
      return lapse();
    }

    unsigned long lapse()
    {
      if(running_)
      {
        stop_ = boost::posix_time::microsec_clock::universal_time();
        running_ = false;
      }

      return static_cast<unsigned long>((stop_ - start_).total_milliseconds());
    }

  private:
    boost::posix_time::ptime start_;
    boost::posix_time::ptime  stop_;
    bool running_;
  };
}
#endif // STOPWATCH_H
