// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Examples/ExamplesPch.h>
#include "MulticastDecoder.h"
#include <Codecs/Decoder.h>
#include <Codecs/XMLTemplateParser.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/DataSourceBuffer.h>
#include <Messages/Message.h>
#include <Examples/MessageInterpreter.h>
#include <Examples/StopWatch.h>

using namespace QuickFAST;

MulticastDecoder::MulticastDecoder()
: bufferSize_(5000)
, outputFile_(0)
, echoFile_(0)
, echoType_(Codecs::DataSource::HEX)
, echoMessage_(true)
, echoField_(false)
, messageLimit_(0)
, verboseDecode_(false)
, verboseExecution_(false)
, strict_(true)
, portNumber_(30001)
, listenAddressName_("0.0.0.0")
, multicastAddressName_("239.255.0.1")
, strand_(ioService_)
, socket_(ioService_)
, inputCount_(0)
{
}

MulticastDecoder::~MulticastDecoder()
{
}


bool
MulticastDecoder::init(int argc, char * argv[])
{
  commandArgParser_.addHandler(this);
  return commandArgParser_.parse(argc, argv);
}

int
MulticastDecoder::parseSingleArg(int argc, char * argv[])
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
    else if(opt == "-vd")
    {
      verboseDecode_ = !verboseDecode_;
      consumed = 1;
    }
    else if(opt == "-vx")
    {
      verboseExecution_ = !verboseExecution_;
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
MulticastDecoder::usage(std::ostream & out) const
{
  out << "  -b size       : Size of largest expected message. (default 5000)" << std::endl;
  out << "  -t file       : Template file (required)" << std::endl;
  out << "  -o file       : Output file (defaults to standard out)" << std::endl;
  out << "  -l dotted_ip  : Connection listen address (default is 0.0.0.0)" << std::endl;
  out << "  -m dotted_ip  : Multicast address (default is 239.255.0.1)" << std::endl;
  out << "  -p port       : Multicast port number (default 30001)" << std::endl;
  out << "  -c count      : Stop after receiving count messages (default 0 means no limit)" << std::endl;
  out << "  -s            : Apply strict decoding rules." << std::endl;
  out << "  -e file       : Echo input to file" << std::endl;
  out << "    -ehex         : Echo as hexadecimal (default)." << std::endl;
  out << "    -eraw         : Echo as raw binary data" << std::endl;
  out << "    -enone        : Do not echo data (boundaries only)." << std::endl;
  out << "    -em           : Toggle 'echo message boundaries'(default true)" << std::endl;
  out << "    -ef           : Toggle 'echo field boundaries'(default false)" << std::endl;
  out << "  -vd           : Noise to the console decoding" << std::endl;
  out << "  -vx           : Noise to the console about execution status" << std::endl;
}

bool
MulticastDecoder::applyArgs()
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
      templateFile_.open(templateFileName_.c_str());
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
      decoder_ = new Codecs::Decoder(parser.parse(templateFile_));
      decoder_->setStrict(strict_);
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

    consumer_ = new MessageInterpreter(*outputFile_);

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

    multicastAddress_ = boost::asio::ip::address::from_string(multicastAddressName_);
    listenAddress_ = boost::asio::ip::address::from_string(listenAddressName_);
    endpoint_ = boost::asio::ip::udp::endpoint(listenAddress_, portNumber_);
    socket_.open(endpoint_.protocol());
    socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket_.bind(endpoint_);
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

  data_.reset(new unsigned char[bufferSize_]);
  data2_.reset(new unsigned char[bufferSize_]);
  return ok;
}

int
MulticastDecoder::run()
{
  int result = 0;
  try
  {
    // Join the multicast group.
    socket_.set_option(
        boost::asio::ip::multicast::join_group(multicastAddress_));
    startReceive(&data_);
    startReceive(&data2_);

    StopWatch lapse;
//    boost::thread t(boost::bind(&boost::asio::io_service::run, &ioService_));
    ioService_.run();
//    t.join();
    unsigned long receiveLapse = lapse.freeze();
    std::cout << "Received "
      << inputCount_
      << " messages in "
      << std::fixed << std::setprecision(3)
      << receiveLapse
      << " milliseonds. [";
    std::cout << std::fixed << std::setprecision(3)
      << double(receiveLapse)/double(inputCount_) << " msec/message. = "
      << std::fixed << std::setprecision(0)
      << 1000. * double(inputCount_)/double(receiveLapse) << " message/second.]"
      << std::endl;

  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    result = -1;
  }
  return result;
}

void
MulticastDecoder::startReceive(Buffer * data)
{
  socket_.async_receive_from(
    boost::asio::buffer(data->get(), bufferSize_),
    senderEndpoint_,
    strand_.wrap(
      boost::bind(&MulticastDecoder::handleReceive,
        this,
        boost::asio::placeholders::error,
        data,
        boost::asio::placeholders::bytes_transferred)
        )
      );
}

void
MulticastDecoder::handleReceive(
  const boost::system::error_code& error,
  Buffer * data,
  size_t bytesReceived)
{
  if (!error)
  {
    ++inputCount_;
    if(verboseExecution_)
    {
      std::cout << "Received[" << inputCount_ << "]: " << bytesReceived << " bytes" << std::endl;
    }
    try
    {
      Codecs::DataSourceBuffer source(data->get(), bytesReceived);
      Messages::Message message(decoder_->getTemplateRegistry()->maxFieldCount());
      decoder_->reset();
      decoder_->decodeMessage(source, message);

      if(!consumer_->consumeMessage(message))
      {
        if(verboseExecution_)
        {
          std::cerr << "Consumer requested early termination." << std::endl;
        }
        ioService_.stop();
        return;
      }
      startReceive(data);
    }
    catch (const std::exception &ex)
    {
      std::cerr << "Decoding error: " << ex.what() << std::endl;
    }
    if(inputCount_ > messageLimit_)
    {
      ioService_.stop();
    }
  }
  else
  {
    std::cerr << "Error code: " << error.message() << std::endl;
    ioService_.stop();
  }
}


void
MulticastDecoder::fini()
{
}

