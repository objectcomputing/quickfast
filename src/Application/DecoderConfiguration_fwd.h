// Copyright (c) 2009, 2010, 2011, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef DECODERCONFIGURATION_FWD_H
#define DECODERCONFIGURATION_FWD_H

namespace QuickFAST{
  namespace Application{
    struct DecoderConfiguration;
    class DecoderConfigurationEnums
    {
    public:
      enum HeaderType{
        NO_HEADER,
        FIXED_HEADER,
        FAST_HEADER};

      /// @brief What type of assembler processes incoming buffers
      enum AssemblerType{
        MESSAGE_PER_PACKET_ASSEMBLER,
        STREAMING_ASSEMBLER,
        UNSPECIFIED_ASSEMBLER
      };
      /// @brief What type of receiver supplies incoming buffers.
      enum ReceiverType
      {
        MULTICAST_RECEIVER,
        TCP_RECEIVER,
        RAWFILE_RECEIVER,
        PCAPFILE_RECEIVER,
        ASYNCHRONOUS_FILE_RECEIVER,
        BUFFER_RECEIVER,
        UNSPECIFIED_RECEIVER
      };

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
