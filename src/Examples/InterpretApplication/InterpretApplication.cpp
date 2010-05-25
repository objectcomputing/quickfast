// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Examples/ExamplesPch.h>
#include "InterpretApplication.h"
#include <Codecs/XMLTemplateParser.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/GenericMessageBuilder.h>
#include <Codecs/MessagePerPacketAssembler.h>
#include <Codecs/StreamingAssembler.h>

#include <Codecs/NoHeaderAnalyzer.h>
#include <Codecs/FixedSizeHeaderAnalyzer.h>
#include <Codecs/FastEncodedHeaderAnalyzer.h>

#include <Communication/MulticastReceiver.h>
#include <Communication/TCPReceiver.h>
#include <Communication/RawFileReceiver.h>
#include <Communication/PCapFileReceiver.h>

#include <Examples/MessageInterpreter.h>
#include <Examples/ValueToFix.h>

using namespace QuickFAST;
using namespace Examples;

///////////////////////
// InterpretApplication

InterpretApplication::InterpretApplication()
: console_(false)
, fixOutput_(false)
{
}

InterpretApplication::~InterpretApplication()
{
}

bool
InterpretApplication::init(int argc, char * argv[])
{
  commandArgParser_.addHandler(this);
  return commandArgParser_.parse(argc, argv);
}

