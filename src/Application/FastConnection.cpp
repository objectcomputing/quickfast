// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include "FastConnection.h"
#include <Codecs/XMLTemplateParser.h>
#include <Codecs/DataSourceStream.h>
#include <Codecs/DataSourceBlockedStream.h>
#include <Codecs/SynchronousDecoder.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/GenericMessageBuilder.h>
#include <Codecs/MessageConsumer.h>

using namespace QuickFAST;
using namespace Application;

namespace
{
  const static std::ios::openmode readBinaryOpenMode = std::ios::in
#ifdef _WIN32
    | std::ios::binary
#endif
    ;
}

FastConnection::FastConnection()
: verbose_(false)
, resetOnMessage_(false)
, strict_(true)
, echoFile_(0)
, echoType_(Codecs::DataSource::HEX)
, echoMessage_(true)
, echoField_(false)
{
}

void
FastConnection::setVerbose(bool verbose)
{
  verbose_ = verbose;
}

void
FastConnection::setResetOnMessage(bool reset)
{
  resetOnMessage_ = reset;
}

void
FastConnection::setStrict(bool strict)
{
  strict_ = strict;
}

void
FastConnection::setEcho(
  std::ostream * echoFile,
  Codecs::DataSource::EchoType echoType,
  bool echoMessageBoundary,
  bool echoFieldBoundary)
{
  echoFile_ = echoFile;
  echoType_ = echoType;
  echoMessage_ = echoMessageBoundary;
  echoField_ = echoFieldBoundary;
}

void
FastConnection::parseTemplates(const std::string & templateFileName)
{
  std::ifstream templateFile;
  templateFile.open(templateFileName.c_str(), readBinaryOpenMode);
  if(!templateFile.good())
  {
    std::stringstream message;
    message << "ERROR: Can't open template file: " << templateFileName;
    throw QuickFAST::TemplateDefinitionError(message.str());
  }

  Codecs::XMLTemplateParser parser;
  if(verbose_)
  {
    parser.setVerboseOutput(std::cout);
  }
  templateRegistry_ = parser.parse(templateFile);
}

void
FastConnection::shareTemplates(FastConnection & sourceConnection)
{
  templateRegistry_ = sourceConnection.templateRegistry_;
}

void
FastConnection::shareTemplates(Codecs::TemplateRegistryPtr & templateRegistry)
{
  templateRegistry_ = templateRegistry;
}


void
FastConnection::decodeFile(
  std::string const & filename,
  bool isBlocked,
  Codecs::MessageConsumer & handler)
{
  std::ifstream fastFile(filename.c_str(), readBinaryOpenMode);
  if(!fastFile.good())
  {
    std::stringstream message;
    message << "ERROR: Can't open FAST Message file: "
      << filename;
  }
  decodeFile(fastFile, isBlocked, handler);
}
void
FastConnection::decodeFile(
  std::istream & dataStream,
  bool isBlocked,
  Codecs::MessageConsumer & handler)
{
  Codecs::GenericMessageBuilder builder(handler);
  decodeFile(dataStream, isBlocked, builder);
}

void
FastConnection::decodeFile(
  std::string const & filename,
  bool isBlocked,
  Messages::ValueMessageBuilder & builder)
{
  std::ifstream fastFile(filename.c_str(), readBinaryOpenMode);
  if(!fastFile.good())
  {
    std::stringstream message;
    message << "ERROR: Can't open FAST Message file: "
      << filename;
  }
  decodeFile(fastFile, isBlocked, builder);
}

void
FastConnection::decodeFile(
  std::istream & dataStream,
  bool isBlocked,
  Messages::ValueMessageBuilder & builder)
{
//  Codecs::DataSourceStream source(dataStream);
  Codecs::DataSourcePtr source;
  if(isBlocked)
  {
    source.reset(new Codecs::DataSourceBlockedStream(dataStream));
  }
  else
  {
    source.reset(new Codecs::DataSourceStream(dataStream));
  }
  if(echoFile_ != 0)
  {
    source->setEcho(*echoFile_,
      echoType_,
      echoMessage_,
      echoField_);
  }
  decodeFromDataSource(source, builder);
}

void
FastConnection::decodeFromDataSource(Codecs::DataSourcePtr & source, Messages::ValueMessageBuilder &builder)
{
  Codecs::SynchronousDecoder decoder(templateRegistry_);
  decoder.setStrict(strict_);
  decoder.setResetOnMessage(resetOnMessage_);
  if(verbose_)
  {
    decoder.setVerboseOutput(std::cout);
  }

  decoder.decode(*source, builder);

  if(verbose_)
  {
    std::cout << "Decoded " << decoder.messageCount() << " messages." << std::endl;
  }
}

void
FastConnection::listenForMulticast(
  std::string const & multicastAddressName,
  std::string const & listenAddressName,
  unsigned short portNumber)
{
  multicastDecoder_.reset(new Codecs::MulticastDecoder(
    templateRegistry_,
    multicastAddressName,
    listenAddressName,
    portNumber));
}

void
FastConnection::run(
  Codecs::MessageConsumer & handler,
  size_t threadCount,
  bool useThisThread,
  size_t bufferSize = 1400,
  size_t bufferCount = 2)
{
  builder_.reset(new Codecs::GenericMessageBuilder(handler));
  multicastDecoder_->start(*builder_, bufferSize, bufferCount);
  multicastDecoder_->run(threadCount, useThisThread);
  if(useThisThread)
  {
    multicastDecoder_.reset();
    builder_.reset();
  }
}

void
FastConnection::stop()
{
  if(multicastDecoder_)
  {
    multicastDecoder_->stop();
  }
}

void
FastConnection::joinThreads()
{
  if(multicastDecoder_)
  {
    multicastDecoder_->joinThreads();
    multicastDecoder_.reset();
    builder_.reset();
  }
}

