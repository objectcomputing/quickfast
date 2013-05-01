// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGEPERPACKETASSEMBLER_H
#define MESSAGEPERPACKETASSEMBLER_H

#include "MessagePerPacketAssembler_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Codecs/BasePacketAssembler.h>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Service a Receiver's Queue when expecting packet boundaries to match message boundaries (UDP or Multicast)
    /// with (or without) block headers.
    class QuickFAST_Export MessagePerPacketAssembler
      : public BasePacketAssembler
    {
    public:
      /// @brief Constuct the Assembler
      /// @param templateRegistry defines the decoding instructions for the decoder
      /// @param packetHeaderAnalyzer analyzes the header of each packet (if any)
      /// @param messageHeaderAnalyzer analyzes the header of each message (if any)
      /// @param builder receives the data from the decoder.
      MessagePerPacketAssembler(
          TemplateRegistryPtr templateRegistry,
          HeaderAnalyzer & packetHeaderAnalyzer,
          HeaderAnalyzer & messageHeaderAnalyzer,
          Messages::ValueMessageBuilder & builder);

      virtual ~MessagePerPacketAssembler();

      ///////////////////////////////////////
      // Implement Remaining Assembler method
      virtual bool serviceQueue(Communication::Receiver & receiver);

    private:
      MessagePerPacketAssembler & operator = (const MessagePerPacketAssembler &);
      MessagePerPacketAssembler(const MessagePerPacketAssembler &);
      MessagePerPacketAssembler();
    };
  }
}
#endif // MESSAGEPERPACKETASSEMBLER_H
