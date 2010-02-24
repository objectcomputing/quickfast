// Copyright (c) 2009, Object Computing, Inc.
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
      size_t head_;                   // -head
      bool reset_;                    // -r
      bool strict_;                   // -strict

      std::string templateFileName_;  // -t
      std::string fastFileName_;      // -file
      std::string verboseFileName_;   // -vo
      std::string pcapFileName_;      // -pcap
      std::string echoFileName_;      // -e
      Codecs::DataSource::EchoType echoType_;  // -ehex; -eraw; -enone
      bool echoMessage_;              // -em
      bool echoField_;                // -ef

      size_t pcapWordSize_;           // 0 means native; 32 or 64

      enum HeaderType{
        NO_HEADER,                    // -hnone
        FIXED_HEADER,                 // -hfixed n
        FAST_HEADER}                  // -hfast
        headerType_;

      size_t headerMessageSizeBytes_; // -hfixed *N*
      bool headerBigEndian_;          // -hbig [no]
      size_t headerPrefixCount_;      // -hprefix N
      size_t headerSuffixCount_;      // -hsuffix N

      enum AssemblerType{
        MESSAGE_PER_PACKET_ASSEMBLER, // -streaming
        STREAMING_ASSEMBLER,          // -datagram
        UNSPECIFIED_ASSEMBLER
      } assemblerType_;

      bool waitForCompleteMessage_;   // -streaming [no]wait

      enum ReceiverType
      {
        MULTICAST_RECEIVER,           // -multicast
        TCP_RECEIVER,                 // -tcp
        RAWFILE_RECEIVER,             // -file
        PCAPFILE_RECEIVER,            // -pcap
        UNSPECIFIED_RECEIVER
      } receiverType_;

      std::string multicastGroupIP_;    // -multicast IP:port
      std::string listenInterfaceIP_;   // -mlisten IP
      unsigned short portNumber_;       // -multicast ip:PORT
      std::string portName_;            // -tcp host:PORT
      std::string hostName_;            // -tcp HOST:port

      size_t bufferSize_;               // -buffersize SIZE
      size_t bufferCount_;              // -buffers COUNT

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
        , listenInterfaceIP_("0.0.0.0")
        , portNumber_(13014)
        , bufferSize_(1400)
        , bufferCount_(2)
      {
      }
    };
  }
}
#endif // DECODERCONFIGURATION_H
