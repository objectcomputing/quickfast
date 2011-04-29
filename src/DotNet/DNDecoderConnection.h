// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#pragma unmanaged
#include <Application/DecoderConnection_fwd.h>
#include <Application/DecoderConfiguration_fwd.h>
#pragma managed
#include <DotNet/DNMessageBuilder.h>
#include <DotNet/StringConversion.h>

namespace QuickFAST
{
  namespace DotNet
  {
    class DNDecoderConnectionImpl;

    /// @brief a class to configure and run a FAST decoder
    public ref class DNDecoderConnection
    {
    public:

      DNDecoderConnection();
      !DNDecoderConnection();
      ~DNDecoderConnection();

      /// @brief Run the decoder's event loop.
      /// @param dnBuilder is the Message Builder that receives the decoded information
      /// @param extraThreadCount determines how many additional threads will be started to support decoding'
      /// @param useThisThread True: control will not return to the caller until the decoding stops.
      ///        False: return immediately (only makes sense if extraThreadCount > 0)
      void run(DNMessageBuilder ^ dnBuilder, size_t extraThreadCount, bool useThisThread);


      /// @brief Run the decoder to read bytes from a buffer
      ///
      /// May be called repeatedly.
      /// Should be called instead of the run() method with thread counts.
      ///  i.e. do not call both run() methods.
      /// @param dnBuilder is the Message Builder that receives the decoded information
      /// @param  memoryBuffer contains the data to be decoded
      /// @param byteOffset is the starting position in the buffer
      /// @param bytesUsed is the number of bytes to be decoded
      /// @param reset resets the decoder before decoding begins.
      void run(
        DNMessageBuilder ^ dnBuilder,
        array<unsigned char>^ memoryBuffer,
        long byteOffset,
        long bytesUsed,
        bool reset
        );

      /// @brief Request stop after calling run with thread counts
      ///
      /// Returns immediately.  Call join to wait for the stop to complete.
      void stop();

      /// @brief Wait for the decoder to come to a stop.
      ///
      /// Unless stop() is called first (or later in another thread), this will never return.
      /// Should only be called after calling run with useThisThread = false;
      void joinThreads();

      /////////////
      // Properties

      /// @brief Stop after the first "head" messages
      property
      unsigned int Head
      {
        void set(unsigned int value);
      }

      /// @brief Reset the decoder at the start of every message and/or packet
      property
      bool Reset
      {
        void set(bool reset);
      }

      /// @brief Use strict decoding rules
      property
      bool Strict
      {
        void set(bool strict);
      }

      /// @brief The name of the template file
      property
      System::String ^ TemplateFileName
      {
        void set(System::String ^ templateFileName);
      }

      /// @brief The name of a data file containing Raw FAST records
      property
      System::String ^ FastFileName
      {
        void set(System::String ^ fastFileName);
      }

      /// @brief The name of a file to which verbose output will be written.
      property
      System::String ^ VerboseFileName
      {
        void set(System::String ^ verboseFileName);
      }

      /// @brief The name of a file containing PCap captured, FAST encoded records
      property
      System::String ^ PcapFileName
      {
        void set(System::String ^ pcapFileName);
      }

      /// @brief The name of a file to which echo output will be written
      property
      System::String ^ EchoFileName
      {
        void set(System::String ^ echoFileName);
      }

      /// @brief The type of data to be echoed (hex/raw)
      enum class EchoTypes{
        ECHO_HEX = Application::DecoderConfigurationEnums::HEX,
        ECHO_RAW = Application::DecoderConfigurationEnums::RAW,
        ECHO_NONE = Application::DecoderConfigurationEnums::NONE
      };

      property
      EchoTypes EchoType
      {
        void set(EchoTypes echoType);
      }

      /// @brief Echo Message Boundaries?
      property
      bool EchoMessage
      {
        void set(bool echoMessage);
      }

      /// @brief Echo Field Boundaries?
      property
      bool EchoField
      {
        void set(bool echoField);
      }

      /// @brief What word size is used in the PCAP file.
      property
      unsigned int PcapWordSize
      {
        void set(unsigned int pcapWordSize);
      }

      /// @brief what type of header (if any) appears in incoming FAST data
      enum class HeaderTypes{
        NO_HEADER = Application::DecoderConfigurationEnums::NO_HEADER,
        FIXED_HEADER = Application::DecoderConfigurationEnums::FIXED_HEADER,
        FAST_HEADER = Application::DecoderConfigurationEnums::FAST_HEADER};

      property
      HeaderTypes HeaderType
      {
        void set(HeaderTypes headerType);
      }

      /// @brief for FIXED_HEADER, how long is the message size field
      property
      unsigned int HeaderMessageSizeBytes
      {
        void set(unsigned int headerMessageSizeBytes);
      }

      /// @brief for FIXED_HEADER, is the message size field big or little -endian
      property
      bool HeaderBigEndian
      {
        void set(bool headerBigEndian);
      }

      /// @brief for FIXED_HEADER/FAST_HEADER how many bytes/fields come before the message size field
      property
      unsigned int HeaderPrefixCount
      {
        void set(unsigned int headerPrefixCount);
      }

      /// @brief for FIXED_HEADER/FAST_HEADER how many bytes/fields come after the message size field
      property
      unsigned int HeaderSuffixCount
      {
        void set(unsigned int headerSuffixCount);
      }

      /// @brief How should incoming packets be assembled into messages?
      enum class DNAssemblerType{
        MESSAGE_PER_PACKET_ASSEMBLER = Application::DecoderConfigurationEnums::MESSAGE_PER_PACKET_ASSEMBLER,
        STREAMING_ASSEMBLER = Application::DecoderConfigurationEnums::STREAMING_ASSEMBLER,
        UNSPECIFIED_ASSEMBLER = Application::DecoderConfigurationEnums::UNSPECIFIED_ASSEMBLER
      };

      property
      DNAssemblerType AssemblerType
      {
        void set(DNAssemblerType assemblerType);
      }

      /// @brief for STREAMING_ASSEMBLER, wait for complete message before decoding begins?
      property
      bool WaitForCompleteMessage
      {
        void set(bool waitForCompleteMessage);
      }

      /// @brief Where does the FAST data come from?
      enum class ReceiverTypes
      {
        MULTICAST_RECEIVER = Application::DecoderConfigurationEnums::MULTICAST_RECEIVER,
        TCP_RECEIVER = Application::DecoderConfigurationEnums::TCP_RECEIVER,
        RAWFILE_RECEIVER = Application::DecoderConfigurationEnums::RAWFILE_RECEIVER,
        PCAPFILE_RECEIVER = Application::DecoderConfigurationEnums::PCAPFILE_RECEIVER,
        BUFFER_RECEIVER = Application::DecoderConfigurationEnums::BUFFER_RECEIVER,
        UNSPECIFIED_RECEIVER = Application::DecoderConfigurationEnums::UNSPECIFIED_RECEIVER
      };

      property
      ReceiverTypes ReceiverType
      {
        void set(ReceiverTypes receiverType);
      }

      /// @brief for MULTICAST_RECEIVER identify the group IP to subscribe to
      property
      System::String ^ MulticastGroupIP
      {
        void set(System::String ^ multicastGroupIP);
      }

      /// @brief for MULTICAST_RECEIVER identify the group port to subscribe to
      property
      unsigned short PortNumber
      {
        void set(unsigned short portNumber);
      }

      /// @brief for MULTICAST_RECEIVER Select a NIC on the local machine to use for multicaast
      property
      System::String ^ ListenInterfaceIP
      {
        void set(System::String ^ listenInterfaceIP);
      }

      /// @brief for TCP_RECEIVER identify the host to connect to
      property
      System::String ^ HostName
      {
        void set(System::String ^ hostName);
      }

      /// @brief for TCP_RECEIVER identify the port (name or number) to connect to
      property
      System::String ^ PortName
      {
        void set(System::String ^ portName);
      }

      /// @brief What size buffers should be allocated for the receiver
      ///
      /// For MESSAGE_PER_PACKET_ASSEMBLER must equal or exceed the size of the largest expected message
      property
      unsigned int BufferSize
      {
        void set(unsigned int bufferSize);
      }

      /// @brief How many buffers should be allocated for the receiver
      ///
      /// For STREAMING_ASSEMBLER with  WaitForCompleteMessage set:
      ///  BufferSize * BufferCount must equal or exceed the size of the largest expected message
      property
      unsigned int BufferCount
      {
        void set(unsigned int bufferCount);
      }

    private:
      DNDecoderConnectionImpl * impl_;
    };
  }
}
