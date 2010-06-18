// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include "DecoderConnection.h"
#include <Codecs/XMLTemplateParser.h>
#include <Codecs/MessagePerPacketAssembler.h>
#include <Codecs/StreamingAssembler.h>
#include <Codecs/NoHeaderAnalyzer.h>
#include <Codecs/FixedSizeHeaderAnalyzer.h>
#include <Codecs/FastEncodedHeaderAnalyzer.h>
#include <Codecs/TemplateRegistry.h>

#include <Communication/MulticastReceiver.h>
#include <Communication/TCPReceiver.h>
#include <Communication/RawFileReceiver.h>
#include <Communication/PCapFileReceiver.h>
#include <Communication/BufferReceiver.h>

using namespace QuickFAST;
using namespace Application;

namespace
{
#ifdef _WIN32
  const std::ios::openmode binaryMode = std::ios::binary;
#else
  const std::ios::openmode binaryMode = static_cast<std::ios::openmode>(0);
#endif
}


DecoderConnection::DecoderConnection()
: fastFile_(0)
, echoFile_(0)
, verboseFile_(0)
, ownEchoFile_(false)
, ownVerboseFile_(false)
{
}

DecoderConnection::~DecoderConnection()
{
  delete fastFile_;
  if(ownEchoFile_)
  {
    delete echoFile_;
  }
  if(ownVerboseFile_)
  {
    delete verboseFile_;
  }
}

