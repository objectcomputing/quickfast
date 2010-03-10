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
                Console.WriteLine("  -f file     : FAST Message file (required)");
                Console.WriteLine("  -r          : Toggle 'reset decoder on every message' (default false).");
                Console.WriteLine("  -s          : Toggle 'strict decoding rules' (default true).");
                Console.WriteLine("  -vx         : Toggle 'noisy execution progress' (default false).");
            }

            public bool init(ref string[] args)
            {
                bool readTemplateName = false;
                bool readSourceName = false;
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
                    else if (opt == "-vx")
                    {
                        decoder_.VerboseFileName = "cout";
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
                    builder_.MessageReceived +=
                        new QuickFAST.DotNet.DNMessageDeliverer.MessageReceiver(
                            MessageInterpreter);

                    decoder_.run(builder_, 0, true);
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
            private QuickFAST.DotNet.DNMessageDeliverer builder_ = new QuickFAST.DotNet.DNMessageDeliverer();
            private FieldSetInterpreter interpreter_ = new FieldSetInterpreter();
            long recordCount_ = 0;
        }
    }
}
