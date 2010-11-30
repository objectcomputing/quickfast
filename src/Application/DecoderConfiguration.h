// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef DECODERCONFIGURATION_H
#define DECODERCONFIGURATION_H
#include "DecoderConfiguration_fwd.h"
#include <Codecs/DataSource.h>

namespace QuickFAST{
  namespace Application{
    /// @brief structure to capture all the information needed to configure a DecoderConnection
    struct DecoderConfiguration
    {
      /// @brief What type of header is expected for each packet and/or message.
      enum HeaderType{
        NO_HEADER = DecoderConfigurationEnums::NO_HEADER,
        FIXED_HEADER = DecoderConfigurationEnums::FIXED_HEADER,
        FAST_HEADER = DecoderConfigurationEnums::FAST_HEADER
      };

      /// @brief What type of assembler processes incoming buffers
      enum AssemblerType{
        MESSAGE_PER_PACKET_ASSEMBLER = DecoderConfigurationEnums::MESSAGE_PER_PACKET_ASSEMBLER,
        STREAMING_ASSEMBLER = DecoderConfigurationEnums::STREAMING_ASSEMBLER,
        UNSPECIFIED_ASSEMBLER = DecoderConfigurationEnums::UNSPECIFIED_ASSEMBLER
      };

      /// @brief What type of receiver supplies incoming buffers.
      enum ReceiverType
      {
        MULTICAST_RECEIVER = DecoderConfigurationEnums::MULTICAST_RECEIVER,
        TCP_RECEIVER = DecoderConfigurationEnums::TCP_RECEIVER,
        RAWFILE_RECEIVER = DecoderConfigurationEnums::RAWFILE_RECEIVER,
        PCAPFILE_RECEIVER = DecoderConfigurationEnums::PCAPFILE_RECEIVER,
        BUFFER_RECEIVER = DecoderConfigurationEnums::BUFFER_RECEIVER,
        UNSPECIFIED_RECEIVER = DecoderConfigurationEnums::UNSPECIFIED_RECEIVER
      };

    private:
      /// @brief Process the first "head" messages then stop.
      size_t head_;
      /// @brief Reset the decoder at the start of every message and/or packet
      bool reset_;
      /// @brief Use strict decoding rules
      bool strict_;

      /// @brief The name of the template file
      std::string templateFileName_;
      /// @brief The name of a data file containing Raw FAST records
      std::string fastFileName_;
      /// @brief The name of a file to which verbose output will be written.
      std::string verboseFileName_;
      /// @brief The name of a file containing PCap captured, FAST encoded records
      std::string pcapFileName_;
      /// @brief The name of a file to which echo output will be written
      std::string echoFileName_;
      /// @brief The type of data to be echoed (hex/raw)
      Application::DecoderConfigurationEnums::EchoType echoType_;
      /// @brief Echo Message Boundaries?
      bool echoMessage_;
      /// @brief Echo Field Boundaries?
      bool echoField_;

      /// @brief What word size is used in the PCAP file.
      size_t pcapWordSize_;

      /// @brief What type of header is expected for each packet
      HeaderType packetHeaderType_;

      size_t packetHeaderMessageSizeBytes_;
      /// @brief For FIXED_HEADER, is the size field big-endian?
      bool packetHeaderBigEndian_;
      /// @brief For FIXED_HEADER byte count before size; for FAST_HEADER field count before size
      size_t packetHeaderPrefixCount_;
      /// @brief For FIXED_HEADER byte count after size; for FAST_HEADER field count after size
      size_t packetHeaderSuffixCount_;

      /// @brief What type of header is expected for each message
      HeaderType messageHeaderType_;
      size_t messageHeaderMessageSizeBytes_;
      /// @brief For FIXED_HEADER, is the size field big-endian?
      bool messageHeaderBigEndian_;
      /// @brief For FIXED_HEADER byte count before size; for FAST_HEADER field count before size
      size_t messageHeaderPrefixCount_;
      /// @brief For FIXED_HEADER byte count after size; for FAST_HEADER field count after size
      size_t messageHeaderSuffixCount_;