int
InterpretApplication::parseSingleArg(int argc, char * argv[])
{
  int consumed = 0;
  std::string opt(argv[0]);
  try
  {
    if(opt == "-t" && argc > 1)
    {
      configuration_.setTemplateFileName(argv[1]);
      consumed = 2;
    }
    else if(opt == "-limit" && argc > 1)
    {
      configuration_.setHead(boost::lexical_cast<size_t>(argv[1]));
      consumed = 2;
    }
    else if(opt == "-reset")
    {
      configuration_.setReset(true);
      consumed = 1;
    }
    else if(opt == "-strict")
    {
      configuration_.setStrict(false);
      consumed = 1;
    }
    else if(opt == "-vo" && argc > 1)
    {
      configuration_.setVerboseFileName(argv[1]);
      consumed = 2;
    }
    else if(opt == "-e" && argc > 1)
    {
      configuration_.setEchoFileName(argv[1]);
      consumed = 2;
    }
    else if(opt == "-ehex")
    {
      configuration_.setEchoType(Codecs::DataSource::HEX);
      consumed = 1;
    }
    else if(opt == "-eraw")
    {
      configuration_.setEchoType(Codecs::DataSource::RAW);
      consumed = 1;
    }
    else if(opt == "-enone")
    {
      configuration_.setEchoType(Codecs::DataSource::NONE);
      consumed = 1;
    }
    else if(opt == "-em")
    {
      configuration_.setEchoMessage(true);
      consumed = 1;
    }
    else if(opt == "-em-")
    {
      configuration_.setEchoMessage(false);
      consumed = 1;
    }
    else if(opt == "-ef")
    {
      configuration_.setEchoField(true);
      consumed = 1;
    }
    else if(opt == "-ef-")
    {
      configuration_.setEchoField(false);
      consumed = 1;
    }
    else if(opt == "-file" && argc > 1)
    {
      configuration_.setReceiverType(Application::DecoderConfiguration::RAWFILE_RECEIVER);
      configuration_.setFastFileName(argv[1]);
      consumed = 2;
    }
    else if(opt == "-buffer" && argc > 1)
    {
      configuration_.setReceiverType(Application::DecoderConfiguration::BUFFER_RECEIVER);
      bufferFilename_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-pcap" && argc > 1)
    {
      configuration_.setReceiverType(Application::DecoderConfiguration::PCAPFILE_RECEIVER);
      configuration_.setPcapFileName(argv[1]);
      consumed = 2;
    }
    else if(opt == "pcapsource" && argc > 1)
    {
      if(argv[1][0] == '6' && argv[1][1] == '4' &&  argv[1][1] == '\0')
      {
        configuration_.setPcapWordSize(64);
        consumed = 2;
      }
      else if(argv[1][0] == '3' && argv[1][1] == '2' &&  argv[1][1] == '\0')
      {
        configuration_.setPcapWordSize(32);
        consumed = 2;
      }
    }
    else if(opt == "-multicast" && argc > 1)
    {
      configuration_.setReceiverType(Application::DecoderConfiguration::MULTICAST_RECEIVER);
      std::string address = argv[1];
      std::string::size_type colon = address.find(':');
      configuration_.setMulticastGroupIP(address.substr(0, colon));
      if(colon != std::string::npos)
      {
        configuration_.setPortNumber(boost::lexical_cast<unsigned short>(
          address.substr(colon+1)));
      }
      consumed = 2;
    }
    else if(opt == "-mlisten" && argc > 1)
    {
      configuration_.setListenInterfaceIP(argv[1]);
      consumed = 2;
    }
    else if(opt == "-tcp" && argc > 1)
    {
      configuration_.setReceiverType(Application::DecoderConfiguration::TCP_RECEIVER);
      std::string address = argv[1];
      std::string::size_type colon = address.find(':');
      configuration_.setHostName(address.substr(0, colon));
      if(colon != std::string::npos)
      {
        configuration_.setPortName(address.substr(colon+1));
      }
      consumed = 2;
    }
    else if(opt == "-streaming" )
    {
      configuration_.setAssemblerType(Application::DecoderConfiguration::STREAMING_ASSEMBLER);
      consumed = 1;
      configuration_.setWaitForCompleteMessage(false);
      if(argc > 1)
      {
        if(argv[1] == "block")
        {
          consumed = 2;
          configuration_.setWaitForCompleteMessage(true);
        }
        else if(argv[1] == "noblock")
        {
          consumed = 2;
        }
      }
    }
    else if(opt == "-datagram") //           : Message boundaries match packet boundaries (default if Multicast or PCap file).
    {
      configuration_.setAssemblerType(Application::DecoderConfiguration::MESSAGE_PER_PACKET_ASSEMBLER);
      consumed = 1;
    }
    else if(opt == "-hnone" ) //              : No header
    {
      configuration_.setHeaderType(Application::DecoderConfiguration::NO_HEADER);
      consumed = 1;
    }
    else if(opt == "-hfix" && argc > 1) // n             : Header contains fixed size fields; block size field is n bytes" << std::endl;
    {
      configuration_.setHeaderType(Application::DecoderConfiguration::FIXED_HEADER);
      configuration_.setHeaderMessageSizeBytes(boost::lexical_cast<size_t>(argv[1]));
      consumed = 2;
    }
    else if(opt == "-hfast" ) //              : Header contains fast encoded fields" << std::endl;
    {
      configuration_.setHeaderType(Application::DecoderConfiguration::FAST_HEADER);
      consumed = 1;
    }
    else if(opt == "-hprefix" && argc > 1) // n            : 'n' bytes (fixed) or fields (FAST) preceed block size" << std::endl;
    {
      configuration_.setHeaderPrefixCount(boost::lexical_cast<size_t>(argv[1]));
      consumed = 2;
    }
    else if(opt == "-hsuffix" && argc > 1) // n            : 'n' bytes (fixed) or fields (FAST) follow block size" << std::endl;
    {
      configuration_.setHeaderSuffixCount(boost::lexical_cast<size_t>(argv[1]));
      consumed = 2;
    }
    else if(opt == "-hbig" ) //                 : fixed size header is big-endian" << std::endl;
    {
      configuration_.setHeaderBigEndian(true);
      consumed = 1;
      if(argc > 1)
      {
        if(argv[1] == "no")
        {
          configuration_.setHeaderBigEndian(false);
          consumed = 2;
        }
        else if(argv[1] == "yes")
        {
          configuration_.setHeaderBigEndian(true);
          consumed = 2;
        }
      }
    }
    else if(opt == "-testskip" && argc > 1)
    {
      configuration_.setTestSkip(boost::lexical_cast<size_t>(argv[1]));
      consumed = 2;
    }
    else if(opt == "-buffersize" && argc > 1) // size         : Size of communication buffers. For multicast largest expected message. (default " << bufferSize_ << ")" << std::endl;
    {
      configuration_.setBufferSize(boost::lexical_cast<size_t>(argv[1]));
      consumed = 2;
    }
    else if(opt == "-buffers" && argc > 1) // count      : Number of buffers. (default " << bufferCount_ << ")" << std::endl;
    {
      configuration_.setBufferCount(boost::lexical_cast<size_t>(argv[1]));
      consumed = 2;
    }
    else if(opt == "-nonstandard" && argc > 1)
    {
      configuration_.setNonstandard(boost::lexical_cast<unsigned long>(argv[1]));
      consumed = 2;
    }
    else if(opt == "-ofix")
    {
      fixOutput_ = true;
      consumed = 1;
    }
    else if(opt == "-console")
    {
      console_ = true;
      consumed = 1;
    }
  }
  catch (std::exception & ex)
  {
    // because the lexical cast can throw
    std::cerr << ex.what() << " while interpreting " << opt << std::endl;
    consumed = 0;
  }
  return consumed;
}

void
InterpretApplication::usage(std::ostream & out) const
{
  out << "  -t file              : Template file (required)." << std::endl;
  out << "  -limit n             : Process only the first 'n' messages." << std::endl;
  out << "  -reset               : Toggle 'reset decoder on" << std::endl;
  out << "                         every message' (default false)." << std::endl;
  out << "  -strict              : Toggle 'strict decoding rules'" << std::endl;
  out << "                         (default true)." << std::endl;
  out << "  -vo filename         : Write verbose output to file" << std::endl;
  out << "                         (cout for standard out;" << std::endl;
  out << "                         cerr for standard error)." << std::endl;
  out << std::endl;
  out << "  -console             : While running, listen for single character commands from the console." << std::endl;
  out << "                         Currently defined commands are:" << std::endl;
  out << "                          <q>uit.    : exit the program." << std::endl;
  out << "                          <p>ause.   : temporarily unsubscribe from the multicast feed" << std::endl;
  out << "                          <r>esume.  : resubscribe after a pause." << std::endl;
  out << "                         Note that you must hit ENTER before the command will be recognized." << std::endl;
  out << std::endl;
  out << "  -ofix                : Write the output as newline separated FIX records." << std::endl;
  out << std::endl;
  out << "  -file file           : Input from raw FAST message file." << std::endl;
  out << "  -buffer file         : Input from raw FAST message file into a buffer; decode from buffer." << std::endl;
  out << "  -pcap file           : Input from PCap FAST message file." << std::endl;
  out << "  -pcapsource [64|32]    : Word size of the machine where the PCap data was captured." << std::endl;
  out << "                           Defaults to the current platform." << std::endl;
  out << "  -multicast ip:port   : Input from Multicast." << std::endl;
  out << "                         Subscribe to dotted \"ip\" address" << std::endl;
  out << "                         on port number \"port\":" << std::endl;
  out << "  -mlisten ip            : Multicast dotted IP listen address" << std::endl;
  out << "                           (default is " << configuration_.listenInterfaceIP() << ")." << std::endl;
  out << "                           Select local network interface (NIC)" << std::endl;
  out << "                           on which to subscribe and listen." << std::endl;
  out << "                           0.0.0.0 means pick any NIC." << std::endl;
  out << "  -tcp host:port       : Input from TCP/IP.  Connect to \"host\" name or" << std::endl;
  out << "                         dotted IP on named or numbered port." << std::endl;
  out << std::endl;
  out << "  -streaming [no]block : Message boundaries do not match packet" << std::endl;
  out << "                         boundaries (default if TCP/IP or raw file)." << std::endl;
  out << "                         noblock means decoding doesn't start until" << std::endl;
  out << "                           a complete message has arrived." << std::endl;
  out << "                           No thread will be blocked waiting" << std::endl;
  out << "                           input if this option is used." << std::endl;
  out << "                         block means the decoding starts immediately" << std::endl;
  out << "                           The decoding thread may block for more data." << std::endl;
  out << "  -datagram            : Message boundaries match packet boundaries" << std::endl;
  out << "                         (default if Multicast or PCap file)." << std::endl;
  out << std::endl;
  out << "  -hnone               : No header(preamble) in FAST data (default)." << std::endl;
  out << "  -hfix n              : Header contains fixed size fields;" << std::endl;
  out << "                         block size field is n bytes:" << std::endl;
  out << "  -hbig                  : fixed size header is big-endian." << std::endl;
  out << "  -hfast               : Header contains fast encoded fields:" << std::endl;
  out << "  -hprefix n             : 'n' bytes (fixed) or fields (FAST) precede" << std::endl;
  out << "                           block size." << std::endl;
  out << "  -hsuffix n             : 'n' bytes (fixed) or fields (FAST) follow" << std::endl;
  out << "                           block size." << std::endl;
  out << std::endl;
  out << "  -buffersize size     : Size of communication buffers." << std::endl;
  out << "                         For \"-datagram\" largest expected message." << std::endl;
  out << "                         (default " << configuration_.bufferSize() << ")." << std::endl;
  out << "  -buffers count       : Number of buffers. (default " << configuration_.bufferCount() << ")." << std::endl;
  out << "                         For \"-streaming block\" buffersize * buffers must" << std::endl;
  out << "                         exceed largest expected message." << std::endl;
  out << std::endl;
  out << "  -e file              : Echo input to file:" << std::endl;
  out << "    -ehex                : Echo as hexadecimal (default)." << std::endl;
  out << "    -eraw                : Echo as raw binary data." << std::endl;
  out << "    -enone               : Do not echo data (boundaries only)." << std::endl;
  out << "    -em / -em-           : Echo message boundaries on/off. (default on)" << std::endl;
  out << "    -ef / -ef-           : Echo field boundaries on/off (default off)" << std::endl;
  out << std::endl;
  out << "  -nonstandard n       : enable nonstandard features (or bits together)" << std::endl;
  out << "                         : 1 is allow presence attribute on length element (Shanghai Exchange)" << std::endl;
}

bool
InterpretApplication::applyArgs()
{
  bool ok = true;
  try
  {
    if(configuration_.templateFileName().empty())
    {
      ok = false;
      std::cerr << "ERROR: -t [templatefile] option is required." << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    ok = false;
  }
  if(!ok)
  {
    commandArgParser_.usage(std::cerr);
  }
  return ok;
}

int
InterpretApplication::run()
{
  int result = 0;
  try
  {

    MessageInterpreter handler(std::cout);
    Messages::ValueMessageBuilderPtr builder;
    if(fixOutput_)
    {
      builder.reset(new ValueToFix(std::cout));
    }
    else
    {
      builder.reset(new Codecs::GenericMessageBuilder(handler));
    }


    connection_.configure(*builder, configuration_);

    if(console_)
    {
      // start a thread to receive data, but reserve this thread for console input
      connection_.receiver().runThreads(1, false);
      bool more = true;
      while(more)
      {
        char c;
        std::cin >> c;
        if(isgraph(c))
        {
          if(c == 'q')
          {
            more = false;
            connection_.receiver().stop();
            connection_.receiver().joinThreads();
          }
          else if (c == 'p')
          {
            connection_.receiver().pause();
          }
          else if( c == 'r')
          {
            connection_.receiver().resume();
          }
          else
          {
            std::cout << "(q)uit; (p)ause; (r)esume" << std::endl;
          }
        }
      }
    }
    else if(!bufferFilename_.empty())
    {
      std::FILE * bufferFile = std::fopen(bufferFilename_.c_str(),
#ifdef _WIN32
        "rb"
#else
        "r"
#endif
        );
      if(bufferFile <= 0)
      {
        std::cerr << "Can't open file " << bufferFilename_ << std::endl;
        return -1;
      }
      std::fseek(bufferFile, 0, SEEK_END);
      size_t fileSize = std::ftell(bufferFile);
      std::fseek(bufferFile, 0, SEEK_SET);
      boost::scoped_array<unsigned char> buffer(new unsigned char[fileSize]);
      std::fread(buffer.get(), 1, fileSize, bufferFile);
      connection_.receiver().receiveBuffer(buffer.get(), fileSize);
    }
    else
    {
      // run the event loop in this thread.  Do not return until
      // receiver is stopped.
      connection_.receiver().run();
    }
  }

  catch (std::exception & e)
  {
    std::cerr << e.what() << std::endl;
    result = -1;
  }
  return result;
}

void
InterpretApplication::fini()
{
}
