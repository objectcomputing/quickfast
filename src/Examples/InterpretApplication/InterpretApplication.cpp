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

using namespace QuickFAST;
using namespace Examples;

///////////////////////
// InterpretApplication

InterpretApplication::InterpretApplication()
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
      configuration_.templateFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-limit" && argc > 1)
    {
      configuration_.head_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-reset")
    {
      configuration_.reset_ = true;
      consumed = 1;
    }
    else if(opt == "-strict")
    {
      configuration_.strict_ = false;
      consumed = 1;
    }
    else if(opt == "-vo" && argc > 1)
    {
      configuration_.verboseFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-e" && argc > 1)
    {
      configuration_.echoFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-ehex")
    {
      configuration_.echoType_ = Codecs::DataSource::HEX;
      consumed = 1;
    }
    else if(opt == "-eraw")
    {
      configuration_.echoType_ = Codecs::DataSource::RAW;
      consumed = 1;
    }
    else if(opt == "-enone")
    {
      configuration_.echoType_ = Codecs::DataSource::NONE;
      consumed = 1;
    }
    else if(opt == "-em")
    {
      configuration_.echoMessage_ = !configuration_.echoMessage_;
      consumed = 1;
    }
    else if(opt == "-ef")
    {
      configuration_.echoField_ = !configuration_.echoField_;
      consumed = 1;
    }
    else if(opt == "-file" && argc > 1)
    {
      configuration_.receiverType_ = Application::DecoderConfiguration::RAWFILE_RECEIVER;
      configuration_.fastFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-pcap" && argc > 1)
    {
      configuration_.receiverType_ = Application::DecoderConfiguration::PCAPFILE_RECEIVER;
      configuration_.pcapFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "pcapsource" && argc > 1)
    {
      if(argv[1][0] == '6' && argv[1][1] == '4' &&  argv[1][1] == '\0')
      {
        configuration_.pcapWordSize_ = 64;
        consumed = 2;
      }
      else if(argv[1][0] == '3' && argv[1][1] == '2' &&  argv[1][1] == '\0')
      {
        configuration_.pcapWordSize_ = 32;
        consumed = 2;
      }
    }
    else if(opt == "-multicast" && argc > 1)
    {
      configuration_.receiverType_ = Application::DecoderConfiguration::MULTICAST_RECEIVER;
      std::string address = argv[1];
      std::string::size_type colon = address.find(':');
      configuration_.multicastGroupIP_ = address.substr(0, colon);
      if(colon != std::string::npos)
      {
        configuration_.portNumber_ = boost::lexical_cast<unsigned short>(
          address.substr(colon+1));
      }
      consumed = 2;
    }
    else if(opt == "-mlisten" && argc > 1)
    {
      configuration_.listenInterfaceIP_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-tcp" && argc > 1)
    {
      configuration_.receiverType_ = Application::DecoderConfiguration::TCP_RECEIVER;
      std::string address = argv[1];
      std::string::size_type colon = address.find(':');
      configuration_.hostName_ = address.substr(0, colon);
      if(colon != std::string::npos)
      {
        configuration_.portName_ = address.substr(colon+1);
      }
      consumed = 2;
    }
    else if(opt == "-streaming" )
    {
      configuration_.assemblerType_ = Application::DecoderConfiguration::STREAMING_ASSEMBLER;
      consumed = 1;
      configuration_.waitForCompleteMessage_ = false;
      if(argc > 1)
      {
        if(argv[1] == "block")
        {
          consumed = 2;
          configuration_.waitForCompleteMessage_ = true;
        }
        else if(argv[1] == "noblock")
        {
          consumed = 2;
        }
      }
    }
    else if(opt == "-datagram") //           : Message boundaries match packet boundaries (default if Multicast or PCap file).
    {
      configuration_.assemblerType_ = Application::DecoderConfiguration::MESSAGE_PER_PACKET_ASSEMBLER;
      consumed = 1;
    }
    else if(opt == "-hnone" ) //              : No header
    {
      configuration_.headerType_ = Application::DecoderConfiguration::NO_HEADER;
      consumed = 1;
    }
    else if(opt == "-hfix" && argc > 1) // n             : Header contains fixed size fields; block size field is n bytes" << std::endl;
    {
      configuration_.headerType_ = Application::DecoderConfiguration::FIXED_HEADER;
      configuration_.headerMessageSizeBytes_ =  boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-hfast" ) //              : Header contains fast encoded fields" << std::endl;
    {
      configuration_.headerType_ = Application::DecoderConfiguration::FAST_HEADER;
      consumed = 1;
    }
    else if(opt == "-hprefix" && argc > 1) // n            : 'n' bytes (fixed) or fields (FAST) preceed block size" << std::endl;
    {
      configuration_.headerPrefixCount_ =  boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-hsuffix" && argc > 1) // n            : 'n' bytes (fixed) or fields (FAST) follow block size" << std::endl;
    {
      configuration_.headerSuffixCount_ =  boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-hbig" ) //                 : fixed size header is big-endian" << std::endl;
    {
      configuration_.headerBigEndian_ = true;
      consumed = 1;
      if(argc > 1)
      {
        if(argv[1] == "no")
        {
          configuration_.headerBigEndian_ = false;
          consumed = 2;
        }
        else if(argv[1] == "yes")
        {
          configuration_.headerBigEndian_ = true;
          consumed = 2;
        }
      }
    }
    else if(opt == "-buffersize" && argc > 1) // size         : Size of communication buffers. For multicast largest expected message. (default " << bufferSize_ << ")" << std::endl;
    {
      configuration_.bufferSize_ =  boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-buffers" && argc > 1) // count      : Number of buffers. (default " << bufferCount_ << ")" << std::endl;
    {
      configuration_.bufferCount_ =  boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
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
  out << "  -e file              : Echo input to file:" << std::endl;
  out << "    -ehex                : Echo as hexadecimal (default)." << std::endl;
  out << "    -eraw                : Echo as raw binary data." << std::endl;
  out << "    -enone               : Do not echo data (boundaries only)." << std::endl;
  out << "    -em                  : Echo message boundaries" << std::endl;
  out << "                           (default true, switch turns it off)." << std::endl;
  out << "    -ef                  : Echo field boundaries" << std::endl;
  out << "                           (default false)." << std::endl;
  out << std::endl;
  out << "  -file file           : Input from raw FAST message file." << std::endl;
  out << "  -pcap file           : Input from PCap FAST message file." << std::endl;
  out << "  -pcapsource [64|32]    : Word size of the machine where the PCap data was captured." << std::endl;
  out << "                           Defaults to the current platform." << std::endl;
  out << "  -multicast ip:port   : Input from Multicast." << std::endl;
  out << "                         Subscribe to dotted \"ip\" address" << std::endl;
  out << "                         on port number \"port\":" << std::endl;
  out << "  -mlisten ip            : Multicast dotted IP listen address" << std::endl;
  out << "                           (default is " << configuration_.listenInterfaceIP_ << ")." << std::endl;
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
  out << "                         (default " << configuration_.bufferSize_ << ")." << std::endl;
  out << "  -buffers count       : Number of buffers. (default " << configuration_.bufferCount_ << ")." << std::endl;
  out << "                         For \"-streaming block\" buffersize * buffers must" << std::endl;
  out << "                         exceed largest expected message." << std::endl;
}

bool
InterpretApplication::applyArgs()
{
  bool ok = true;
  try
  {
    if(configuration_.templateFileName_.empty())
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
    Codecs::GenericMessageBuilder builder(handler);
    connection_.configure(builder, configuration_);
    // run the event loop in this thread.  Do not return until
    // receiver is stopped.
    connection_.receiver().run();
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