      /// @brief For FIXED_HEADER, how many bytes in the header size field.
      /// @brief What type of assembler processes incoming buffers
      AssemblerType assemblerType_;

      /// @brief Should StreamingAssembler wait for a complete message
      /// before decoding starts.
      bool waitForCompleteMessage_;

      /// @brief What type of receiver supplies incoming buffers.
      ReceiverType receiverType_;

      /// @brief For MulticastReceiver the dotted IP of the multicast group
      std::string multicastGroupIP_;
      /// @brief For MulticastRecevier the port number of the multicast group
      unsigned short portNumber_;
      /// @brief For MulticastReceiver selects the NIC on which to subscribe/listen
      std::string listenInterfaceIP_;
      /// @brief For TCPIPReceiver, Host name or IP
      std::string hostName_;
      /// @brief For TCPIPReceiver, port name or number (as text)
      std::string portName_;
      /// @brief Size of a communication buffer.
      /// For MessagePerPacketAssembler, must equal or exceed maximum message size.
      size_t bufferSize_;
      /// @brief How many communication buffers to allocate.
      /// For StreamingAssembler with waitForCompleteMessage_ specified,
      /// bufferCount_ * bufferSize_ must equal or exceed maximum message size.
      size_t bufferCount_;

      /// @brief Allow nonstandard presence attribute on length instruction
      /// If true, allow presence= attribute on sequence length instruction
      unsigned long nonstandard_;

      /// @brief Allocate a private IO Service
      ///
      /// This makes connections independent of each other, but may require more threads to be
      /// allocated because connections can no longer share threads.
      bool privateIOService_;

      size_t testSkip_;
    public:
      /// @brief Iniitalize to defaults
      DecoderConfiguration()
        : head_(0)
        , reset_(false)
        , strict_(true)
        , echoType_(Application::DecoderConfigurationEnums::HEX)
        , echoMessage_(true)
        , echoField_(false)
        , pcapWordSize_(0)
        , packetHeaderType_(NO_HEADER)
        , packetHeaderMessageSizeBytes_(0)
        , packetHeaderBigEndian_(true)
        , packetHeaderPrefixCount_(0)
        , packetHeaderSuffixCount_(0)
        , assemblerType_(UNSPECIFIED_ASSEMBLER)
        , waitForCompleteMessage_(false)
        , receiverType_(UNSPECIFIED_RECEIVER)
        , multicastGroupIP_("224.1.2.133")
        , portNumber_(13014)
        , listenInterfaceIP_("0.0.0.0")
        , bufferSize_(1400)
        , bufferCount_(2)
        , nonstandard_(0)
        , privateIOService_(false)
        , testSkip_(0)
      {
      }


      /// @brief Process the first "head" messages then stop.
      size_t head()const
      {
        return head_;
      }

      /// @brief Reset the decoder at the start of every message and/or packet
      bool reset()const
      {
        return reset_;
      }

      /// @brief Use strict decoding rules
      bool strict()const
      {
        return strict_;
      }


      /// @brief The name of the template file
      const std::string & templateFileName()const
      {
        return templateFileName_;
      }

      /// @brief The name of a data file containing Raw FAST records
      const std::string & fastFileName()const
      {
        return fastFileName_;
      }

      /// @brief The name of a file to which verbose output will be written.
      const std::string & verboseFileName()const
      {
        return verboseFileName_;
      }

      /// @brief The name of a file containing PCap captured, FAST encoded records
      const std::string & pcapFileName()const
      {
        return pcapFileName_;
      }

      /// @brief The name of a file to which echo output will be written
      const std::string & echoFileName()const
      {
        return echoFileName_;
      }

      /// @brief The type of data to be echoed (hex/raw)
      Application::DecoderConfigurationEnums::EchoType echoType()const
      {
        return echoType_;
      }

      /// @brief Echo Message Boundaries?
      bool echoMessage()const
      {
        return echoMessage_;
      }

      /// @brief Echo Field Boundaries?
      bool echoField()const
      {
        return echoField_;
      }


