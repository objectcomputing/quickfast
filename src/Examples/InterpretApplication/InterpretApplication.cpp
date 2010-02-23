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

namespace
{
#ifdef _WIN32
  const std::ios::openmode binaryMode = std::ios::binary;
#else
  const std::ios::openmode binaryMode = static_cast<std::ios::openmode>(0);
#endif
}

///////////////////////
// InterpretApplication

InterpretApplication::InterpretApplication()
  : fastFile_(0)
  , echoFile_(0)
  , verboseFile_(0)
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
      config_.templateFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-limit" && argc > 1)
    {
      config_.head_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-reset")
    {
      config_.reset_ = true;
      consumed = 1;
    }
    else if(opt == "-strict")
    {
      config_.strict_ = false;
      consumed = 1;
    }
    else if(opt == "-vo" && argc > 1)
    {
      config_.verboseFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-e" && argc > 1)
    {
      config_.echoFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-ehex")
    {
      config_.echoType_ = Codecs::DataSource::HEX;
      consumed = 1;
    }
    else if(opt == "-eraw")
    {
      config_.echoType_ = Codecs::DataSource::RAW;
      consumed = 1;
    }
    else if(opt == "-enone")
    {
      config_.echoType_ = Codecs::DataSource::NONE;
      consumed = 1;
    }
    else if(opt == "-em")
    {
      config_.echoMessage_ = !config_.echoMessage_;
      consumed = 1;
    }
    else if(opt == "-ef")
    {
      config_.echoField_ = !config_.echoField_;
      consumed = 1;
    }
    else if(opt == "-file" && argc > 1)
    {
      config_.receiverType_ = DecoderConfiguration::RAWFILE_RECEIVER;
      config_.fastFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-pcap" && argc > 1)
    {
      config_.receiverType_ = DecoderConfiguration::PCAPFILE_RECEIVER;
      config_.pcapFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-multicast" && argc > 1)
    {
      config_.receiverType_ = DecoderConfiguration::MULTICAST_RECEIVER;
      std::string address = argv[1];
      std::string::size_type colon = address.find(':');
      config_.multicastGroupIP_ = address.substr(0, colon);
      if(colon != std::string::npos)
      {
        config_.portNumber_ = boost::lexical_cast<unsigned short>(
          address.substr(colon+1));
      }
      consumed = 2;
    }
    else if(opt == "-mlisten" && argc > 1)
    {
      config_.listenInterfaceIP_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-tcp" && argc > 1)
    {
      config_.receiverType_ = DecoderConfiguration::TCP_RECEIVER;
      std::string address = argv[1];
      std::string::size_type colon = address.find(':');
      config_.hostName_ = address.substr(0, colon);
      if(colon != std::string::npos)
      {
        config_.portName_ = address.substr(colon+1);
      }
      consumed = 2;
    }
    else if(opt == "-streaming" )
    {
      config_.assemblerType_ = DecoderConfiguration::STREAMING_ASSEMBLER;
      consumed = 1;
      config_.waitForCompleteMessage_ = false;
      if(argc > 1)
      {
        if(argv[1] == "block")
        {
          consumed = 2;
          config_.waitForCompleteMessage_ = true;
        }
        else if(argv[1] == "noblock")
        {
          consumed = 2;
        }
      }
    }
    else if(opt == "-datagram") //           : Message boundaries match packet boundaries (default if Multicast or PCap file).
    {
      config_.assemblerType_ = DecoderConfiguration::MESSAGE_PER_PACKET_ASSEMBLER;
      consumed = 1;
    }
    else if(opt == "-hnone" ) //              : No header
    {
      config_.headerType_ = DecoderConfiguration::NO_HEADER;
      consumed = 1;
    }
    else if(opt == "-hfix" && argc > 1) // n             : Header contains fixed size fields; block size field is n bytes" << std::endl;
    {
      config_.headerType_ = DecoderConfiguration::FIXED_HEADER;
      config_.headerMessageSizeBytes_ =  boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-hfast" ) //              : Header contains fast encoded fields" << std::endl;
    {
      config_.headerType_ = DecoderConfiguration::FAST_HEADER;
      consumed = 1;
    }
    else if(opt == "-hprefix" && argc > 1) // n            : 'n' bytes (fixed) or fields (FAST) preceed block size" << std::endl;
    {
      config_.headerPrefixCount_ =  boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-hsuffix" && argc > 1) // n            : 'n' bytes (fixed) or fields (FAST) follow block size" << std::endl;
    {
      config_.headerSuffixCount_ =  boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-hbig" ) //                 : fixed size header is big-endian" << std::endl;
    {
      config_.headerBigEndian_ = true;
      consumed = 1;
      if(argc > 1)
      {
        if(argv[1] == "no")
        {
          config_.headerBigEndian_ = false;
          consumed = 2;
        }
        else if(argv[1] == "yes")
        {
          config_.headerBigEndian_ = true;
          consumed = 2;
        }
      }
    }
    else if(opt == "-buffersize" && argc > 1) // size         : Size of communication buffers. For multicast largest expected message. (default " << bufferSize_ << ")" << std::endl;
    {
      config_.bufferSize_ =  boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-buffers" && argc > 1) // count      : Number of buffers. (default " << bufferCount_ << ")" << std::endl;
    {
      config_.bufferCount_ =  boost::lexical_cast<size_t>(argv[1]);
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
  out << "  -multicast ip:port   : Input from Multicast." << std::endl;
  out << "                         Subscribe to dotted \"ip\" address" << std::endl;
  out << "                         on port number \"port\":" << std::endl;
  out << "  -mlisten ip            : Multicast dotted IP listen address" << std::endl;
  out << "                           (default is " << config_.listenInterfaceIP_ << ")." << std::endl;
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
  out << "                         (default " << config_.bufferSize_ << ")." << std::endl;
  out << "  -buffers count       : Number of buffers. (default " << config_.bufferCount_ << ")." << std::endl;
  out << "                         For \"-streaming block\" buffersize * buffers must" << std::endl;
  out << "                         exceed largest expected message." << std::endl;
}

bool
InterpretApplication::applyArgs()
{
  bool ok = true;
  try
  {
    if(config_.templateFileName_.empty())
    {
      ok = false;
      std::cerr << "ERROR: -t [templatefile] option is required." << std::endl;
    }
    if(!config_.fastFileName_.empty())
    {
      fastFile_ = new std::ifstream(config_.fastFileName_.c_str(), binaryMode);
      if(!fastFile_->good())
      {
        ok = false;
        std::cerr << "ERROR: Can't open FAST data file: "
          << config_.fastFileName_
          << std::endl;
      }
    }

    if(!config_.verboseFileName_.empty())
    {
      if(config_.verboseFileName_ == "cerr")
      {
        verboseFile_= &std::cerr;
      }
      else if(config_.verboseFileName_ == "cout")
      {
        verboseFile_ = &std::cout;
      }
      else
      {
        verboseFile_ = new std::ofstream(config_.verboseFileName_.c_str());
        if(!verboseFile_->good())
        {
          ok = false;
          std::cerr << "ERROR: Can't open verbose file: "
            << config_.verboseFileName_
            << std::endl;
        }
      }
    }

    if(! config_.echoFileName_.empty())
    {

      std::ios::openmode mode = std::ios::out | std::ios::trunc;
      if(config_.echoType_ == Codecs::DataSource::RAW)
      {
        mode |= binaryMode;
      }
      if(config_.echoFileName_ == "cout")
      {
        echoFile_ = & std::cout;
      }
      else if(config_.echoFileName_ == "cerr")
      {
        echoFile_ = & std::cerr;
      }
      else
      {
        echoFile_ = new std::ofstream(config_.echoFileName_.c_str(), mode);
      }
      if(!echoFile_->good())
      {
        ok = false;
        std::cerr << "ERROR: Can't open echo file: "
          << config_.echoFileName_
          << std::endl;
      }
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
    std::ifstream templates(config_.templateFileName_.c_str());
    if(!templates.good())
    {
        std::cerr << "ERROR: Can't open template file: "
          << config_.templateFileName_
          << std::endl;
        return -1;
    }
    Codecs::XMLTemplateParser parser;
    parser.setVerboseOutput(*verboseFile_);

    Codecs::TemplateRegistryPtr registry = parser.parse(templates);

    boost::scoped_ptr<Codecs::HeaderAnalyzer> analyzer;
    switch(config_.headerType_)
    {
    case DecoderConfiguration::NO_HEADER:
      {
        analyzer.reset(new Codecs::NoHeaderAnalyzer);
        break;
      }
    case DecoderConfiguration::FIXED_HEADER:
      {
        analyzer.reset(new Codecs::FixedSizeHeaderAnalyzer(
          config_.headerMessageSizeBytes_,
          config_.headerBigEndian_,
          config_.headerPrefixCount_,
          config_.headerSuffixCount_
        ));
        break;
      }
    case DecoderConfiguration::FAST_HEADER:
      {
        analyzer.reset(new Codecs::FastEncodedHeaderAnalyzer(
          config_.headerPrefixCount_,
          config_.headerSuffixCount_,
          config_.headerMessageSizeBytes_ != 0)); // true if header contains message size
        break;
      }
    }
    MessageInterpreter handler(std::cout);
    Codecs::GenericMessageBuilder builder(handler);

    boost::scoped_ptr<Communication::Assembler> assembler;
    switch(config_.assemblerType_)
    {
    case DecoderConfiguration::MESSAGE_PER_PACKET_ASSEMBLER:
      {
        Codecs::MessagePerPacketAssembler * pAssembler = new Codecs::MessagePerPacketAssembler(
          registry,
          *analyzer,
          builder);
        assembler.reset(pAssembler);
        pAssembler->setEcho(*echoFile_, config_.echoType_, config_.echoMessage_, config_.echoField_);
        pAssembler->setMessageLimit(config_.head_);
        break;
      }
    case DecoderConfiguration::STREAMING_ASSEMBLER:
      {
        Codecs::StreamingAssembler * pAssembler = new Codecs::StreamingAssembler(
          registry,
          *analyzer,
          builder,
          config_.waitForCompleteMessage_);
        assembler.reset(pAssembler);
        pAssembler->setEcho(*echoFile_, config_.echoType_, config_.echoMessage_, config_.echoField_);
        pAssembler->setMessageLimit(config_.head_);
        break;
      }
    default:
      {
        switch(config_.receiverType_)
        {
        case DecoderConfiguration::MULTICAST_RECEIVER:
        case DecoderConfiguration::PCAPFILE_RECEIVER:
          {
            Codecs::MessagePerPacketAssembler * pAssembler = new Codecs::MessagePerPacketAssembler(
              registry,
              *analyzer,
              builder);
            assembler.reset(pAssembler);
            pAssembler->setEcho(*echoFile_, config_.echoType_, config_.echoMessage_, config_.echoField_);
            pAssembler->setMessageLimit(config_.head_);
            break;
          }
        case DecoderConfiguration::TCP_RECEIVER:
        case DecoderConfiguration::RAWFILE_RECEIVER:
          {
            Codecs::StreamingAssembler * pAssembler = new Codecs::StreamingAssembler(
              registry,
              *analyzer,
              builder,
              config_.waitForCompleteMessage_);
            assembler.reset(pAssembler);
            pAssembler->setEcho(*echoFile_, config_.echoType_, config_.echoMessage_, config_.echoField_);
            pAssembler->setMessageLimit(config_.head_);
            break;
          }
        default:
          {
            std::cerr << "ERROR: Cannot determine what type of assembler to use." << std::endl;
            return -1;
          }
        }
      }
    }

    boost::scoped_ptr<Communication::Receiver> receiver;
    switch(config_.receiverType_)
    {
    case DecoderConfiguration::MULTICAST_RECEIVER:
      {
        receiver.reset(new Communication::MulticastReceiver(
          config_.multicastGroupIP_,
          config_.listenInterfaceIP_,
          config_.portNumber_));
        break;
      }
    case DecoderConfiguration::TCP_RECEIVER:
      {
        receiver.reset(new Communication::TCPReceiver(
        config_.hostName_,
        config_.portName_));
        break;
      }
    case DecoderConfiguration::RAWFILE_RECEIVER:
      {
        receiver.reset(new Communication::RawFileReceiver(
          *fastFile_));
        break;
      }
    case DecoderConfiguration::PCAPFILE_RECEIVER:
      {
        receiver.reset(new Communication::PCapFileReceiver(
          config_.pcapFileName_));
        break;
      }
    }

    receiver->start(*assembler, config_.bufferSize_, config_.bufferCount_);

    // run the event loop in this thread.  Do not return until
    // receiver is stopped.
    receiver->run();
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
