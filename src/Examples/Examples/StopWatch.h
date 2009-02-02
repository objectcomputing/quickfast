// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef STOPWATCH_H
#define STOPWATCH_H

namespace QuickFAST{
  namespace Examples{
    /// @brief Measure the wall-clock lapse between creation and freeze()
    class StopWatch
    {
    public:
      /// @brief Create the stopwatch and start it running.
      StopWatch()
        : start_(boost::posix_time::microsec_clock::universal_time())
        , running_(true)
      {
      }

      ~StopWatch()
      {
      }

      /// @brief stop or pause the clock and return lapsed time
      unsigned long freeze()
      {
        running_ = true;
        return lapse();
      }

      /// @brief retrieve the lapsed time in milliseconds.  Freeze if necessary.
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
}
#endif // STOPWATCH_H