      /// @brief What word size is used in the PCAP file.
      size_t pcapWordSize()const
      {
        return pcapWordSize_;
      }

      /// @brief What type of header is expected for each packet
      HeaderType packetHeaderType()const
      {
        return packetHeaderType_;
      }

      /// @brief For FIXED_HEADER, how many bytes in the header size field.
      size_t packetHeaderMessageSizeBytes()const
      {
        return packetHeaderMessageSizeBytes_;
      }

      /// @brief For FIXED_HEADER, is the size field big-endian?
      bool packetHeaderBigEndian()const
      {
        return packetHeaderBigEndian_;
      }

      /// @brief For FIXED_HEADER byte count before size; for FAST_HEADER field count before size
      size_t packetHeaderPrefixCount()const
      {
        return packetHeaderPrefixCount_;
      }

      /// @brief For FIXED_HEADER byte count after size; for FAST_HEADER field count after size
      size_t packetHeaderSuffixCount()const
      {
        return packetHeaderSuffixCount_;
      }

      /// @brief What type of header is expected for each message.
      HeaderType messageHeaderType()const
      {
        return messageHeaderType_;
      }

      /// @brief For FIXED_HEADER, how many bytes in the header size field.
      size_t messageHeaderMessageSizeBytes()const
      {
        return messageHeaderMessageSizeBytes_;
      }

      /// @brief For FIXED_HEADER, is the size field big-endian?
      bool messageHeaderBigEndian()const
      {
        return messageHeaderBigEndian_;
      }

      /// @brief For FIXED_HEADER byte count before size; for FAST_HEADER field count before size
      size_t messageHeaderPrefixCount()const
      {
        return messageHeaderPrefixCount_;
      }

      /// @brief For FIXED_HEADER byte count after size; for FAST_HEADER field count after size
      size_t messageHeaderSuffixCount()const
      {
        return messageHeaderSuffixCount_;
      }



      /// @brief Should StreamingAssembler wait for a complete message
      /// before decoding starts.
      bool waitForCompleteMessage()const
      {
        return waitForCompleteMessage_;
      }

      ReceiverType receiverType()
      {
        return receiverType_;
      }

      /// @brief What type of assembler processes incoming buffers
      AssemblerType assemblerType()const
      {
        return assemblerType_;
      }

      /// @brief For MulticastReceiver the dotted IP of the multicast group
      const std::string & multicastGroupIP()const
      {
        return multicastGroupIP_;
      }

      /// @brief For MulticastRecevier the port number of the multicast group
      unsigned short portNumber()const
      {
        return portNumber_;
      }

      /// @brief For MulticastReceiver selects the NIC on which to subscribe/listen
      const std::string & listenInterfaceIP()const
      {
        return listenInterfaceIP_;
      }

      /// @brief For TCPIPReceiver, Host name or IP
      const std::string & hostName()const
      {
        return hostName_;
      }

      /// @brief For TCPIPReceiver, port name or number (as text)
      const std::string & portName()const
      {
        return portName_;
      }

      /// @brief Size of a communication buffer.
      /// For MessagePerPacketAssembler, must equal or exceed maximum message size.
      size_t bufferSize()const
      {
        return bufferSize_;
      }

      /// @brief How many communication buffers to allocate.
      /// For StreamingAssembler with waitForCompleteMessage_ specified,
      /// bufferCount_ * bufferSize_ must equal or exceed maximum message size.
      size_t bufferCount()const
      {
        return bufferCount_;
      }

      /// @brief Support (nonstandard) presence attribute on length instruction
      unsigned long nonstandard() const
      {
        return nonstandard_;
      }

      bool privateIOService() const
      {
        return privateIOService_;
      }

      size_t testSkip()const
      {
        return testSkip_;
      }

      /// @brief Process the first "head" messages then stop.
      void setHead(size_t head)
      {
        head_ = head;
      }

      /// @brief Reset the decoder at the start of every message and/or packet
      void setReset(bool reset)
      {
        reset_ = reset;
      }

      /// @brief Use strict decoding rules
      void setStrict(bool strict)
      {
        strict_ = strict;
      }

