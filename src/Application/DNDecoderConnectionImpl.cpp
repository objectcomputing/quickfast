// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#include <Common/QuickFASTPch.h>
#include "DNDecoderConnectionImpl.h"
#include <Application/DecoderConnection.h>
#include <Application/DecoderConfiguration.h>
#include <Communication/Receiver.h>
#include <Codecs/DataSource_fwd.h>
#include <Application/DecoderConnection.h>

using namespace QuickFAST;
using namespace DotNet;

DNDecoderConnectionImpl::DNDecoderConnectionImpl()
  : configuration_(new Application::DecoderConfiguration)
  , connection_(new Application::DecoderConnection)
{
}

DNDecoderConnectionImpl::~DNDecoderConnectionImpl()
{
  delete connection_;
  connection_ = 0;
  delete configuration_;
  configuration_ = 0;
}

void
DNDecoderConnectionImpl::run(
  Messages::ValueMessageBuilder & builder,
  size_t extraThreadCount,
  bool useThisThread
  )
{
    connection_->configure(builder, *configuration_);
    connection_->receiver().runThreads(extraThreadCount, useThisThread);
}

void
DNDecoderConnectionImpl::run(
  Messages::ValueMessageBuilder & builder,
  const unsigned char * pBuffer,
  long byteOffset,
  long bytesUsed,
  bool reset)
{
  connection_->configure(builder, *configuration_);
  if(reset)
  {
    connection_->decoder().reset();
  }
  connection_->receiver().receiveBuffer(pBuffer + byteOffset, bytesUsed);
}

void
DNDecoderConnectionImpl::setHead(unsigned int value)
{
  configuration_->setHead(value);
}

void
DNDecoderConnectionImpl::setReset(bool value)
{
  configuration_->setReset(value);
}


void
DNDecoderConnectionImpl::setStrict(bool value)
{
  configuration_->setReset(value);
}

void
DNDecoderConnectionImpl::setTemplateFileName(const std::string & value)
{
  configuration_->setTemplateFileName(value);
}

void
DNDecoderConnectionImpl::setFastFileName(const std::string & value)
{
  configuration_->setFastFileName(value);
}

void
DNDecoderConnectionImpl::setVerboseFileName(const std::string & value)
{
  configuration_->setVerboseFileName(value);
}

void
DNDecoderConnectionImpl::setPcapFileName(const std::string & value)
{
  configuration_->setPcapFileName(value);
}

void
DNDecoderConnectionImpl::setEchoFileName(const std::string & value)
{
  configuration_->setEchoFileName(value);
}

void
DNDecoderConnectionImpl::setEchoType(Application::DecoderConfigurationEnums::EchoType type)
{
  configuration_->setEchoType(type);
}

void
DNDecoderConnectionImpl::setEchoMessage(bool value)
{
  configuration_->setEchoMessage(value);
}

void
DNDecoderConnectionImpl::setEchoField(bool value)
{
  configuration_->setEchoField(value);
}

void
DNDecoderConnectionImpl::setPcapWordSize(unsigned int value)
{
  configuration_->setPcapWordSize(value);
}

void
DNDecoderConnectionImpl::setHeaderType(Application::DecoderConfigurationEnums::HeaderType value)
{
  configuration_->setHeaderType(static_cast<Application::DecoderConfiguration::HeaderType>(value));
}

void
DNDecoderConnectionImpl::setHeaderMessageSizeBytes(unsigned int value)
{
  configuration_->setHeaderMessageSizeBytes(value);
}

void
DNDecoderConnectionImpl::setHeaderBigEndian(bool value)
{
  configuration_->setHeaderBigEndian(value);
}

void
DNDecoderConnectionImpl::setHeaderPrefixCount(unsigned int value)
{
  configuration_->setHeaderPrefixCount(value);
}

void
DNDecoderConnectionImpl::setHeaderSuffixCount(unsigned int value)
{
  configuration_->setHeaderSuffixCount(value);
}

void
DNDecoderConnectionImpl::setAssemblerType(Application::DecoderConfigurationEnums::AssemblerType value)
{
  configuration_->setAssemblerType(static_cast<Application::DecoderConfiguration::AssemblerType>(value));
}

void
DNDecoderConnectionImpl::setWaitForCompleteMessage(bool value)
{
  configuration_->setWaitForCompleteMessage(value);
}

void
DNDecoderConnectionImpl::setReceiverType(Application::DecoderConfigurationEnums::ReceiverType value)
{
  configuration_->setReceiverType(static_cast<Application::DecoderConfiguration::ReceiverType>(value));
}

void
DNDecoderConnectionImpl::setMulticastGroupIP(const std::string & value)
{
  configuration_->setMulticastGroupIP(value);
}

void
DNDecoderConnectionImpl::setPortNumber(unsigned short value)
{
  configuration_->setPortNumber(value);
}

void
DNDecoderConnectionImpl::setListenInterfaceIP(const std::string & value)
{
  configuration_->setListenInterfaceIP(value);
}

void
DNDecoderConnectionImpl::setHostName(const std::string & value)
{
  configuration_->setHostName(value);
}

void
DNDecoderConnectionImpl::setPortName(const std::string & value)
{
  configuration_->setPortName(value);
}

void
DNDecoderConnectionImpl::setBufferSize(unsigned int value)
{
  configuration_->setBufferSize(value);
}

void
DNDecoderConnectionImpl::setBufferCount(unsigned int value)
{
  configuration_->setBufferCount(value);
}
void
DNDecoderConnectionImpl::stop()
{
  connection_->receiver().stop();
}

void
DNDecoderConnectionImpl::joinThreads()
{
  connection_->receiver().joinThreads();
}
