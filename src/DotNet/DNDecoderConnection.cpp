// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#include "QuickFASTDotNetPch.h"
#include "DNDecoderConnection.h"
#pragma unmanaged
#include <Application/DNDecoderConnectionImpl.h>
#pragma managed

using namespace QuickFAST;
using namespace DotNet;


DNDecoderConnection::DNDecoderConnection()
{
  impl_ = new DNDecoderConnectionImpl();
}

DNDecoderConnection::!DNDecoderConnection()
{
  this->~DNDecoderConnection();
}

DNDecoderConnection::~DNDecoderConnection()
{
  delete impl_;
}

void
DNDecoderConnection::run(DNMessageBuilder ^ dnBuilder, size_t extraThreadCount, bool useThisThread)
{
  try
  {
    Messages::ValueMessageBuilder & builder = dnBuilder->builder();
    impl_->run(builder, extraThreadCount, useThisThread);
  }
  catch (std::exception const & ex)
  {
    throw gcnew System::Exception(gcnew System::String(ex.what()));
  }
}

void
DNDecoderConnection::run(
  DNMessageBuilder ^ dnBuilder,
  array<unsigned char>^ memoryBuffer,
  long byteOffset,
  long bytesUsed,
  bool reset
  )
{
  try
  {
    Messages::ValueMessageBuilder & builder = dnBuilder->builder();
    pin_ptr<unsigned char> pinnedBuffer = &memoryBuffer[0];
    const unsigned char * pBuffer = pinnedBuffer;
    impl_->run(builder, pBuffer, byteOffset, bytesUsed, reset);
  }
  catch (std::exception const & ex)
  {
    throw gcnew System::Exception(gcnew System::String(ex.what()));
  }
}


void DNDecoderConnection::Head::set(unsigned int value)
{
  impl_->setHead(value);
}

void DNDecoderConnection::Reset::set(bool reset)
{
  impl_->setReset(reset);
}

void DNDecoderConnection::Strict::set(bool strict)
{
  impl_->setStrict(strict);
}

void DNDecoderConnection::TemplateFileName::set(System::String ^ templateFileName)
{
  impl_->setTemplateFileName(string_cast<std::string>(templateFileName));
}

void DNDecoderConnection::FastFileName::set(System::String ^ fastFileName)
{
  impl_->setFastFileName(string_cast<std::string>(fastFileName));
}

void DNDecoderConnection::VerboseFileName::set(System::String ^ verboseFileName)
{
  impl_->setVerboseFileName(string_cast<std::string>(verboseFileName));
}

void DNDecoderConnection::PcapFileName::set(System::String ^ pcapFileName)
{
  impl_->setPcapFileName(string_cast<std::string>(pcapFileName));
}

void DNDecoderConnection::EchoFileName::set(System::String ^ echoFileName)
{
  impl_->setEchoFileName(string_cast<std::string>(echoFileName));
}

void DNDecoderConnection::EchoType::set(EchoTypes echoType)
{
  impl_->setEchoType(static_cast<Application::DecoderConfigurationEnums::EchoType>(echoType));
}

void DNDecoderConnection::EchoMessage::set(bool echoMessage)
{
  impl_->setEchoMessage(echoMessage);
}

void DNDecoderConnection::EchoField::set(bool echoField)
{
  impl_->setEchoField(echoField);
}

void DNDecoderConnection::PcapWordSize::set(unsigned int pcapWordSize)
{
  impl_->setPcapWordSize(pcapWordSize);
}

void DNDecoderConnection::HeaderType::set(HeaderTypes headerType)
{
  impl_->setHeaderType(static_cast<Application::DecoderConfigurationEnums::HeaderType>(headerType));
}

void DNDecoderConnection::HeaderMessageSizeBytes::set(unsigned int headerMessageSizeBytes)
{
  impl_->setHeaderMessageSizeBytes(headerMessageSizeBytes);
}

void DNDecoderConnection::HeaderBigEndian::set(bool headerBigEndian)
{
  impl_->setHeaderBigEndian(headerBigEndian);
}

void DNDecoderConnection::HeaderPrefixCount::set(unsigned int headerPrefixCount)
{
  impl_->setHeaderPrefixCount(headerPrefixCount);
}

void DNDecoderConnection::HeaderSuffixCount::set(unsigned int headerSuffixCount)
{
  impl_->setHeaderSuffixCount(headerSuffixCount);
}

void DNDecoderConnection::AssemblerType::set(DNAssemblerType assemblerType)
{
  impl_->setAssemblerType(static_cast<Application::DecoderConfigurationEnums::AssemblerType>(assemblerType));
}

void DNDecoderConnection::WaitForCompleteMessage::set(bool waitForCompleteMessage)
{
  impl_->setWaitForCompleteMessage(waitForCompleteMessage);
}

void DNDecoderConnection::ReceiverType::set(ReceiverTypes receiverType)
{
  impl_->setReceiverType(static_cast<Application::DecoderConfigurationEnums::ReceiverType>(receiverType));
}

void DNDecoderConnection::MulticastGroupIP::set(System::String ^ multicastGroupIP)
{
  impl_->setMulticastGroupIP(string_cast<std::string>(multicastGroupIP));
}

void DNDecoderConnection::PortNumber::set(unsigned short portNumber)
{
  impl_->setPortNumber(portNumber);
}

void DNDecoderConnection::ListenInterfaceIP::set(System::String ^ listenInterfaceIP)
{
  impl_->setListenInterfaceIP(string_cast<std::string>(listenInterfaceIP));
}

void DNDecoderConnection::HostName::set(System::String ^ hostName)
{
  impl_->setHostName(string_cast<std::string>(hostName));
}

void DNDecoderConnection::PortName::set(System::String ^ portName)
{
  impl_->setPortName(string_cast<std::string>(portName));
}

void DNDecoderConnection::BufferSize::set(unsigned int bufferSize)
{
  impl_->setBufferSize(bufferSize);
}

void DNDecoderConnection::BufferCount::set(unsigned int bufferCount)
{
  impl_->setBufferCount(bufferCount);
}

void DNDecoderConnection::stop()
{
  impl_->stop();
}

void DNDecoderConnection::joinThreads()
{
  impl_->joinThreads();
}
