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

#include <Communication/MulticastReceiver.h>
#include <Communication/TCPReceiver.h>
#include <Communication/RawFileReceiver.h>
#include <Communication/PCapFileReceiver.h>

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
  if(!configuration.fastFileName_.empty())
  {
    fastFile_ = new std::ifstream(configuration.fastFileName_.c_str(), binaryMode);
    if(!fastFile_->good())
    {
      std::stringstream msg;
      msg << "Can't open FAST data file: "
        << configuration.fastFileName_;
      throw std::invalid_argument(msg.str());
    }
  }

  if(!configuration.verboseFileName_.empty())
  {
    if(configuration.verboseFileName_ == "cerr")
    {
      verboseFile_= &std::cerr;
    }
    else if(configuration.verboseFileName_ == "cout")
    {
      verboseFile_ = &std::cout;
    }
    else
    {
      ownVerboseFile_ = true;
      verboseFile_ = new std::ofstream(configuration.verboseFileName_.c_str());
      if(!verboseFile_->good())
      {
        std::stringstream msg;
        msg << "Can't open verbose file: "
          << configuration.verboseFileName_;
        throw std::invalid_argument(msg.str());
      }
    }
  }

  if(! configuration.echoFileName_.empty())
  {
    ownEchoFile_ = true;
    std::ios::openmode mode = std::ios::out | std::ios::trunc;
    if(configuration.echoType_ == Codecs::DataSource::RAW)
    {
      mode |= binaryMode;
    }
    if(configuration.echoFileName_ == "cout")
    {
      echoFile_ = & std::cout;
    }
    else if(configuration.echoFileName_ == "cerr")
    {
      echoFile_ = & std::cerr;
    }
    else
    {
      echoFile_ = new std::ofstream(configuration.echoFileName_.c_str(), mode);
    }
    if(!echoFile_->good())
    {
      std::stringstream msg;
      msg << "Can't open echo file: "
        << configuration.echoFileName_;
      throw std::invalid_argument(msg.str());
    }
  }

  std::ifstream templates(configuration.templateFileName_.c_str());
  if(!templates.good())
  {
      std::stringstream msg;
      msg << "Can't open template file: "
        << configuration.templateFileName_;
      throw std::invalid_argument(msg.str());
  }
  Codecs::XMLTemplateParser parser;
  parser.setVerboseOutput(*verboseFile_);

  registry_ = parser.parse(templates);

  switch(configuration.headerType_)
  {
  case Application::DecoderConfiguration::NO_HEADER:
    {
      analyzer_.reset(new Codecs::NoHeaderAnalyzer);
      break;
    }
  case Application::DecoderConfiguration::FIXED_HEADER:
    {
      analyzer_.reset(new Codecs::FixedSizeHeaderAnalyzer(
        configuration.headerMessageSizeBytes_,
        configuration.headerBigEndian_,
        configuration.headerPrefixCount_,
        configuration.headerSuffixCount_
      ));
      break;
    }
  case Application::DecoderConfiguration::FAST_HEADER:
    {
      analyzer_.reset(new Codecs::FastEncodedHeaderAnalyzer(
        configuration.headerPrefixCount_,
        configuration.headerSuffixCount_,
        configuration.headerMessageSizeBytes_ != 0)); // true if header contains message size
      break;
    }
  }

  switch(configuration.assemblerType_)
  {
  case Application::DecoderConfiguration::MESSAGE_PER_PACKET_ASSEMBLER:
    {
      Codecs::MessagePerPacketAssembler * pAssembler = new Codecs::MessagePerPacketAssembler(
        registry_,
        *analyzer_,
        builder);
      assembler_.reset(pAssembler);
      pAssembler->setEcho(*echoFile_, configuration.echoType_, configuration.echoMessage_, configuration.echoField_);
      pAssembler->setMessageLimit(configuration.head_);
      break;
    }
  case Application::DecoderConfiguration::STREAMING_ASSEMBLER:
    {
      Codecs::StreamingAssembler * pAssembler = new Codecs::StreamingAssembler(
        registry_,
        *analyzer_,
        builder,
        configuration.waitForCompleteMessage_);
      assembler_.reset(pAssembler);
      pAssembler->setEcho(*echoFile_, configuration.echoType_, configuration.echoMessage_, configuration.echoField_);
      pAssembler->setMessageLimit(configuration.head_);
      break;
    }
  default:
    {
      switch(configuration.receiverType_)
      {
      case Application::DecoderConfiguration::MULTICAST_RECEIVER:
      case Application::DecoderConfiguration::PCAPFILE_RECEIVER:
        {
          Codecs::MessagePerPacketAssembler * pAssembler = new Codecs::MessagePerPacketAssembler(
            registry_,
            *analyzer_,
            builder);
          assembler_.reset(pAssembler);
          pAssembler->setEcho(*echoFile_, configuration.echoType_, configuration.echoMessage_, configuration.echoField_);
          pAssembler->setMessageLimit(configuration.head_);
          break;
        }
      case Application::DecoderConfiguration::TCP_RECEIVER:
      case Application::DecoderConfiguration::RAWFILE_RECEIVER:
        {
          Codecs::StreamingAssembler * pAssembler = new Codecs::StreamingAssembler(
            registry_,
            *analyzer_,
            builder,
            configuration.waitForCompleteMessage_);
          assembler_.reset(pAssembler);
          pAssembler->setEcho(*echoFile_, configuration.echoType_, configuration.echoMessage_, configuration.echoField_);
          pAssembler->setMessageLimit(configuration.head_);
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

  switch(configuration.receiverType_)
  {
  case Application::DecoderConfiguration::MULTICAST_RECEIVER:
    {
      receiver_.reset(new Communication::MulticastReceiver(
        configuration.multicastGroupIP_,
        configuration.listenInterfaceIP_,
        configuration.portNumber_));
      break;
    }
  case Application::DecoderConfiguration::TCP_RECEIVER:
    {
      receiver_.reset(new Communication::TCPReceiver(
      configuration.hostName_,
      configuration.portName_));
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
        configuration.pcapFileName_,
        configuration.pcapWordSize_));

      break;
    }
  }

  receiver_->start(*assembler_, configuration.bufferSize_, configuration.bufferCount_);

}