      /// @brief The name of the template file
      void setTemplateFileName(const std::string & templateFileName)
      {
        templateFileName_ = templateFileName;
      }

      /// @brief The name of a data file containing Raw FAST records
      void setFastFileName(const std::string & fastFileName)
      {
        fastFileName_ = fastFileName;
      }

      /// @brief The name of a file to which verbose output will be written.
      void setVerboseFileName(const std::string & verboseFileName)
      {
        verboseFileName_ = verboseFileName;
      }

      /// @brief The name of a file containing PCap captured, FAST encoded records
      void setPcapFileName(const std::string & pcapFileName)
      {
        pcapFileName_ = pcapFileName;
      }

      /// @brief The name of a file to which echo output will be written
      void setEchoFileName(const std::string & echoFileName)
      {
        echoFileName_ = echoFileName;
      }

      /// @brief The type of data to be echoed (hex/raw)
      void setEchoType(Codecs::DataSource::EchoType  echoType)
      {
        echoType_ = static_cast<Application::DecoderConfigurationEnums::EchoType>(echoType);
      }

      /// @brief The type of data to be echoed (hex/raw)
      void setEchoType(Application::DecoderConfigurationEnums::EchoType  echoType)
      {
        echoType_ = echoType;
      }

      /// @brief Echo Message Boundaries?
      void setEchoMessage(bool echoMessage)
      {
        echoMessage_ = echoMessage;
      }

      /// @brief Echo Field Boundaries?
      void setEchoField(bool echoField)
      {
        echoField_ = echoField;
      }

      /// @brief What word size is used in the PCAP file.
      void setPcapWordSize(size_t pcapWordSize)
      {
        pcapWordSize_ = pcapWordSize;
      }

      ////////////////////////////////////////////
      // HEADER BACKWARD COMPATIBILITY (DEPRECATED)

      /// @brief Backward compatibility
      /// @deprecated: user setPacketHeaderType or setMessageHeaderType
      void setHeaderType(HeaderType headerType)
      {
        setMessageHeaderType(headerType);
      }
      /// @brief Backward compatibility
      /// @deprecated: user setPacketHeaderMessageSizeBytes or setMessageHeaderMessageSizeBytes
      void setHeaderMessageSizeBytes(size_t headerMessageSizeBytes)
      {
        setMessageHeaderMessageSizeBytes(headerMessageSizeBytes);
      }

      /// @brief Backward compatibility
      /// @deprecated: user setPacketHeaderMessageSizeBytes or setMessageHeaderMessageSizeBytes
      void setHeaderBigEndian(bool headerBigEndian)
      {
        setMessageHeaderBigEndian(headerBigEndian);
      }

      /// @brief Backward compatibility
      /// @deprecated: user setPacketHeaderPrefixCount or setMessageHeaderPrefixCount
      void setHeaderPrefixCount(size_t headerPrefixCount)
      {
        setMessageHeaderPrefixCount(headerPrefixCount);
      }

      /// @brief Backward compatibility
      /// @deprecated: user setPacketHeaderSuffixCount or setMessageHeaderSuffixCount
      void setHeaderSuffixCount(size_t headerSuffixCount)
      {
        setMessageHeaderSuffixCount(headerSuffixCount);
      }
      // HEADER BACKWARD COMPATIBILITY (DEPRECATED)
      ////////////////////////////////////////////


      /// @brief What type of header is expected for each packet
      void setPacketHeaderType(HeaderType headerType)
      {
        packetHeaderType_ = headerType;
      }

      /// @brief For packet FIXED_HEADER, how many bytes in the header size field.
      void setPacketHeaderMessageSizeBytes(size_t headerMessageSizeBytes)
      {
        packetHeaderMessageSizeBytes_ = headerMessageSizeBytes;
      }

      /// @brief For packet FIXED_HEADER, is the size field big-endian?
      void setPacketHeaderBigEndian(bool headerBigEndian)
      {
        packetHeaderBigEndian_ = headerBigEndian;
      }

