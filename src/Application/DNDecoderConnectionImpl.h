// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DNDECODERCONNECTIONIMPL_H
#define DNDECODERCONNECTIONIMPL_H
#include <Application/DecoderConnection_fwd.h>
#include <Common/QuickFAST_Export.h>

#include <Application/DecoderConfiguration_fwd.h>
#include <Messages/ValueMessageBuilder_fwd.h>
namespace QuickFAST
{
  namespace DotNet
  {
    /// @brief Implementation of DNDecoderConnection
    ///
    /// Helps isolate .NET/C# code from native C++ code.
    class QuickFAST_Export DNDecoderConnectionImpl
    {
    public:
      DNDecoderConnectionImpl();
      ~DNDecoderConnectionImpl();

      /// @brief Start the decoding process
      /// @param builder is the object to receive the decode message
      /// @param extraThreadCount is a count of additional threads to be started.
      /// @param useThisThread if true will cause this thread to stay in the decoder until decoding is complete.
      void run(
        Messages::ValueMessageBuilder & builder,
        size_t extraThreadCount,
        bool useThisThread
        );

      /// @brief Decode from an in-memory buffer
      /// @param builder is the object to receive the decode message
      /// @param pBuffer points to the data
      /// @param byteOffset is a starting offset in the data
      /// @param bytesUsed is the amount of data (including offset) in the buffer
      /// @param reset resets the decoder's dictionaries before decoding.
      void run(
        Messages::ValueMessageBuilder & builder,
        const unsigned char * pBuffer,
        long byteOffset,
        long bytesUsed,
        bool reset);

      /// @brief Stop decoding
      void stop();
      /// @brief Wait for any threads started by run() to finish.
      void joinThreads();

      /// @brief Limit the number of records to decode.
      void setHead(unsigned int value);

      /// @brief Reset for every message
      void setReset(bool value);

      /// @brief strict decoding rules.
      void setStrict(bool value);

      /// @brief The name of a template file.
      void setTemplateFileName(const std::string & value);

      /// @brief The name of a raw data file.
      void setFastFileName(const std::string & value);

      /// @brief Name of a file containing captured network traffic.
      void setPcapFileName(const std::string & value);

      /// @brief Word size of the machine where the PCAP file was captured.
      void setPcapWordSize(unsigned int value);

      /// @brief Name of a file to receive verbose output.
      void setVerboseFileName(const std::string & value);

      /// @brief Name of a file to receive echoed input.
      void setEchoFileName(const std::string & value);

      /// @brief Type of data to be echoed.
      void setEchoType(Application::DecoderConfigurationEnums::EchoType type);

      /// @brief Echo message boundaries?
      void setEchoMessage(bool value);

      /// @brief Echo field boundaries?
      void setEchoField(bool value);

      /// @brief Type of header on each message
      void setHeaderType(Application::DecoderConfigurationEnums::HeaderType value);

      /// @brief Numbers in header are big endian
      void setHeaderBigEndian(bool value);

      /// @brief Bytes in message size field in the header.
      void setHeaderMessageSizeBytes(unsigned int value);

      /// @brief Bytes before beginning of message size field in header.
      void setHeaderPrefixCount(unsigned int value);

      /// @brief Bytes after end of message size field in header.
      void setHeaderSuffixCount(unsigned int value);

      /// @brief How to assemble complete messages.
      void setAssemblerType(Application::DecoderConfigurationEnums::AssemblerType value);

      /// @brief  Wait for complete message before decoding starts.
      void setWaitForCompleteMessage(bool value);

      /// @brief How to receive incoming data.
      void setReceiverType(Application::DecoderConfigurationEnums::ReceiverType value);

      /// @brief For Multicast Data: Group ip
      void setMulticastGroupIP(const std::string & value);

      /// @brief For Multicast or TCP/IP Data: Port
      void setPortNumber(unsigned short value);

      /// @brief For Multicast Data: Listen IP (selects NIC)
      void setListenInterfaceIP(const std::string & value);

      /// @brief For TCP/IP initiator: Host to connect to
      void setHostName(const std::string & value);
      /// @brief For TCP/IP initiator: name of port.
      void setPortName(const std::string & value);

      /// @brief Size of buffers to use to receive FAST data.
      void setBufferSize(unsigned int value);

      /// @brief Number of buffers to use to receive FAST data.
      void setBufferCount(unsigned int value);
    private:
      Application::DecoderConfiguration * configuration_;
      Application::DecoderConnection * connection_;

    };

  }
}
#endif // DNDECODERCONNECTIONIMPL_H
