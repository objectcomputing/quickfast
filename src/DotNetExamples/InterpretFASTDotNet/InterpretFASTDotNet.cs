using System;
using System.Collections.Generic;
using System.Text;

namespace QuickFASTDotNet
{
    namespace Examples
    {
        public class InterpretFASTDotNet
        {
            string templateFileName_;
            System.IO.FileStream templateFile_;
            string fastFileName_;
            System.IO.FileStream fastFile_;
            string outputFileName_;
            System.IO.FileStream outputFileStream_;
            System.IO.TextWriter outputFile_;

            bool resetOnMessage_;
            bool strict_;
            bool verboseExecution_;

            public InterpretFASTDotNet()
            {
                templateFileName_ = null;
                fastFileName_ = null;
                outputFileName_ = null;
                resetOnMessage_ = false;
                strict_ = true;
                verboseExecution_ = false;
            }


            public void usage()
            {
                System.Console.WriteLine("Usage of this program:");
                System.Console.WriteLine("  -t file     : Template file (required)");
                System.Console.WriteLine("  -f file     : FAST Message file (required)");
                System.Console.WriteLine("  -o file     : File to which performance measurements are written. (default standard output)");
                System.Console.WriteLine("  -r          : Toggle 'reset decoder on every message' (default false).");
                System.Console.WriteLine("  -s          : Toggle 'strict decoding rules' (default true).");
                System.Console.WriteLine("  -vx         : Toggle 'noisy execution progress' (default false).");
            }



            public bool init(ref string[] args)
            {
                bool readTemplateName = false;
                bool readSourceName = false;
                bool readOutName = false;
                bool ok = true;

                foreach (string opt in args)
                {
                    if (readTemplateName)
                    {
                        templateFileName_ = opt;
                        readTemplateName = false;
                    }
                    else if (readSourceName)
                    {
                        fastFileName_ = opt;
                        readSourceName = false;
                    }
                    else if (readOutName)
                    {
                        outputFileName_ = opt;
                        readOutName = false;
                    }
                    else if (opt == "-r")
                    {
                        resetOnMessage_ = !resetOnMessage_;
                    }
                    else if (opt == "-s")
                    {
                        strict_ = !strict_;
                    }
                    else if (opt == "-t")
                    {
                        readTemplateName = true;
                    }
                    else if (opt == "-f")
                    {
                        readSourceName = true;
                    }
                    else if (opt == "-o")
                    {
                        readOutName = true;
                    }
                    else if (opt == "-h")
                    {
                        ok = false;
                    }
                    else if (opt == "-vx")
                    {
                        verboseExecution_ = !verboseExecution_;
                    }
                }

                try
                {
                    if (templateFileName_ == null)
                    {
                        ok = false;
                        System.Console.WriteLine("ERROR: -t [templatefile] option is required.");
                    }
                    if (ok)
                    {
                        try
                        {

                            templateFile_ = new System.IO.FileStream(templateFileName_, System.IO.FileMode.Open);
                        }
                        catch (System.IO.IOException iex)
                        {
                            ok = false;
                            System.Console.WriteLine("ERROR: Can't open template file: {0}", templateFileName_);
                            System.Console.WriteLine(iex.ToString());
                        }
                    }
                    if (fastFileName_ == null)
                    {
                        ok = false;
                        System.Console.WriteLine("ERROR: -f [FASTfile] option is required.");
                    }
                    if (ok)
                    {
                        try
                        {
                            fastFile_ = new System.IO.FileStream(fastFileName_, System.IO.FileMode.Open);
                        }
                        catch (System.IO.IOException iex)
                        {
                            ok = false;
                            System.Console.WriteLine("ERROR: Can't open FAST Message file: {0}", fastFileName_);
                            System.Console.WriteLine(iex.ToString());
                        }
                    }
                    if (ok && outputFileName_ != null)
                    {
                        try
                        {
                            outputFileStream_ = new System.IO.FileStream(outputFileName_, System.IO.FileMode.OpenOrCreate);
                            outputFile_ = new System.IO.StreamWriter(outputFileStream_);
                        }

                        catch (System.IO.IOException iex)
                        {
                            ok = false;
                            System.Console.WriteLine("ERROR: Can't open performance output file: {0}", outputFileName_);
                            System.Console.WriteLine(iex.ToString());
                        }
                    }
                    else
                    {
                        outputFile_ = System.Console.Out;
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
                    if (verboseExecution_)
                    {
                        System.Console.WriteLine("Parsing templates");
                    }
                    QuickFASTDotNet.Codecs.TemplateRegistry templateRegistry = QuickFASTDotNet.Codecs.TemplateRegistry.Parse(templateFile_);
                    if (verboseExecution_)
                    {
                        System.Console.WriteLine("Parsed {0} templates.", templateRegistry.Size);
                        System.Console.WriteLine("Decoding messages");
                    }

                    MessageInterpreter handler = new MessageInterpreter(outputFile_);
                    QuickFASTDotNet.Codecs.SynchronousDecoder decoder = new QuickFASTDotNet.Codecs.SynchronousDecoder(templateRegistry, fastFile_);
                    decoder.ResetOnMessage = resetOnMessage_;
                    decoder.Strict = strict_;
                    QuickFASTDotNet.Codecs.MessageReceivedDelegate handlerDelegate;
                    handlerDelegate = new QuickFASTDotNet.Codecs.MessageReceivedDelegate(handler.MessageReceived);

                    decoder.Decode(handlerDelegate);
                    if (verboseExecution_)
                    {
                        System.Console.WriteLine("Decoded {0} messages.", decoder.MessageCount);
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                    result = 1;
                }

                return result;
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
                    System.Console.WriteLine("ERROR: Failed initialization.");
                    result = 1;
                }
                return result;
            }
        }
    }
}