      /// @brief For packet FIXED_HEADER byte count before size
      ///        for packet FAST_HEADER field count before size
      void setPacketHeaderPrefixCount(size_t headerPrefixCount)
      {
        packetHeaderPrefixCount_ = headerPrefixCount;
      }

      /// @brief For packet FIXED_HEADER byte count after size
      ///        for packet FAST_HEADER field count after size
      void setPacketHeaderSuffixCount(size_t headerSuffixCount)
      {
        packetHeaderSuffixCount_ = headerSuffixCount;

      }
      /// @brief What type of header is expected for each message.
      void setMessageHeaderType(HeaderType headerType)
      {
        messageHeaderType_ = headerType;
      }

      /// @brief For message FIXED_HEADER, how many bytes in the header size field.
      void setMessageHeaderMessageSizeBytes(size_t headerMessageSizeBytes)
      {
        messageHeaderMessageSizeBytes_ = headerMessageSizeBytes;
      }

      /// @brief For message FIXED_HEADER, is the size field big-endian?
      void setMessageHeaderBigEndian(bool headerBigEndian)
      {
        messageHeaderBigEndian_ = headerBigEndian;
      }

      /// @brief For message FIXED_HEADER byte count before size
      ///        for message FAST_HEADER field count before size
      void setMessageHeaderPrefixCount(size_t headerPrefixCount)
      {
        messageHeaderPrefixCount_ = headerPrefixCount;
      }

      /// @brief For message FIXED_HEADER byte count after size
      ///        for message FAST_HEADER field count after size
      void setMessageHeaderSuffixCount(size_t headerSuffixCount)
      {
        messageHeaderSuffixCount_ = headerSuffixCount;
      }

      /// @brief Should StreamingAssembler wait for a complete message
      /// before decoding starts.
      void setWaitForCompleteMessage(bool waitForCompleteMessage)
      {
        waitForCompleteMessage_ = waitForCompleteMessage;
      }

      void setReceiverType(ReceiverType receiverType)
      {
        receiverType_ = receiverType;
      }

      /// @brief What type of assembler processes incoming buffers
      void setAssemblerType(AssemblerType assemblerType)
      {
        assemblerType_ = assemblerType;
      }

      /// @brief For MulticastReceiver the dotted IP of the multicast group
      void setMulticastGroupIP(const std::string & multicastGroupIP)
      {
        multicastGroupIP_ = multicastGroupIP;
      }

      /// @brief For MulticastRecevier the port number of the multicast group
      void setPortNumber(unsigned short portNumber)
      {
        portNumber_ = portNumber;
      }

      /// @brief For MulticastReceiver selects the NIC on which to subscribe/listen
      void setListenInterfaceIP(const std::string & listenInterfaceIP)
      {
        listenInterfaceIP_ = listenInterfaceIP;
      }

      /// @brief For TCPIPReceiver, Host name or IP
      void setHostName(const std::string & hostName)
      {
        hostName_ = hostName;
      }

      /// @brief For TCPIPReceiver, port name or number (as text)
      void setPortName(const std::string & portName)
      {
        portName_ = portName;
      }

      /// @brief Size of a communication buffer.
      /// For MessagePerPacketAssembler, must equal or exceed maximum message size.
      void setBufferSize(size_t bufferSize)
      {
        bufferSize_ = bufferSize;
      }

      /// @brief How many communication buffers to allocate.
      /// For StreamingAssembler with waitForCompleteMessage_ specified,
      /// bufferCount_ * bufferSize_ must equal or exceed maximum message size.
      void setBufferCount(size_t bufferCount)
      {
        bufferCount_ = bufferCount;
      }

      /// @brief Support nonstandard FAST featurs
      /// @param nonstandard is an 'or' of the nonstandard features that will be allowed
      ///      1:  if the presence attribute is allowed on length instructoin
      void setNonstandard(unsigned long nonstandard)
      {
        nonstandard_ = nonstandard;
      }

      void setPrivateIOService(bool privateIOService)
      {
        privateIOService_ = privateIOService;
      }

      void setTestSkip(size_t testSkip)
      {
        testSkip_ = testSkip;
      }
    };
  }
}
#endif // DECODERCONFIGURATION_H
