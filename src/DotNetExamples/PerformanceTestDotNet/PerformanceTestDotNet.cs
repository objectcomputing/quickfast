using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime;
using System.Diagnostics;

namespace QuickFASTDotNet
{
    namespace Examples
    {
        public class PerformanceTestDotNet
        {
            string performanceFileName_;
            System.IO.FileStream performanceFileStream_;
            System.IO.TextWriter performanceFile_;

            uint loop_;
            uint limit_;
            uint duplicate_;

            public PerformanceTestDotNet()
            {
                performanceFileName_ = null;
                loop_ = 1;
                limit_ = 0;
                duplicate_ = 0;
            }

            public void usage()
            {
                System.Console.WriteLine("Usage of this program");
                System.Console.WriteLine("  -t file     : Template file (required)");
                System.Console.WriteLine("  -f file     : FAST Message file (required)");
                System.Console.WriteLine("  -p file     : File to which performance measurements are written. (default standard output)");
                System.Console.WriteLine("  -c count    : repeat the test 'count' times");
                System.Console.WriteLine("  -r          : Toggle 'reset decoder on every message' (default false).");
                System.Console.WriteLine("  -s          : Toggle 'strict decoding rules' (default true).");
                System.Console.WriteLine("  -i count  : Interpret messages count times instead of just counting them.");
                System.Console.WriteLine("  -l limit    : Process only the first 'limit' messages.");
            }



            public bool init(ref string[] args)
            {
                bool readTemplateName = false;
                bool readSourceName = false;
                bool readPerfName = false;
                bool readCount = false;
                bool readLimit = false;
                bool readDuplicate = false;
                bool ok = true;

                foreach (string opt in args)
                {
                    if (readTemplateName)
                    {
                        decoder_.TemplateFileName = opt;
                        readTemplateName = false;
                    }
                    else if (readSourceName)
                    {
                        decoder_.FastFileName = opt;
                        decoder_.ReceiverType = QuickFAST.DotNet.DNDecoderConnection.ReceiverTypes.RAWFILE_RECEIVER;
                        decoder_.HeaderType = QuickFAST.DotNet.DNDecoderConnection.HeaderTypes.NO_HEADER;
                        readSourceName = false;
                    }
                    else if (readPerfName)
                    {
                        performanceFileName_ = opt;
                        readPerfName = false;
                    }
                    else if (readCount)
                    {
                        loop_ = System.Convert.ToUInt32(opt);
                        readCount = false;
                    }
                    else if (readLimit)
                    {
                      limit_ = System.Convert.ToUInt32(opt);
                      readLimit = false;
                    }
                    else if (readDuplicate)
                    {
                      duplicate_ = System.Convert.ToUInt32(opt);
                      readDuplicate = false;
                    }
                    else if (opt == "-r")
                    {
                        decoder_.Reset = true;
                    }
                    else if (opt == "-s")
                    {
                        decoder_.Strict = true;
                    }
                    else if (opt == "-t")
                    {
                      readTemplateName = true;
                    }
                    else if (opt == "-f")
                    {
                      readSourceName = true;
                    }
                    else if (opt == "-p")
                    {
                      readPerfName = true;
                    }
                    else if (opt == "-c")
                    {
                      readCount = true;
                    }
                    else if (opt == "-h")
                    {
                      ok = false;
                    }
                    else if (opt == "-i")
                    {
                      readDuplicate = true;
                    }
                    else if (opt == "-l")
                    {
                      readLimit = true;
                    }
                    else
                    {
                        Console.Error.WriteLine("Unknown Option: {0}", opt);
                        ok = false;
                    }

                }

                try
                {
                    if (ok && performanceFileName_ != null)
                    {
                        try
                        {
                            performanceFileStream_ = new System.IO.FileStream(performanceFileName_, System.IO.FileMode.OpenOrCreate);
                            performanceFile_ = new System.IO.StreamWriter(performanceFileStream_);
                        }

                        catch (System.IO.IOException iex)
                        {
                            ok = false;
                            System.Console.WriteLine("ERROR: Can't open performance output file: {0}", performanceFileName_);
                            System.Console.WriteLine(iex.ToString());
                        }
                    }
                    else
                    {
                        performanceFile_ = System.Console.Out;
                    }

                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                    ok = false;
                }

                if (!ok)
                {
                    this.usage();
                }
                return ok;
            }

            public int run()
            {
                int result = 0;
                try
                {
                    QuickFAST.DotNet.DNMessageDeliverer builder = new QuickFAST.DotNet.DNMessageDeliverer();
                    // Handle incoming FAST decoded messages
                    builder.MessageReceived +=
                        new QuickFAST.DotNet.DNMessageDeliverer.MessageReceiver(
                            MessageInterpreter);
                    // Handle log messages
                    builder.LogMessage +=
                        new QuickFAST.DotNet.DNMessageDeliverer.LogHandler(
                            Logger);
                    // Handle communications errors
                    builder.CommunicationError +=
                        new QuickFAST.DotNet.DNMessageDeliverer.ErrorHandler(
                            CommunicationErrorHandler);
                    // Handle decoding errors
                    builder.DecodingError +=
                        new QuickFAST.DotNet.DNMessageDeliverer.ErrorHandler(
                            DecoderErrorHandler);

                    GCSettings.LatencyMode = GCLatencyMode.LowLatency;

                    QuickFAST.DotNet.Stopwatch decodeTimer = new QuickFAST.DotNet.Stopwatch();
                    decoder_.run(builder, 0, true);
                    decodeTimer.Stop();
                    ulong decodeLapse = decodeTimer.ElapsedMilliseconds;

                    GCSettings.LatencyMode = GCLatencyMode.Interactive;

#if DEBUG
                    performanceFile_.Write("[debug] ");
#endif // DEBUG
                    performanceFile_.Write("Decoded {0} messages in {1}  milliseconds. [", recordCount_, decodeLapse);
                    performanceFile_.Write("{0:F3} msec/message. = ", (double)decodeLapse / (double)recordCount_);
                    performanceFile_.WriteLine("{0:F3} messages/second]", (double)1000 * (double)recordCount_ / (double)decodeLapse);
                    performanceFile_.Flush();
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                    result = 1;
                }
                return result;
            }

            public bool MessageInterpreter(
                QuickFAST.DotNet.DNMessageDeliverer builder,
                QuickFAST.DotNet.DNFieldSet message)
            {
                ++recordCount_;
                return true;
            }
            public bool Logger(
                QuickFAST.DotNet.DNMessageDeliverer builder,
                ushort logLevel,
                String message)
            {
                Console.WriteLine("{0}", message);
                return true;
            }

            bool CommunicationErrorHandler(
                QuickFAST.DotNet.DNMessageDeliverer builder,
                String message)
            {
                Console.WriteLine("Communication error: {0}", message);
                return true;
            }

            bool DecoderErrorHandler(
                QuickFAST.DotNet.DNMessageDeliverer builder,
                String message)
            {
                Console.WriteLine("Decoder error: {0}", message);
                return true;
            }

            static int Main(string[] args)
            {
                {
                    PerformanceTestDotNet application = new PerformanceTestDotNet();
                    if (application.init(ref args))
                    {
                        application.run();
                    }
                    else
                    {
                        System.Console.WriteLine("ERROR: Failed initialization.");
                    }
                }
                return 0;
            }
            private QuickFAST.DotNet.DNDecoderConnection decoder_ = new QuickFAST.DotNet.DNDecoderConnection();
            long recordCount_ = 0;
        }
    }
}
