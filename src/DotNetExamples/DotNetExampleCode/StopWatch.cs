using System;
using System.Text;
using System.Runtime.InteropServices;

namespace QuickFASTDotNet
{
    namespace Examples
    {
        public class StopWatch
        {
            [DllImport("Kernel32.dll")]
            private static extern bool QueryPerformanceCounter(
                out long lpPerformanceCount);

            [DllImport("Kernel32.dll")]
            private static extern bool QueryPerformanceFrequency(
                out long lpFrequency);

            private long startTime_, stopTime_;
            private long frequency_;
            private bool isPerfCounterSupported_ = false;
            private bool running_ = false;

            public StopWatch()
            {
                isPerfCounterSupported_ = QueryPerformanceFrequency(out frequency_);
                if (isPerfCounterSupported_ == true)
                {
                    QueryPerformanceCounter(out startTime_);
                }
                else
                {
                    frequency_ = 1000;
                    System.Console.WriteLine("WARNING: High resolution timer is not supported using Environment.TickCount");
                    startTime_ = Environment.TickCount;
                }
                running_ = true;

            }


            // Milliseconds elapsed since started running.
            public long lapse()
            {
                if (running_)
                {
                    if (isPerfCounterSupported_)
                    {
                        QueryPerformanceCounter(out stopTime_);
                    }
                    else
                    {
                        stopTime_ = Environment.TickCount;
                    }
                    running_ = false;
                }

                long totalTicks = stopTime_ - startTime_;
                long milliseconds = totalTicks;
                if (isPerfCounterSupported_)
                    milliseconds = (1000 * totalTicks / frequency_);

                return milliseconds;
            }

            public long freeze()
            {
                running_ = true;
                return lapse();
            }
        }
    }
}
