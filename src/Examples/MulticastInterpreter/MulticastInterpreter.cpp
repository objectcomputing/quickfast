// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Examples/ExamplesPch.h>
#include "MulticastInterpreter.h"
#include <Codecs/Decoder.h>
#include <Codecs/XMLTemplateParser.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/DataSourceBuffer.h>
#include <Messages/Message.h>
#include <Examples/StopWatch.h>
#include <Examples/MessageInterpreter.h>
#if 1
#include <Codecs/MulticastReceiver.h>
#include <Codecs/MessageConsumer.h>
#include <Codecs/BufferDecoder.h>
#endif


//////////////
// temp debugging
#include <Codecs/MulticastReceiver.h>

using namespace QuickFAST;
using namespace Examples;

MulticastInterpreter::MulticastInterpreter()
: bufferSize_(1400)
, bufferCount_(4)
, echoType_(Codecs::DataSource::HEX)
, messageLimit_(0)
, strict_(true)
, silent_(false)
, portNumber_(13014)
, listenAddressName_("0.0.0.0")
, multicastAddressName_("224.1.2.133")
, echoMessage_(true)
, echoField_(false)
, outputFile_(0)
, echoFile_(0)
{
}

MulticastInterpreter::~MulticastInterpreter()
{
}


bool
MulticastInterpreter::init(int argc, char * argv[])
{
  commandArgParser_.addHandler(this);
  return commandArgParser_.parse(argc, argv);
}

int
MulticastInterpreter::parseSingleArg(int argc, char * argv[])
{
  int consumed = 0;
  std::string opt(argv[0]);
  try
  {
    if(opt == "-b" && argc > 1)
    {
      bufferSize_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    if(opt == "-#" && argc > 1)
    {
      bufferCount_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    if(opt == "-c" && argc > 1)
    {
      messageLimit_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-p" && argc > 1)
    {
      portNumber_ = boost::lexical_cast<unsigned short>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-a" && argc > 1)
    {
      listenAddressName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-m" && argc > 1)
    {
      multicastAddressName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-s")
    {
      strict_ = !strict_;
      consumed = 1;
    }
    else if(opt == "-q")
    {
      silent_ = !silent_;
      consumed = 1;
    }
    else if(opt == "-t" && argc > 1)
    {
      templateFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-o" && argc > 1)
    {
      outputFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-e" && argc > 1)
    {
      echoFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-ehex")
    {
      echoType_ = Codecs::DataSource::HEX;
      consumed = 1;
    }
    else if(opt == "-eraw")
    {
      echoType_ = Codecs::DataSource::RAW;
      consumed = 1;
    }
    else if(opt == "-enone")
    {
      echoType_ = Codecs::DataSource::NONE;
      consumed = 1;
    }
    else if(opt == "-em")
    {
      echoMessage_ = !echoMessage_;
      consumed = 1;
    }
    else if(opt == "-ef")
    {
      echoField_ = !echoField_;
      consumed = 1;
    }
  }
  catch (std::exception & ex)
  {
    std::cerr << ex.what() << " while interpreting " << opt << std::endl;
    consumed = 0;
  }
  return consumed;
}

void
MulticastInterpreter::usage(std::ostream & out) const
{
  out << "  -b size       : Size of largest expected message. (default " << bufferSize_ << ")" << std::endl;
  out << "  -# count      : Number of buffers. (default " << bufferCount_ << ")" << std::endl;
  out << "  -t file       : Template file (required)" << std::endl;
  out << "  -o file       : Output file (defaults to standard out)" << std::endl;
  out << "  -l dotted_ip  : Connection listen address (default is " << listenAddressName_ << ")" << std::endl;
  out << "  -m dotted_ip  : Multicast address (default is " << multicastAddressName_ << ")" << std::endl;
  out << "  -p port       : Multicast port number (default " << portNumber_ << ")" << std::endl;
  out << "  -c count      : Stop after receiving count messages (default 0 means no limit)" << std::endl;
  out << "  -s            : Apply strict decoding rules." << std::endl;
  out << "  -q            : Quiet." << std::endl;
  out << "  -e file       : Echo input to file" << std::endl;
  out << "    -ehex         : Echo as hexadecimal (default)." << std::endl;
  out << "    -eraw         : Echo as raw binary data" << std::endl;
  out << "    -enone        : Do not echo data (boundaries only)." << std::endl;
  out << "    -em           : Toggle 'echo message boundaries'(default true)" << std::endl;
  out << "    -ef           : Toggle 'echo field boundaries'(default false)" << std::endl;
}

bool
MulticastInterpreter::applyArgs()
{
  bool ok = true;
  try
  {
    if(templateFileName_.empty())
    {
      ok = false;
      std::cerr << "ERROR: -t [templatefile] option is required." << std::endl;
    }
    if(ok)
    {
      templateFile_.open(templateFileName_.c_str(), std::ios::in | std::ios::binary);
      if(!templateFile_.good())
      {
        ok = false;
        std::cerr << "ERROR: Can't open template file: "
          << templateFileName_
          << std::endl;
      }
    }
    if(ok)
    {
      Codecs::XMLTemplateParser parser;
      templateRegistry_ = parser.parse(templateFile_);
      decoder_ = new Codecs::MulticastDecoder(
        templateRegistry_,
        multicastAddressName_,
        listenAddressName_,
        portNumber_);
      decoder_->setStrict(strict_);
      decoder_->setLimit(messageLimit_);
    }

    if(outputFileName_.empty())
    {
      outputFile_ = &std::cout;
    }
    else if(ok)
    {
      outputFile_ = new std::ofstream(outputFileName_.c_str());
      if(!outputFile_->good())
      {
        ok = false;
        std::cerr << "ERROR: Can't open output file: "
          << outputFileName_
          << std::endl;
      }
    }

    if(! echoFileName_.empty())
    {
      std::ios::openmode mode = std::ios::out | std::ios::trunc;
#ifdef _WIN32
      if(echoType_ == Codecs::DataSource::RAW)
      {
        mode |= std::ios::binary;
      }
#endif
      echoFile_ = new std::ofstream(echoFileName_.c_str(), mode);
      if(!echoFile_->good())
      {
        ok = false;
        std::cerr << "ERROR: Can't open echo file: "
          << echoFileName_
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
MulticastInterpreter::run()
{
  int result = 0;

  MessageInterpreter consumer(*outputFile_, silent_);
  Codecs::GenericMessageBuilder builder(consumer);
  decoder_->start(builder, bufferSize_, bufferCount_);

  try
  {
    StopWatch lapse;
    decoder_->run();
    unsigned long receiveLapse = lapse.freeze();
    size_t messageCount = decoder_->messageCount();
      std::cout << "Received "
        << messageCount
        << " messages in "
        << std::fixed << std::setprecision(3)
        << receiveLapse
        << " milliseonds. [";
    if(messageCount > 0)
    {
      std::cout << std::fixed << std::setprecision(3)
        << double(receiveLapse)/double(messageCount) << " msec/message. = "
        << std::fixed << std::setprecision(0)
        << 1000. * double(messageCount)/double(receiveLapse) << " message/second.]"
        << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    result = -1;
  }
  return result;
}


void
MulticastInterpreter::fini()
{
}

