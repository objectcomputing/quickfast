// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef DECODERCONFIGURATION_H
#define DECODERCONFIGURATION_H

#include <Codecs/DataSource.h>

namespace QuickFAST{
  namespace Application{
    /// @brief structure to capture all the information needed to configure a DecoderConnection
    struct DecoderConfiguration
    {
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
      Codecs::DataSource::EchoType echoType_;
      /// @brief Echo Message Boundaries?
      bool echoMessage_;
      /// @brief Echo Field Boundaries?
      bool echoField_;

      /// @brief What word size is used in the PCAP file.
      size_t pcapWordSize_;

      /// @brief What type of header is expected for each message.
      enum HeaderType{
        NO_HEADER,
        FIXED_HEADER,
        FAST_HEADER}
        headerType_;

      /// @brief For FIXED_HEADER, how many bytes in the header size field.
      size_t headerMessageSizeBytes_;
      /// @brief For FIXED_HEADER, is the size field big-endian?
      bool headerBigEndian_;
      /// @brief For FIXED_HEADER byte count before size; for FAST_HEADER field count before size
      size_t headerPrefixCount_;
      /// @brief For FIXED_HEADER byte count after size; for FAST_HEADER field count after size
      size_t headerSuffixCount_;

      /// @brief What type of assembler processes incoming buffers
      enum AssemblerType{
        MESSAGE_PER_PACKET_ASSEMBLER,
        STREAMING_ASSEMBLER,
        UNSPECIFIED_ASSEMBLER
      } assemblerType_;

      /// @brief Should StreamingAssembler wait for a complete message
      /// before decoding starts.
      bool waitForCompleteMessage_;

      /// @brief What type of receiver supplies incoming buffers.
      enum ReceiverType
      {
        MULTICAST_RECEIVER,
        TCP_RECEIVER,
        RAWFILE_RECEIVER,
        PCAPFILE_RECEIVER,
        UNSPECIFIED_RECEIVER
      } receiverType_;

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

      /// @brief Iniitalize to defaults
      DecoderConfiguration()
        : head_(0)
        , reset_(false)
        , strict_(true)
        , echoType_(Codecs::DataSource::HEX)
        , echoMessage_(true)
        , echoField_(false)
        , pcapWordSize_(0)
        , headerType_(NO_HEADER)
        , headerMessageSizeBytes_(0)
        , headerBigEndian_(true)
        , headerPrefixCount_(0)
        , headerSuffixCount_(0)
        , assemblerType_(UNSPECIFIED_ASSEMBLER)
        , waitForCompleteMessage_(false)
        , receiverType_(UNSPECIFIED_RECEIVER)
        , multicastGroupIP_("224.1.2.133")
        , portNumber_(13014)
        , listenInterfaceIP_("0.0.0.0")
        , bufferSize_(1400)
        , bufferCount_(2)
      {
      }
    };
  }
}
#endif // DECODERCONFIGURATION_H