void
DecoderConnection::configure(
  Messages::ValueMessageBuilder & builder,
  Application::DecoderConfiguration &configuration)
{
  if(!configuration.fastFileName().empty())
  {
    fastFile_ = new std::ifstream(configuration.fastFileName().c_str(), binaryMode);
    if(!fastFile_->good())
    {
      std::stringstream msg;
      msg << "Can't open FAST data file: "
        << configuration.fastFileName();
      throw std::invalid_argument(msg.str());
    }
  }

  if(!configuration.verboseFileName().empty())
  {
    if(configuration.verboseFileName() == "cerr")
    {
      verboseFile_= &std::cerr;
    }
    else if(configuration.verboseFileName() == "cout")
    {
      verboseFile_ = &std::cout;
    }
    else
    {
      ownVerboseFile_ = true;
      verboseFile_ = new std::ofstream(configuration.verboseFileName().c_str());
      if(!verboseFile_->good())
      {
        std::stringstream msg;
        msg << "Can't open verbose file: "
          << configuration.verboseFileName();
        throw std::invalid_argument(msg.str());
      }
    }
  }

  if(! configuration.echoFileName().empty())
  {
    ownEchoFile_ = true;
    std::ios::openmode mode = std::ios::out | std::ios::trunc;
    if(configuration.echoType() == Codecs::DataSource::RAW)
    {
      mode |= binaryMode;
    }
    if(configuration.echoFileName() == "cout")
    {
      echoFile_ = & std::cout;
      ownEchoFile_ = false;
    }
    else if(configuration.echoFileName() == "cerr")
    {
      echoFile_ = & std::cerr;
      ownEchoFile_ = false;
    }
    else
    {
      echoFile_ = new std::ofstream(configuration.echoFileName().c_str(), mode);
    }
    if(!echoFile_->good())
    {
      std::stringstream msg;
      msg << "Can't open echo file: "
        << configuration.echoFileName();
      throw std::invalid_argument(msg.str());
    }
  }

  std::ifstream templates(configuration.templateFileName().c_str(),
    std::ios::in
#ifdef _WIN32
    | std::ios::binary
#endif
    );
  if(!templates.good())
  {
      std::stringstream msg;
      msg << "Can't open template file: "
        << configuration.templateFileName();
      throw std::invalid_argument(msg.str());
  }
  Codecs::XMLTemplateParser parser;
  parser.setVerboseOutput(*verboseFile_);
  parser.setNonstandard(configuration.nonstandard());

  registry_ = parser.parse(templates);

  if((configuration.nonstandard() & 4) != 0)
  {
    registry_->display(std::cout, 0);
  }

  switch(configuration.headerType())
  {
  case Application::DecoderConfiguration::NO_HEADER:
    {
      Codecs::NoHeaderAnalyzer * analyzer = new Codecs::NoHeaderAnalyzer;
      analyzer->setTestSkip(configuration.testSkip());
      analyzer_.reset(analyzer);
      break;
    }
  case Application::DecoderConfiguration::FIXED_HEADER:
    {
      Codecs::FixedSizeHeaderAnalyzer * fixedSizeHeaderAnalyzer = new Codecs::FixedSizeHeaderAnalyzer(
        configuration.headerMessageSizeBytes(),
        configuration.headerBigEndian(),
        configuration.headerPrefixCount(),
        configuration.headerSuffixCount());
      fixedSizeHeaderAnalyzer->setTestSkip(configuration.testSkip());
      analyzer_.reset(fixedSizeHeaderAnalyzer);
      break;
    }
  case Application::DecoderConfiguration::FAST_HEADER:
    {
      analyzer_.reset(new Codecs::FastEncodedHeaderAnalyzer(
        configuration.headerPrefixCount(),
        configuration.headerSuffixCount(),
        configuration.headerMessageSizeBytes() != 0)); // true if header contains message size
      break;
    }
  }

  switch(configuration.assemblerType())
  {
  case Application::DecoderConfiguration::MESSAGE_PER_PACKET_ASSEMBLER:
    {
      Codecs::MessagePerPacketAssembler * pAssembler = new Codecs::MessagePerPacketAssembler(
        registry_,
        *analyzer_,
        builder);
      assembler_.reset(pAssembler);
      pAssembler->setEcho(*echoFile_, configuration.echoType(), configuration.echoMessage(), configuration.echoField());
      pAssembler->setMessageLimit(configuration.head());
      break;
    }
  case Application::DecoderConfiguration::STREAMING_ASSEMBLER:
    {
      Codecs::StreamingAssembler * pAssembler = new Codecs::StreamingAssembler(
        registry_,
        *analyzer_,
        builder,
        configuration.waitForCompleteMessage());
      assembler_.reset(pAssembler);
      pAssembler->setEcho(*echoFile_, configuration.echoType(), configuration.echoMessage(), configuration.echoField());
      pAssembler->setMessageLimit(configuration.head());
      break;
    }
  default:
    {
      switch(configuration.receiverType())
      {
      case Application::DecoderConfiguration::MULTICAST_RECEIVER:
      case Application::DecoderConfiguration::PCAPFILE_RECEIVER:
      case Application::DecoderConfiguration::BUFFER_RECEIVER:
        {
          Codecs::MessagePerPacketAssembler * pAssembler = new Codecs::MessagePerPacketAssembler(
            registry_,
            *analyzer_,
            builder);
          assembler_.reset(pAssembler);
          pAssembler->setEcho(*echoFile_, configuration.echoType(), configuration.echoMessage(), configuration.echoField());
          pAssembler->setMessageLimit(configuration.head());
          break;
        }
      case Application::DecoderConfiguration::TCP_RECEIVER:
      case Application::DecoderConfiguration::RAWFILE_RECEIVER:
        {
          Codecs::StreamingAssembler * pAssembler = new Codecs::StreamingAssembler(
            registry_,
            *analyzer_,
            builder,
            configuration.waitForCompleteMessage());
          assembler_.reset(pAssembler);
          pAssembler->setEcho(*echoFile_, configuration.echoType(), configuration.echoMessage(), configuration.echoField());
          pAssembler->setMessageLimit(configuration.head());
          break;
        }
      default:
        {
          std::stringstream msg;
          msg << "DecoderConnection: Cannot determine what type of assembler to use.";
          throw std::invalid_argument(msg.str());
        }
      }
    }
  }

  assembler_->setReset(configuration.reset());
  assembler_->setStrict(configuration.strict());

  switch(configuration.receiverType())
  {
  case Application::DecoderConfiguration::MULTICAST_RECEIVER:
    {
      receiver_.reset(new Communication::MulticastReceiver(
        configuration.multicastGroupIP(),
        configuration.listenInterfaceIP(),
        configuration.portNumber()));
      break;
    }
  case Application::DecoderConfiguration::TCP_RECEIVER:
    {
      receiver_.reset(new Communication::TCPReceiver(
      configuration.hostName(),
      configuration.portName()));
      break;
    }
  case Application::DecoderConfiguration::RAWFILE_RECEIVER:
    {
      receiver_.reset(new Communication::RawFileReceiver(
        *fastFile_));
      break;
    }
  case Application::DecoderConfiguration::PCAPFILE_RECEIVER:
    {
      receiver_.reset(new Communication::PCapFileReceiver(
        configuration.pcapFileName(),
        configuration.pcapWordSize()));
      break;
    }
  case Application::DecoderConfiguration::BUFFER_RECEIVER:
    {
      receiver_.reset(new Communication::BufferReceiver);
      break;
    }
  default:
    {
      std::stringstream msg;
      msg << "DecoderConnection: Cannot determine what type of receiver to use.";
      throw std::invalid_argument(msg.str());
    }
  }

  receiver_->start(*assembler_, configuration.bufferSize(), configuration.bufferCount());

}

Codecs::Decoder &
DecoderConnection::decoder() const
{
  if(!assembler_)
  {
    throw UsageError("Coding Error","Using DecoderConnection decoder before it is configured.");
  }
  return assembler_->decoder();
}

