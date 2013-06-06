// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef PACKETSEQUENCINGASSEMBLER_H
#define PACKETSEQUENCINGASSEMBLER_H

#include "PacketSequencingAssembler_fwd.h"
#include <Codecs/BasePacketAssembler.h>
#include <Communication/BufferQueue.h>
#include <Communication/RecoveryFeed_fwd.h>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Service a Receiver's Queue when expecting packet boundaries to match message boundaries (UDP or Multicast)
    /// with (or without) block headers.
    class QuickFAST_Export PacketSequencingAssembler
      : public BasePacketAssembler
    {
    public:
      /// @brief Constuct the Assembler
      /// @param templateRegistry defines the decoding instructions for the decoder
      /// @param packetHeaderAnalyzer analyzes the header of each packet (if any)
      /// @param messageHeaderAnalyzer analyzes the header of each message (if any)
      /// @param builder receives the data from the decoder.
      /// @param lookAheadCount how many packets to look ahead before deciding there is a gap.
      /// @param recoveryFeed an object to recover the packets that should have been in a gap.
      PacketSequencingAssembler(
          TemplateRegistryPtr templateRegistry,
          HeaderAnalyzer & packetHeaderAnalyzer,
          HeaderAnalyzer & messageHeaderAnalyzer,
          Messages::ValueMessageBuilder & builder,
          size_t lookAheadCount,
          const Communication::RecoveryFeedPtr & recoveryFeed);

      virtual ~PacketSequencingAssembler();

      ///////////////////////////////////////
      // Implement Remaining Assembler method
      virtual bool serviceQueue(Communication::Receiver & receiver);

    private:
      /// @brief Initial processing of incoming packet from any source.
      void capturePacket(Communication::LinkedBuffer * buffer);
      /// @brief Ready to decode a packet
      void processPacket(Communication::LinkedBuffer * buffer);
      /// @brief Packet is no longer needed.  Return it to from whence it came.
      void releasePacket(Communication::LinkedBuffer * buffer);
      /// @brief Packet is beyond the look-ahead array.   Hang on to it for later.
      void addToDeferred(Communication::LinkedBuffer * buffer, sequence_t sequenceNumber);
      /// @brief Promote deferred packets to the look-ahead array if possible.
      /// @return true any were promoted.
      bool promoteDeferred();

      /// @brief find the sequence number of the first available packet after a gap.
      sequence_t findGapEnd()const;

      /// @brief Report a gap to the recovery feed.
      /// Either wait for recovery information to arrive, or skip over the gap.
      void handleGap();

    private:
      PacketSequencingAssembler & operator = (const PacketSequencingAssembler &);
      PacketSequencingAssembler(const PacketSequencingAssembler &);
      PacketSequencingAssembler();

    private:
      size_t lookAheadCount_;
      boost::scoped_array<Communication::LinkedBuffer *> lookAhead_;
      bool first_;
      sequence_t nextSequenceNumber_;
      bool gapWait_;
      sequence_t gapEnd_;

      Communication::BufferQueue deferredQueue_;
      Communication::BufferQueue recoveryIncoming_;
      Communication::RecoveryFeedPtr recoveryFeed_;

      Communication::Receiver * receiver_;

    };

  }
}
#endif // PACKETSEQUENCINGASSEMBLER_H
