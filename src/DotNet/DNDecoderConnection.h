// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#pragma unmanaged
#include <Application/DecoderConnection_fwd.h>
#include <Application/DecoderConfiguration.h>
#include <Codecs/DataSource.h>    // For Echo definitions
#pragma managed
#include <DotNet/DNMessageBuilder.h>
#include <DotNet/StringConversion.h>

namespace QuickFAST
{
  namespace DotNet
  {
    /// @Brief a class to configure and run a FAST decoder
    public ref class DNDecoderConnection
    {
    public:

      DNDecoderConnection();
      !DNDecoderConnection();
      ~DNDecoderConnection();

      void run(DNMessageBuilder ^ dnBuilder, size_t extraThreadCount, bool useThisThread);

      /////////////
      // Properties

      /// @brief Stop after the first "head" messages
      property
      unsigned int Head
      {
        void set(unsigned int head)
        {
          configuration_->head_ = head;
        }
      }

      /// @brief Reset the decoder at the start of every message and/or packet
      property
      bool Reset
      {
        void set(bool reset)
        {
          configuration_->reset_ = reset;
        }
      }

      /// @brief Use strict decoding rules
      property
      bool Strict
      {
        void set(bool strict)
        {
          configuration_->strict_ = strict;
        }
      }

      /// @brief The name of the template file
      property
      System::String ^ TemplateFileName
      {
        void set(System::String ^ templateFileName)
        {
          assignString(configuration_->templateFileName_, templateFileName);
        }
      }

      /// @brief The name of a data file containing Raw FAST records
      property
      System::String ^ FastFileName
      {
        void set(System::String ^ fastFileName)
        {
          assignString(configuration_->fastFileName_, fastFileName);
        }
      }

      /// @brief The name of a file to which verbose output will be written.
      property
      System::String ^ VerboseFileName
      {
        void set(System::String ^ verboseFileName)
        {
          assignString(configuration_->verboseFileName_, verboseFileName);
        }
      }

      /// @brief The name of a file containing PCap captured, FAST encoded records
      property
      System::String ^ PcapFileName
      {
        void set(System::String ^ pcapFileName)
        {
          assignString(configuration_->pcapFileName_, pcapFileName);
        }
      }

      /// @brief The name of a file to which echo output will be written
      property
      System::String ^ EchoFileName
      {
        void set(System::String ^ echoFileName)
        {
          assignString(configuration_->echoFileName_, echoFileName);
        }
      }

      /// @brief The type of data to be echoed (hex/raw)
      enum class EchoTypes{
          ECHO_HEX = Codecs::DataSource::HEX,
          ECHO_RAW = Codecs::DataSource::RAW,
          ECHO_NONE = Codecs::DataSource::NONE
      };

      property
      EchoTypes EchoType
      {
        void set(EchoTypes echoType)
        {
          configuration_->echoType_ = static_cast<Codecs::DataSource::EchoType>(echoType);
        }
      }

      /// @brief Echo Message Boundaries?
      property
      bool EchoMessage
      {
        void set(bool echoMessage)
        {
          configuration_->echoMessage_ = echoMessage;
        }
      }

      /// @brief Echo Field Boundaries?
      property
      bool EchoField
      {
        void set(bool echoField)
        {
          configuration_->echoField_ = echoField;
        }
      }

      /// @brief What word size is used in the PCAP file.
      property
      unsigned int PcapWordSize
      {
        void set(unsigned int pcapWordSize)
        {
          configuration_->pcapWordSize_ = pcapWordSize;
        }
      }

      enum class HeaderTypes{
        NO_HEADER = Application::DecoderConfiguration::NO_HEADER,
        FIXED_HEADER = Application::DecoderConfiguration::FIXED_HEADER,
        FAST_HEADER = Application::DecoderConfiguration::FAST_HEADER};

      property
      HeaderTypes HeaderType
      {
        void set(HeaderTypes headerType)
        {
          configuration_->headerType_ = static_cast<Application::DecoderConfiguration::HeaderType>(headerType);
        }
      }

      property
      unsigned int HeaderMessageSizeBytes
      {
        void set(unsigned int headerMessageSizeBytes)
        {
          configuration_->headerMessageSizeBytes_ = headerMessageSizeBytes;
        }
      }

      property
      bool HeaderBigEndian
      {
        void set(bool headerBigEndian)
        {
          configuration_->headerBigEndian_ = headerBigEndian;
        }
      }
      property
      unsigned int HeaderPrefixCount
      {
        void set(unsigned int headerPrefixCount)
        {
          configuration_->headerPrefixCount_ = headerPrefixCount;
        }
      }
      property
      unsigned int HeaderSuffixCount
      {
        void set(unsigned int headerSuffixCount)
        {
          configuration_->headerSuffixCount_ = headerSuffixCount;
        }
      }

      enum class AssemblerTypes{
        MESSAGE_PER_PACKET_ASSEMBLER = Application::DecoderConfiguration::MESSAGE_PER_PACKET_ASSEMBLER,
        STREAMING_ASSEMBLER = Application::DecoderConfiguration::STREAMING_ASSEMBLER,
        UNSPECIFIED_ASSEMBLER = Application::DecoderConfiguration::UNSPECIFIED_ASSEMBLER
      };

      property
      AssemblerTypes AssemblerType
      {
        void set(AssemblerTypes assemblerType)
        {
          configuration_->assemblerType_ =
            static_cast<Application::DecoderConfiguration::AssemblerType>(assemblerType);
        }
      }

      property
      bool WaitForCompleteMessage
      {
        void set(bool waitForCompleteMessage)
        {
          configuration_->waitForCompleteMessage_ = waitForCompleteMessage;
        }
      }

      enum class ReceiverTypes
      {
        MULTICAST_RECEIVER = Application::DecoderConfiguration::MULTICAST_RECEIVER,
        TCP_RECEIVER = Application::DecoderConfiguration::TCP_RECEIVER,
        RAWFILE_RECEIVER = Application::DecoderConfiguration::RAWFILE_RECEIVER,
        PCAPFILE_RECEIVER = Application::DecoderConfiguration::PCAPFILE_RECEIVER,
        UNSPECIFIED_RECEIVER = Application::DecoderConfiguration::UNSPECIFIED_RECEIVER
      };

      property
      ReceiverTypes ReceiverType
      {
        void set(ReceiverTypes receiverType)
        {
          configuration_->receiverType_ =
            static_cast<Application::DecoderConfiguration::ReceiverType>(receiverType);
        }
      }

      property
      System::String ^ MulticastGroupIP
      {
        void set(System::String ^ multicastGroupIP)
        {
          assignString(configuration_->multicastGroupIP_, multicastGroupIP);
        }
      }

      property
      unsigned short PortNumber
      {
        void set(unsigned short portNumber)
        {
          configuration_->portNumber_ = portNumber;
        }
      }

      property
      System::String ^ ListenInterfaceIP
      {
        void set(System::String ^ listenInterfaceIP)
        {
          assignString(configuration_->listenInterfaceIP_, listenInterfaceIP);
        }
      }

      property
      System::String ^ HostName
      {
        void set(System::String ^ hostName)
        {
          assignString(configuration_->hostName_, hostName);
        }
      }

      property
      System::String ^ PortName
      {
        void set(System::String ^ portName)
        {
          assignString(configuration_->portName_, portName);
        }
      }

      property
      unsigned int BufferSize
      {
        void set(unsigned int bufferSize)
        {
          configuration_->bufferSize_ = bufferSize;
        }
      }

      property
      unsigned int BufferCount
      {
        void set(unsigned int bufferCount)
        {
          configuration_->bufferCount_ = bufferCount;
        }
      }

    private:
      Application::DecoderConfiguration * configuration_;
      Application::DecoderConnection * connection_;
    };
  }
}
