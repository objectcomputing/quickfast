using System;
using System.Collections.Generic;
using System.Text;

namespace QuickFASTDotNet
{
    namespace Examples
    {
        public class InterpretFASTDotNet
        {
            public InterpretFASTDotNet()
            {
            }

            public void usage()
            {
                Console.WriteLine("Usage of this program:");
                Console.WriteLine("  -t file     : Template file (required)");
                Console.WriteLine("  -f file     : FAST Message file");
                Console.WriteLine("  -b file     : FAST Message file: buffer the data in memory before decoding");
                Console.WriteLine("  -r          : Toggle 'reset decoder on every message' (default false).");
                Console.WriteLine("  -s          : Toggle 'strict decoding rules' (default true).");
                Console.WriteLine("  -vx         : Toggle 'noisy execution progress' (default false).");
                Console.WriteLine("  -e file     : Echo input to file.");
                Console.WriteLine("  -ef         : Echo field names.");
                Console.WriteLine("  -em         : Echo Message Boundaries.");
                Console.WriteLine("  -ex         : Echo as hex (default).");
                Console.WriteLine("  -er         : Echo as raw data.");
                Console.WriteLine("  -silent     : Don't display data.");
            }

            public bool init(ref string[] args)
            {
                bool readTemplateName = false;
                bool readSourceName = false;
                bool readBufferedFileName = false;
                bool readEchoFileName = false;
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
                    else if (readBufferedFileName)
                    {
                        memoryBuffer_ = System.IO.File.ReadAllBytes(opt);
                        decoder_.ReceiverType = QuickFAST.DotNet.DNDecoderConnection.ReceiverTypes.BUFFER_RECEIVER;
                        decoder_.HeaderType = QuickFAST.DotNet.DNDecoderConnection.HeaderTypes.NO_HEADER;
                        readBufferedFileName = false;
                    }
                    else if (readEchoFileName)
                    {
                        decoder_.EchoFileName = opt;
                        readEchoFileName = false;
                    }
                    else if (opt == "-b")
                    {
                        readBufferedFileName = true;
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
                    else if (opt == "-h")
                    {
                        ok = false;
                    }
                    else if (opt == "-e")
                    {
                        readEchoFileName = true;
                    }
                    else if (opt == "-em")
                    {
                        decoder_.EchoMessage = true;
                    }
                    else if (opt == "-ef")
                    {
                        decoder_.EchoField = true;
                    }
                    else if (opt == "-ex")
                    {
                        decoder_.EchoType = QuickFAST.DotNet.DNDecoderConnection.EchoTypes.ECHO_HEX;
                    }
                    else if (opt == "-er")
                    {
                        decoder_.EchoType = QuickFAST.DotNet.DNDecoderConnection.EchoTypes.ECHO_RAW;
                    }
                    else if (opt == "-vx")
                    {
                        decoder_.VerboseFileName = "cout";
                    }

                    else if (opt == "-silent")
                    {
                        interpreter_.Silent = true;
                    }

                    else
                    {
                        Console.Error.WriteLine("Unknown Option: {0}", opt);
                        ok = false;
                    }
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
                    /// Handle incoming FAST decoded messages
                    QuickFAST.DotNet.DNMessageDeliverer builder = new QuickFAST.DotNet.DNMessageDeliverer();
                    builder.MessageReceived +=
                        new QuickFAST.DotNet.DNMessageDeliverer.MessageReceiver(
                            this.MessageInterpreter);
                    builder.LogMessage +=
                        new QuickFAST.DotNet.DNMessageDeliverer.LogHandler(
                            Logger);
                    builder.CommunicationError +=
                        new QuickFAST.DotNet.DNMessageDeliverer.ErrorHandler(
                            CommunicationErrorHandler);
                    builder.DecodingError +=
                        new QuickFAST.DotNet.DNMessageDeliverer.ErrorHandler(
                            DecoderErrorHandler);
                    if (memoryBuffer_ != null)
                    {
                        decoder_.run(builder, memoryBuffer_, 0, memoryBuffer_.Length, true);

                    }
                    else
                    {
                        decoder_.run(builder, 0, true);
                    }
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
                Console.Write("Record #{0} ", recordCount_);
                interpreter_.interpret(message);
                Console.WriteLine();
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
                int result = 0;
                InterpretFASTDotNet app = new InterpretFASTDotNet();

                if (app.init(ref args))
                {
                    result = app.run();
                }
                else
                {
                    Console.WriteLine("ERROR: Failed initialization.");
                    result = 1;
                }
                return result;
            }
            private QuickFAST.DotNet.DNDecoderConnection decoder_ = new QuickFAST.DotNet.DNDecoderConnection();
            private FieldSetInterpreter interpreter_ = new FieldSetInterpreter();
            private long recordCount_ = 0;

            private byte[] memoryBuffer_;
        }
    }
}
