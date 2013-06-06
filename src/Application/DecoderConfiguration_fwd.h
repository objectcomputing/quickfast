// Copyright (c) 2009, 2010, 2011, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DECODERCONFIGURATION_FWD_H
#define DECODERCONFIGURATION_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

namespace QuickFAST{
  namespace Application{
    struct DecoderConfiguration;
    /// @brief Enumerated values used to specifiy configuration options
    class DecoderConfigurationEnums
    {
    public:
      /// @brief What type of header is present on message or packet
      enum HeaderType{
        NO_HEADER,    /// No header
        FIXED_HEADER, /// Fixed size header
        FAST_HEADER   /// FAST encoded header
      };

      /// @brief What type of assembler processes incoming buffers
      enum AssemblerType{
        MESSAGE_PER_PACKET_ASSEMBLER, /// Message boundaries on packet boundaries.
        STREAMING_ASSEMBLER,          /// Transport layer doesn't know about message boundaries.
        UNSPECIFIED_ASSEMBLER         /// Assembler has not yet been specified.
      };
      /// @brief What type of receiver supplies incoming buffers.
      enum ReceiverType
      {
        MULTICAST_RECEIVER,           /// Multicast: unreliable packets
        TCP_RECEIVER,                 /// TCP/IP: streaming
        RAWFILE_RECEIVER,             /// File containing FAST encoded records
        BUFFERED_RAWFILE_RECEIVER,    /// File containing FAST encoded records read entirely into memory.
        PCAPFILE_RECEIVER,            /// File captured from network in PCAP format
        ASYNCHRONOUS_FILE_RECEIVER,   /// File read using asynchronous I/O (not in core QuickFAST)
        BUFFER_RECEIVER,              /// Decode from in-memory buffer.
        UNSPECIFIED_RECEIVER          /// Receiver has not yet been specified.
      };

      /// @brief How should incoming data be echoed.
      enum EchoType
      {
          RAW,    /// Binary data as read
          HEX,    /// Two hex characters per byte
          NONE    /// No data echoed (only field/message boundaries)
      };

    };
  }
}
#endif // DECODERCONFIGURATION_H
