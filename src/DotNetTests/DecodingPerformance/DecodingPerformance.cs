using System;
using System.Collections.Generic;
using System.Text;

namespace QuickFASTDotNet
{
    namespace Tests
    {
        public class DecodingPerformance
        {
            string templateFileName_;
            System.IO.FileStream templateFile_;
            string fastFileName_;
            System.IO.FileStream fastFile_;
            string performanceFileName_;
            System.IO.FileStream performanceFileStream_;
            System.IO.TextWriter performanceFile_;

            bool resetOnMessage_;
            bool strict_;


            public DecodingPerformance()
            {
                templateFileName_ = null;
                fastFileName_ = null;
                performanceFileName_ = null;
            }


            public void usage()
            {
                System.Console.WriteLine("Usage of this program:");
                System.Console.WriteLine("  -t file     : Template file (required)");
                System.Console.WriteLine("  -f file     : FAST Message file (required)");
                System.Console.WriteLine("  -p file     : File to which performance measurements are written. (default standard output)");
                System.Console.WriteLine("  -r          : Toggle 'reset decoder on every message' (default false).");
                System.Console.WriteLine("  -s          : Toggle 'strict decoding rules' (default true).");
            }



            public bool init(ref string[] args)
            {
                bool readTemplateName = false;
                bool readSourceName = false;
                bool readPerfName = false;
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
                    else if (readPerfName)
                    {
                        performanceFileName_ = opt;
                        readPerfName = false;
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
                    else if (opt == "-p")
                    {
                        readPerfName = true;
                    }
                    else if (opt == "-h")
                    {
                        ok = false;
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
                    System.Console.WriteLine("Parsing templates");

                    QuickFASTDotNet.Examples.StopWatch parseTimer = new QuickFASTDotNet.Examples.StopWatch();
                    QuickFASTDotNet.Codecs.TemplateRegistry templateRegistry = QuickFASTDotNet.Codecs.TemplateRegistry.Parse(templateFile_);
                    long parseLapse = parseTimer.freeze();
                    uint templateCount = templateRegistry.Size;
                    performanceFile_.Write("Parsed ");
                    performanceFile_.Write(templateCount);
                    performanceFile_.Write(" templates in ");
                    performanceFile_.Write(parseLapse.ToString("F3"));
                    performanceFile_.WriteLine(" milliseconds.");
                    performanceFile_.Write(" milliseconds. [");
                    performanceFile_.Write("{0:F3} msec/template. = ", (double)parseLapse / (double)templateCount);
                    performanceFile_.WriteLine("{0:F0} template/second.]", (double)1000 * (double)templateCount / (double)parseLapse);
                    performanceFile_.Flush();

//                    for (ulong nPass = 0; nPass < count_; ++nPass)
//                    {
//                        System.Console.WriteLine("Decoding input; pass {0} of {1}", nPass + 1, count_);
//                        System.Console.Out.Flush();
//                        MessageCounter handler = new MessageCounter();

//                        fastFile_.Seek(0, System.IO.SeekOrigin.Begin);
//                        QuickFASTDotNet.Codecs.SynchronousDecoder decoder = new QuickFASTDotNet.Codecs.SynchronousDecoder(templateRegistry, fastFile_);
//                        decoder.ResetOnMessage = resetOnMessage_;
//                        decoder.Strict = strict_;
//                        QuickFASTDotNet.Codecs.MessageReceivedDelegate handlerDelegate;
//                        handlerDelegate = new QuickFASTDotNet.Codecs.MessageReceivedDelegate(handler.MessageReceived);
//                        ulong messageCount = 0;
//                        StopWatch decodeTimer = new StopWatch();
//                        { //PROFILE_POINT("Main");
//                            decoder.Decode(handlerDelegate);
//                        }//PROFILE_POINT
//                        long decodeLapse = decodeTimer.freeze();
//                        messageCount = handler.getMesssageCount();
//                        //                        messageCount = decoder.MessageCount;
//#if DEBUG
//                        performanceFile_.Write("[debug] ");
//#endif // DEBUG
//                        performanceFile_.Write("Decoded {0} messages in ", messageCount);
//                        performanceFile_.Write(decodeLapse.ToString("F3"));
//                        performanceFile_.Write(" milliseonds. [");
//                        performanceFile_.Write("{0:F3} msec/message. = ", (double)decodeLapse / (double)messageCount);
//                        performanceFile_.WriteLine("{0:F3} messages/second]", (double)1000 * (double)messageCount / (double)decodeLapse);
//                        performanceFile_.Flush();
//                    }


                    //if (verboseExecution_)
                    //{
                    //    System.Console.WriteLine("Parsing templates");
                    //}
                    //QuickFASTDotNet.Codecs.TemplateRegistry templateRegistry = QuickFASTDotNet.Codecs.TemplateRegistry.Parse(templateFile_);
                    //if (verboseExecution_)
                    //{
                    //    System.Console.WriteLine("Parsed {0} templates.", templateRegistry.Size);
                    //    System.Console.WriteLine("Decoding messages");
                    //}

                    QuickFASTDotNet.Examples.MessageProcessor handler = new QuickFASTDotNet.Examples.MessageProcessor();
                    QuickFASTDotNet.Codecs.Decoder decoder = new QuickFASTDotNet.Codecs.Decoder(templateRegistry, fastFile_);
                    //QuickFASTDotNet.Codecs.SynchronousDecoder decoder = new QuickFASTDotNet.Codecs.SynchronousDecoder(templateRegistry, fastFile_);
                    decoder.Strict = strict_;
                    QuickFASTDotNet.Codecs.MessageReceivedDelegate handlerDelegate;
                    handlerDelegate = new QuickFASTDotNet.Codecs.MessageReceivedDelegate(handler.MessageReceived);

                    decoder.Decode(handlerDelegate);

                    System.Console.WriteLine("Decoded 1 message in {0} milliseconds.", handler.MesssageTime);
                    System.Console.WriteLine("{0}{1} int32s ", '\t', handler.Int32Count);
                    System.Console.WriteLine("{0}{1} uint32s ", '\t', handler.UInt32Count);
                    System.Console.WriteLine("{0}{1} int64s ", '\t', handler.Int64Count);
                    System.Console.WriteLine("{0}{1} uint64s ", '\t', handler.UInt64Count);
                    System.Console.WriteLine("{0}{1} decimals ", '\t', handler.DecimalCount);
                    System.Console.WriteLine("{0}{1} ascii strs ", '\t', handler.AsciStringCount);
                    System.Console.WriteLine("{0}{1} unicode strs ", '\t', handler.UnicodeStringCount);
                    System.Console.WriteLine("{0}{1} byte vectors ", '\t', handler.ByteVectorCount);
                    System.Console.WriteLine("{0}{1} seqs ", '\t', handler.SequenceCount);
                    System.Console.WriteLine("{0}{1} groups ", '\t', handler.GroupCount);
                    System.Console.WriteLine("{0}{1} non std fields ", '\t', handler.NonStandardCount);



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
                DecodingPerformance app = new DecodingPerformance();

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
