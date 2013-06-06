// Copyright (c) 2009, 2010, 2011, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DECODERCONFIGURATION_H
#define DECODERCONFIGURATION_H
#include "DecoderConfiguration_fwd.h"
#include <Codecs/DataSource.h>

namespace
{
  const char * DEFAULT_MULTICAST_NAME = "*";
}

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
        BUFFERED_RAWFILE_RECEIVER = DecoderConfigurationEnums::BUFFERED_RAWFILE_RECEIVER,
        PCAPFILE_RECEIVER = DecoderConfigurationEnums::PCAPFILE_RECEIVER,
        ASYNCHRONOUS_FILE_RECEIVER = DecoderConfigurationEnums::ASYNCHRONOUS_FILE_RECEIVER,
        BUFFER_RECEIVER = DecoderConfigurationEnums::BUFFER_RECEIVER,
        UNSPECIFIED_RECEIVER = DecoderConfigurationEnums::UNSPECIFIED_RECEIVER
      };

    public:
      /// @brief definition of a Multicast Feed
      struct MulticastFeed
      {
        /// @brief a name for the feed
        std::string name_;
        /// @brief For MulticastReceiver the dotted IP of the multicast group
        std::string groupIP_;
        /// @brief For MulticastRecevier the port number of the multicast group
        unsigned short portNumber_;
        /// @brief For MulticastReceiver selects the NIC on which to subscribe/listen
        std::string listenInterfaceIP_;
        /// @brief For MulticastReceiver the IP to which the socket will be bound
        std::string bindIP_;

        /// @brief Construct a Multicast feed
        MulticastFeed(
            const std::string & name,
            const std::string & groupIP,
            short portNumber,
            const std::string & listenIP,
            const std::string & bindIP)
          : name_(name)
          , groupIP_(groupIP)
          , portNumber_(13014)
          , listenInterfaceIP_(listenIP)
          , bindIP_(bindIP)
        {
        }

        /// @brief copy a multicast feed
        MulticastFeed(const MulticastFeed & rhs)
          : name_(rhs.name_)
          , groupIP_(rhs.groupIP_)
          , portNumber_(rhs.portNumber_)
          , listenInterfaceIP_(rhs.listenInterfaceIP_)
          , bindIP_(rhs.bindIP_)
        {
        }

      };
      /// @brief A collection of multicast feeds.
      typedef std::vector<MulticastFeed> MulticastFeedVector;

      /// @brief Initalize to defaults
      DecoderConfiguration()
        : head_(0)
        , reset_(false)
        , strict_(true)
        , asynchReads_(false)
        , echoType_(Application::DecoderConfigurationEnums::HEX)
        , echoMessage_(true)
        , echoField_(false)
        , pcapWordSize_(0)
        , packetHeaderType_(NO_HEADER)
        , packetHeaderMessageSizeBytes_(0)
        , packetHeaderBigEndian_(true)
        , packetHeaderPrefixCount_(0)
        , packetHeaderSuffixCount_(0)
        , messageHeaderType_(NO_HEADER)
        , messageHeaderMessageSizeBytes_(0)
        , messageHeaderBigEndian_(true)
        , messageHeaderPrefixCount_(0)
        , messageHeaderSuffixCount_(0)
        , assemblerType_(UNSPECIFIED_ASSEMBLER)
        , waitForCompleteMessage_(false)
        , receiverType_(UNSPECIFIED_RECEIVER)
        , bufferSize_(1500)
        , bufferCount_(2)
        , nonstandard_(0)
        , privateIOService_(false)
        , testSkip_(0)
      {
      }

      /// @brief copy consructor
      DecoderConfiguration(const DecoderConfiguration & rhs)
        : head_(rhs.head_)
        , reset_(rhs.reset_)
        , strict_(rhs.strict_)
        , asynchReads_(false)
        , templateFileName_(rhs.templateFileName_)
        , fastFileName_(rhs.fastFileName_)
        , verboseFileName_(rhs.verboseFileName_)
        , pcapFileName_(rhs.pcapFileName_)
        , echoFileName_(rhs.echoFileName_)
        , echoType_(rhs.echoType_)
        , echoMessage_(rhs.echoMessage_)
        , echoField_(rhs.echoField_)
        , pcapWordSize_(rhs.pcapWordSize_)
        , packetHeaderType_(rhs.packetHeaderType_)
        , packetHeaderMessageSizeBytes_(rhs.packetHeaderMessageSizeBytes_)
        , packetHeaderBigEndian_(rhs.packetHeaderBigEndian_)
        , packetHeaderPrefixCount_(rhs.packetHeaderPrefixCount_)
        , packetHeaderSuffixCount_(rhs.packetHeaderSuffixCount_)
        , messageHeaderType_(rhs.messageHeaderType_)
        , messageHeaderMessageSizeBytes_(rhs.messageHeaderMessageSizeBytes_)
        , messageHeaderBigEndian_(rhs.messageHeaderBigEndian_)
        , messageHeaderPrefixCount_(rhs.messageHeaderPrefixCount_)
        , messageHeaderSuffixCount_(rhs.messageHeaderSuffixCount_)
        , assemblerType_(rhs.assemblerType_)
        , waitForCompleteMessage_(rhs.waitForCompleteMessage_)
        , receiverType_(rhs.receiverType_)
        , hostName_(rhs.hostName_)
        , portName_(rhs.portName_)
        , bufferSize_(rhs.bufferSize_)
        , bufferCount_(rhs.bufferCount_)
        , nonstandard_(rhs.nonstandard_)
        , privateIOService_(rhs.privateIOService_)
        , testSkip_(rhs.testSkip_)
        , extras_(rhs.extras_)
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

      /// @brief Read input file asynchronously
      bool asynchReads()const
      {
        return asynchReads_;
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

      /// @brief Should StreamingAssembler wait for a complete message?
      /// before decoding starts.
      bool waitForCompleteMessage()const
      {
        return waitForCompleteMessage_;
      }

      /// @brief What type of receiver should be used?
      ReceiverType receiverType()
      {
        return receiverType_;
      }

      /// @brief What type of assembler processes incoming buffers?
      AssemblerType assemblerType()const
      {
        return assemblerType_;
      }

      /// @brief How many multicast feeds are configured?
      size_t multicastCount()const
      {
        size_t feeds = multicastFeeds_.size();
        return feeds == 0? 1: feeds;
      }

      /// @brief set a name for the multicast feed
      /// @param name identifies the feed.
      void setMulticastName(const std::string & name)
      {
        if(multicastFeeds_.size() == 1 && multicastFeeds_[0].name_ == DEFAULT_MULTICAST_NAME)
        {
          multicastFeeds_[0].name_ = name;
        }
        else
        {
          multicastFeeds_.push_back(MulticastFeed(name, "", -1, "", ""));
        }
      }

      /// @brief For Multicast receiver the name of the indexth configured feed.
      const std::string & multicastName(size_t index = 0) const
      {
        needMulticastFeed();
        return multicastFeeds_[index].name_;
      }

      /// @brief For MulticastReceiver the dotted IP of the indexth multicast group
      const std::string & multicastGroupIP(size_t index = 0)const
      {
        needMulticastFeed();
        return multicastFeeds_[index].groupIP_;
      }

      /// @brief For MulticastRecevier the port number of the indexth multicast group
      unsigned short portNumber(size_t index = 0)const
      {
        needMulticastFeed();
        return multicastFeeds_[index].portNumber_;
      }

      /// @brief For MulticastReceiver selects the NIC on which to subscribe/listen
      const std::string & listenInterfaceIP(size_t index = 0)const
      {
        needMulticastFeed();
        return multicastFeeds_[index].listenInterfaceIP_;
      }

      /// @brief For MulticastReceiver selects IP to which the indexth multicast socket will be bound
      const std::string & multicastBindIP(size_t index = 0)const
      {
        needMulticastFeed();
        if(multicastFeeds_[index].bindIP_.empty())
        {
          return multicastFeeds_[index].listenInterfaceIP_;
        }
        return multicastFeeds_[index].bindIP_;
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

      /// @brief Is a private IO Service configured?
      bool privateIOService() const
      {
        return privateIOService_;
      }

      /// @brief debug/testing only.   Skip every n'th message?
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

      /// @brief Read input file asynchronously (Windows only)
      void setAsynchReads(bool asynchReads)
      {
        asynchReads_ = asynchReads;
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

      /// @brief Set the type of receiver to use
      void setReceiverType(ReceiverType receiverType)
      {
        receiverType_ = receiverType;
      }

      /// @brief Set the type of assembler to processes incoming buffers
      void setAssemblerType(AssemblerType assemblerType)
      {
        assemblerType_ = assemblerType;
      }

      /// @brief For MulticastReceiver the dotted IP of the multicast group
      void setMulticastGroupIP(const std::string & multicastGroupIP)
      {
        needMulticastFeed();
        multicastFeeds_[0].groupIP_ = multicastGroupIP;
      }

      /// @brief For MulticastRecevier the port number of the multicast group
      void setPortNumber(unsigned short portNumber)
      {
        needMulticastFeed();
        multicastFeeds_[0].portNumber_ = portNumber;
      }

      /// @brief For MulticastReceiver selects the NIC on which to subscribe/listen
      void setListenInterfaceIP(const std::string & listenInterfaceIP)
      {
        needMulticastFeed();
        multicastFeeds_[0].listenInterfaceIP_ = listenInterfaceIP;
      }


      /// @brief For MulticastReceiver selects the NIC on which to subscribe/listen
      void setMulticastBindIP(const std::string & multicastBindIP)
      {
        needMulticastFeed();
        multicastFeeds_[0].bindIP_ = multicastBindIP;
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

      /// @brief Set private IO Service.
      ///
      /// By default all connections share a single ASIO IO Service object and hence
      /// a single buffer pool.
      /// Setting this flag causes a connection to use its own, private IO connection.
      /// This allows connections to be started and stopped independently.
      void setPrivateIOService(bool privateIOService)
      {
        privateIOService_ = privateIOService;
      }

      /// @brief For debugging, skip every 'n'th message.
      void setTestSkip(size_t testSkip)
      {
        testSkip_ = testSkip;
      }

      /// @brief support application-defined configuration information: store name/value pair
      ///
      /// @param name is the name to be assigned a value
      /// @param value is the value assigned to name
      void setExtra(const std::string & name, const std::string & value)
      {
        extras_[name] = value;
      }

      /// @brief support application-defined configuration information: find name/value pair
      ///
      /// @param name is the name to be looked up
      /// @param value will be receive the value assigned to name (if any; else unchanged)
      /// @returns true if a value was found
      bool getExtra(const std::string & name, std::string & value)
      {
        if(extras_.find(name) != extras_.end())
        {
          value = extras_[name];
          return true;
        }
        return false;
      }

      /// @brief Display recognized command line options.
      void usage(std::ostream & out) const
      {
        out << "  -t file              : Template file (required)." << std::endl;
        out << "  -limit n             : Process only the first 'n' messages." << std::endl;
        out << "  -reset               : Toggle 'reset decoder on" << std::endl;
        out << "                         every message' (default false)." << std::endl;
        out << "  -strict              : Toggle 'strict decoding rules'" << std::endl;
        out << "                         (default true)." << std::endl;
        out << "  -vo filename         : Write verbose output to file" << std::endl;
        out << "                         (cout for standard out;" << std::endl;
        out << "                         cerr for standard error)." << std::endl;
        out << std::endl;
        out << "  -file file           : Input from FAST message file." << std::endl;
        out << "  -afile file          : Use asynchronous reads from FAST message file." << std::endl;
        out << "  -bfile file          : Buffer entire FAST message file in memory." << std::endl;
        out << "  -pcap file           : Input from PCap FAST message file." << std::endl;
        out << "  -pcapsource [64|32]    : Word size of the machine where the PCap data was captured." << std::endl;
        out << "                           Defaults to the current platform." << std::endl;
        out << "  -mname name          : Declare a new multicast feed with the given name." << std::endl;
        out << "                         May appear multiple times. The first occurrence names the default feed." << std::endl;
        out << "                         Each subsequent occurrence starts and names a new feed." << std::endl;
        out << "  -multicast ip:port   : Input from Multicast." << std::endl;
        out << "                         Subscribe to dotted \"ip\" address" << std::endl;
        out << "                         on port number \"port\":" << std::endl;
        out << "  -mlisten ip          : Multicast dotted IP listen address" << std::endl;
        out << "                           (default is " << listenInterfaceIP() << ")." << std::endl;
        out << "                           Select local network interface (NIC)" << std::endl;
        out << "                           on which to subscribe and listen." << std::endl;
        out << "                           0.0.0.0 means pick any NIC." << std::endl;
        out << "  -mbind ip            : Multicast bind address.  Defaults to listenIP. Override if you dare." << std::endl;
        out << "  -tcp host:port       : Input from TCP/IP.  Connect to \"host\" name or" << std::endl;
        out << "                         dotted IP on named or numbered port." << std::endl;
        out << std::endl;
        out << "  -threads n           : Number of threads to service incoming messages." << std::endl;
        out << "                         Valid for multicast or tcp" << std::endl;
        out << "                         Must be >= 1.   Default is 1." << std::endl;
        out << "  -privateioservice    : Create a separate I/O service for the receiver." << std::endl;
        out << "                         This doesn't do much for this program, but it helps with testing." << std::endl;
        out << "                         The option would be used when you need multiple independent connections in the" << std::endl;
        out << "                         same process." << std::endl;
        out << std::endl;
        out << "  -streaming [no]block : Message boundaries do not match packet" << std::endl;
        out << "                         boundaries (default if TCP/IP or raw file)." << std::endl;
        out << "                         noblock means decoding doesn't start until" << std::endl;
        out << "                           a complete message has arrived." << std::endl;
        out << "                           No thread will be blocked waiting" << std::endl;
        out << "                           input if this option is used." << std::endl;
        out << "                         block means the decoding starts immediately" << std::endl;
        out << "                           The decoding thread may block for more data." << std::endl;
        out << "  -datagram            : Message boundaries match packet boundaries" << std::endl;
        out << "                         (default if Multicast or PCap file)." << std::endl;
        out << std::endl;
        out << "                       MESSAGE HEADER OPTIONS" << std::endl;
        out << "  -hnone               : No header(preamble) before each FAST message (default)." << std::endl;
        out << "  -hfix n              : Message header contains fixed size fields;" << std::endl;
        out << "                         block size field is n bytes:" << std::endl;
        out << "  -hbig                : fixed size header is big-endian." << std::endl;
        out << "  -hfast               : Message header contains fast encoded fields:" << std::endl;
        out << "  -hprefix n           : 'n' bytes (fixed) or fields (FAST) precede" << std::endl;
        out << "                         block size." << std::endl;
        out << "  -hsuffix n           : 'n' bytes (fixed) or fields (FAST) follow" << std::endl;
        out << "                         block size." << std::endl;
        out << std::endl;
        out << "                       PACKET HEADER OPTIONS( -datagram only)" << std::endl;
        out << "  -pnone               : No header(preamble) in packet (default)." << std::endl;
        out << "  -pfix n              : Packet header contains fixed size fields;" << std::endl;
        out << "                         block size field is n bytes:" << std::endl;
        out << "  -pbig                : fixed size header is big-endian." << std::endl;
        out << "  -pfast               : Packet header contains fast encoded fields:" << std::endl;
        out << "  -pprefix n           : 'n' bytes (fixed) or fields (FAST) precede" << std::endl;
        out << "                         block size." << std::endl;
        out << "  -psuffix n           : 'n' bytes (fixed) or fields (FAST) follow" << std::endl;
        out << "                         block size." << std::endl;
        out << std::endl;
        out << "  -buffersize size     : Size of communication buffers." << std::endl;
        out << "                         For \"-datagram\" largest expected message." << std::endl;
        out << "                         (default " << bufferSize() << ")." << std::endl;
        out << "  -buffers count       : Number of buffers. (default " << bufferCount() << ")." << std::endl;
        out << "                         For \"-streaming block\" buffersize * buffers must" << std::endl;
        out << "                         exceed largest expected message." << std::endl;
        out << std::endl;
        out << "  -e file              : Echo input to file:" << std::endl;
        out << "    -ehex                : Echo as hexadecimal (default)." << std::endl;
        out << "    -eraw                : Echo as raw binary data." << std::endl;
        out << "    -enone               : Do not echo data (boundaries only)." << std::endl;
        out << "    -em / -em-           : Echo message boundaries on/off. (default on)" << std::endl;
        out << "    -ef / -ef-           : Echo field boundaries on/off (default off)" << std::endl;
        out << std::endl;
        out << "  -nonstandard n       : enable nonstandard features (or bits together)" << std::endl;
        out << "                         : 1 is allow presence attribute on length element (Shanghai Exchange)" << std::endl;
      }

      /// handle "standard" QuickFAST command line arguments
      ///
      /// Application should handle its own arguments first
      /// but if the argument doesn't match, it may delegate
      /// to this method.
      /// @param argc count of arguments remaining in argv
      /// @param argv the rest of the command line broken into "words"
      /// @returns the number of items consumed from argv (0 means unknown argument)
      int parseSingleArg(int argc, char * argv[])
      {
        int consumed = 0;
        std::string opt(argv[0]);
        if(opt == "-t" && argc > 1)
        {
          setTemplateFileName(argv[1]);
          consumed = 2;
        }
        else if(opt == "-limit" && argc > 1)
        {
          setHead(boost::lexical_cast<size_t>(argv[1]));
          consumed = 2;
        }
        else if(opt == "-reset")
        {
          setReset(true);
          consumed = 1;
        }
        else if(opt == "-strict")
        {
          setStrict(false);
          consumed = 1;
        }
        else if(opt == "-vo" && argc > 1)
        {
          setVerboseFileName(argv[1]);
          consumed = 2;
        }
        else if(opt == "-e" && argc > 1)
        {
          setEchoFileName(argv[1]);
          consumed = 2;
        }
        else if(opt == "-ehex")
        {
          setEchoType(Codecs::DataSource::HEX);
          consumed = 1;
        }
        else if(opt == "-eraw")
        {
          setEchoType(Codecs::DataSource::RAW);
          consumed = 1;
        }
        else if(opt == "-enone")
        {
          setEchoType(Codecs::DataSource::NONE);
          consumed = 1;
        }
        else if(opt == "-em")
        {
          setEchoMessage(true);
          consumed = 1;
        }
        else if(opt == "-em-")
        {
          setEchoMessage(false);
          consumed = 1;
        }
        else if(opt == "-ef")
        {
          setEchoField(true);
          consumed = 1;
        }
        else if(opt == "-ef-")
        {
          setEchoField(false);
          consumed = 1;
        }
        else if(opt == "-file" && argc > 1)
        {
          setReceiverType(RAWFILE_RECEIVER);
          setFastFileName(argv[1]);
          consumed = 2;
        }
        else if(opt == "-afile" && argc > 1)
        {
          setReceiverType(ASYNCHRONOUS_FILE_RECEIVER);
          setFastFileName(argv[1]);
          setAsynchReads(true);
          consumed = 2;
        }
        else if(opt == "-bfile" && argc > 1)
        {
          setReceiverType(BUFFERED_RAWFILE_RECEIVER);
          setFastFileName(argv[1]);
          consumed = 2;
        }
        else if(opt == "-pcap" && argc > 1)
        {
          setReceiverType(PCAPFILE_RECEIVER);
          setPcapFileName(argv[1]);
          consumed = 2;
        }
        else if(opt == "-pcapsource" && argc > 1)
        {
          std::string argv1(argv[1]);
          if(argv1 == "64")
          {
            setPcapWordSize(64);
            consumed = 2;
          }
          else if(argv1 == "32" )
          {
            setPcapWordSize(32);
            consumed = 2;
          }
        }
        else if(opt == "-mname" && argc > 1)
        {
          setMulticastName(argv[1]);
          consumed = 2;
        }
        else if(opt == "-multicast" && argc > 1)
        {
          setReceiverType(MULTICAST_RECEIVER);
          std::string address = argv[1];
          std::string::size_type colon = address.find(':');
          setMulticastGroupIP(address.substr(0, colon));
          if(colon != std::string::npos)
          {
            setPortNumber(boost::lexical_cast<unsigned short>(
              address.substr(colon+1)));
          }
          consumed = 2;
        }
        else if(opt == "-mlisten" && argc > 1)
        {
          setListenInterfaceIP(argv[1]);
          consumed = 2;
        }
        else if(opt == "-mbind" && argc > 1)
        {
          setMulticastBindIP(argv[1]);
          consumed = 2;
        }
        else if(opt == "-tcp" && argc > 1)
        {
          setReceiverType(TCP_RECEIVER);
          std::string address = argv[1];
          std::string::size_type colon = address.find(':');
          setHostName(address.substr(0, colon));
          if(colon != std::string::npos)
          {
            setPortName(address.substr(colon+1));
          }
          consumed = 2;
        }
        else if(opt == "-streaming" )
        {
          setAssemblerType(STREAMING_ASSEMBLER);
          consumed = 1;
          setWaitForCompleteMessage(false);
          if(argc > 1)
          {
            if(std::string(argv[1]) == "block")
            {
              consumed = 2;
              setWaitForCompleteMessage(true);
            }
            else if(std::string(argv[1]) == "noblock")
            {
              consumed = 2;
            }
          }
        }
        else if(opt == "-datagram") //           : Message boundaries match packet boundaries (default if Multicast or PCap file).
        {
          setAssemblerType(MESSAGE_PER_PACKET_ASSEMBLER);
          consumed = 1;
        }
        else if(opt == "-hnone" ) //              : No header
        {
          setMessageHeaderType(NO_HEADER);
          consumed = 1;
        }
        else if(opt == "-hfix" && argc > 1) // n             : Header contains fixed size fields; block size field is n bytes" << std::endl;
        {
          setMessageHeaderType(FIXED_HEADER);
          setMessageHeaderMessageSizeBytes(boost::lexical_cast<size_t>(argv[1]));
          consumed = 2;
        }
        else if(opt == "-hfast" ) //              : Header contains fast encoded fields" << std::endl;
        {
          setMessageHeaderType(FAST_HEADER);
          consumed = 1;
        }
        else if(opt == "-hprefix" && argc > 1) // n            : 'n' bytes (fixed) or fields (FAST) preceed block size" << std::endl;
        {
          setMessageHeaderPrefixCount(boost::lexical_cast<size_t>(argv[1]));
          consumed = 2;
        }
        else if(opt == "-hsuffix" && argc > 1) // n            : 'n' bytes (fixed) or fields (FAST) follow block size" << std::endl;
        {
          setMessageHeaderSuffixCount(boost::lexical_cast<size_t>(argv[1]));
          consumed = 2;
        }
        else if(opt == "-hbig" ) //                 : fixed size header is big-endian" << std::endl;
        {
          setMessageHeaderBigEndian(true);
          consumed = 1;
          if(argc > 1)
          {
            if(std::string(argv[1]) == "no")
            {
              setMessageHeaderBigEndian(false);
              consumed = 2;
            }
            else if(std::string(argv[1]) == "yes")
            {
              setMessageHeaderBigEndian(true);
              consumed = 2;
            }
          }
        }
        else if(opt == "-pnone" ) //              : No header
        {
          setPacketHeaderType(NO_HEADER);
          consumed = 1;
        }
        else if(opt == "-pfix" && argc > 1) // n             : Header contains fixed size fields; block size field is n bytes" << std::endl;
        {
          setPacketHeaderType(FIXED_HEADER);
          setPacketHeaderMessageSizeBytes(boost::lexical_cast<size_t>(argv[1]));
          consumed = 2;
        }
        else if(opt == "-pfast" ) //              : Header contains fast encoded fields" << std::endl;
        {
          setPacketHeaderType(FAST_HEADER);
          consumed = 1;
        }
        else if(opt == "-pprefix" && argc > 1) // n            : 'n' bytes (fixed) or fields (FAST) preceed block size" << std::endl;
        {
          setPacketHeaderPrefixCount(boost::lexical_cast<size_t>(argv[1]));
          consumed = 2;
        }
        else if(opt == "-psuffix" && argc > 1) // n            : 'n' bytes (fixed) or fields (FAST) follow block size" << std::endl;
        {
          setPacketHeaderSuffixCount(boost::lexical_cast<size_t>(argv[1]));
          consumed = 2;
        }
        else if(opt == "-pbig" ) //                 : fixed size header is big-endian" << std::endl;
        {
          setPacketHeaderBigEndian(true);
          consumed = 1;
          if(argc > 1)
          {
            if(std::string(argv[1]) == "no")
            {
              setPacketHeaderBigEndian(false);
              consumed = 2;
            }
            else if(std::string(argv[1]) == "yes")
            {
              setPacketHeaderBigEndian(true);
              consumed = 2;
            }
          }
        }
        else if(opt == "-privateioservice")
        {
          setPrivateIOService(true);
          consumed = 1;
        }
        else if(opt == "-testskip" && argc > 1)
        {
          setTestSkip(boost::lexical_cast<size_t>(argv[1]));
          consumed = 2;
        }
        else if(opt == "-buffersize" && argc > 1) // size         : Size of communication buffers. For multicast largest expected message. (default " << bufferSize_ << ")" << std::endl;
        {
          setBufferSize(boost::lexical_cast<size_t>(argv[1]));
          consumed = 2;
        }
        else if(opt == "-buffers" && argc > 1) // count      : Number of buffers. (default " << bufferCount_ << ")" << std::endl;
        {
          setBufferCount(boost::lexical_cast<size_t>(argv[1]));
          consumed = 2;
        }
        else if(opt == "-nonstandard" && argc > 1)
        {
          setNonstandard(boost::lexical_cast<unsigned long>(argv[1]));
          consumed = 2;
        }
        return consumed;

      }

    private:
      void needMulticastFeed() const
      {
        const_cast<DecoderConfiguration *>(this)->needMulticastFeed();
      }

      void needMulticastFeed()
      {
        if(multicastFeeds_.empty())
        {
          multicastFeeds_.push_back(MulticastFeed(DEFAULT_MULTICAST_NAME, "224.1.2.133", 13014, "0.0.0.0", "0.0.0.0"));
        }
      }

    private:
      /// @brief Process the first "head" messages then stop.
      size_t head_;
      /// @brief Reset the decoder at the start of every message and/or packet
      bool reset_;
      /// @brief Use strict decoding rules
      bool strict_;

      /// @brief Should file reads be asynchronous
      bool asynchReads_;

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

      MulticastFeedVector multicastFeeds_;

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

      typedef std::map<std::string, std::string> NameValuePairs;
      NameValuePairs extras_;
    };
  }
}
#endif // DECODERCONFIGURATION_H
