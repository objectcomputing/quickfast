// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include "PacketSequencingAssembler.h"
#include <Communication/Receiver.h>
#include <Communication/RecoveryFeed.h>
#include <Messages/ValueMessageBuilder.h>
#include <Codecs/Decoder.h>
#include <Common/Exceptions.h>

using namespace QuickFAST;
using namespace Codecs;

namespace
{
  // A linked buffer flag to identify source of buffer
  uint32 FROM_RECOVERY_QUEUE = 1;
}

PacketSequencingAssembler::PacketSequencingAssembler(
      TemplateRegistryPtr templateRegistry,
      HeaderAnalyzer & packetHeaderAnalyzer,
      HeaderAnalyzer & messageHeaderAnalyzer,
      Messages::ValueMessageBuilder & builder,
      size_t lookAheadCount,
      const Communication::RecoveryFeedPtr & recoveryFeed)
  : BasePacketAssembler(
      templateRegistry,
      packetHeaderAnalyzer,
      messageHeaderAnalyzer,
      builder)
  , lookAheadCount_(lookAheadCount)
  , lookAhead_(new Communication::LinkedBuffer *[lookAheadCount])
  , first_(true)
  , nextSequenceNumber_(0)
  , gapWait_(false)
  , gapEnd_(0)
  , recoveryFeed_(recoveryFeed)
  , receiver_(0)
{
  if(!packetHeaderAnalyzer.supportsSequenceNumber())
  {
    throw UsageError("Configuration error", "Arbitrage requires sequence number support from packet header analyzer.");
  }
  for(size_t nBuffer = 0; nBuffer < lookAheadCount; ++ nBuffer)
  {
    lookAhead_[nBuffer] = 0;
  }
}

PacketSequencingAssembler::~PacketSequencingAssembler()
{
}

bool
PacketSequencingAssembler::serviceQueue(Communication::Receiver & receiver)
{
  bool result = true;
  receiver_ = & receiver;
  bool more = true;
  while(more)
  {
    // More becomes true when *something* happens
    more = false;
    /////////////////////////////////////////////////////////////////////
    // Check to see if the next packet is already in the look-ahead array
    Communication::LinkedBuffer * buffer = lookAhead_[nextSequenceNumber_ % lookAheadCount_];
    if(buffer != 0)
    {
      lookAhead_[nextSequenceNumber_ % lookAheadCount_] = 0;
      processPacket(buffer);
      more = true;
    }

    if(!more)
    {
      ////////////////////////////////
      // Handle a buffer from receiver
      // Note the receiver merges the
      // A/B feeds into a single queue
      buffer = receiver.getBuffer(false);
      if(buffer != 0)
      {
        buffer->clearFlag(FROM_RECOVERY_QUEUE);
        capturePacket(buffer);
        more = true;
      }
    }

    if(!more && recoveryFeed_)
    {
      //////////////////////////////////////
      // Handle a buffer from recovery feed

      // recoveryIncoming_ is a local queue that does
      // not need thread synchronization (serviceQueue is protected)
      // It is filled with a single call to minimize mutex locking.
      if(recoveryIncoming_.isEmpty())
      {
        // pull all available packets into recoveryIncoming_
        recoveryFeed_->fetchBuffers(recoveryIncoming_);
      }
      buffer = recoveryIncoming_.pop();
      if(buffer != 0)
      {
        buffer->setFlag(FROM_RECOVERY_QUEUE);
        capturePacket(buffer);
        more = true;
      }
    }
    if(!more)
    {
      ////////////////////////////////////////
      // The next buffer is not available(yet)
      // If any deferred buffers are now within the
      // lookahead range, promote them to the lookahead array.
      more = promoteDeferred();
    }
    if(!more && (nextSequenceNumber_ < gapEnd_ || !deferredQueue_.isEmpty()))
    {
      /////////////////////////////////////////////////////////////////////////////////////////////
      // if the next sequence number is < end of the gap we are filling a previously discovered gap
      // if the deferred queue is not empty after any any available packets have been processed, we
      // have a new gap.
      // In either case, handle it.
      // Note this may wait until messages arrive on the recovery feed.
      handleGap();
      // and continue trying to process the next buffer
      more = true;
    }
  }
  // if we're completely up-to-date, return from serviceQueue
  receiver_ = 0;
  return result;
}


void
PacketSequencingAssembler::capturePacket(Communication::LinkedBuffer * buffer)
{
  sequence_t sequenceNumber = packetHeaderAnalyzer_.getSequenceNumber(buffer->get());
  if(first_)
  {
    first_ = false;
    nextSequenceNumber_ = sequenceNumber;
  }
  if(sequenceNumber == nextSequenceNumber_)
  {
    processPacket(buffer);
  }
  else if(sequenceNumber < nextSequenceNumber_)
  {
    releasePacket(buffer);
  }
  else if(sequenceNumber <  nextSequenceNumber_ + lookAheadCount_)
  {
    if(lookAhead_[sequenceNumber % lookAheadCount_] != 0)
    {
      releasePacket(buffer);
    }
    else
    {
      lookAhead_[sequenceNumber % lookAheadCount_] = buffer;
    }
  }
  else
  {
    /// buffer is beyond look-ahead
    addToDeferred(buffer, sequenceNumber);
  }
}

void
PacketSequencingAssembler::processPacket(Communication::LinkedBuffer * buffer)
{
  decodeBuffer(buffer->get(), buffer->used());
  releasePacket(buffer);
  ++nextSequenceNumber_;
}

void
PacketSequencingAssembler::releasePacket(Communication::LinkedBuffer * buffer)
{
  if(buffer->checkAnyFlag(FROM_RECOVERY_QUEUE) && recoveryFeed_)
  {
    recoveryFeed_->releaseBuffer(buffer);
  }
  else
  {
    receiver_->releaseBuffer(buffer);
  }
}

void
PacketSequencingAssembler::addToDeferred(Communication::LinkedBuffer * buffer, sequence_t sequenceNumber)
{
  // check for empty deferred queue
  Communication::LinkedBuffer * positionInDeferred = deferredQueue_.peek();
  if(positionInDeferred == 0)
  {
    deferredQueue_.push_front(buffer);
    return;
  }

  /// because it's likely that this goes at the end of the queue, check that before walking the queue.
  sequence_t deferredSequenceNumber = packetHeaderAnalyzer_.getSequenceNumber(deferredQueue_.peek_tail()->get());
  if(sequenceNumber == deferredSequenceNumber)
  {
    releasePacket(buffer);
    return;
  }
  if(sequenceNumber > deferredSequenceNumber)
  {
    deferredQueue_.push(buffer);
    return;
  }

  // the other "easy" case is the buffer comes before everything in the deferred queue
  deferredSequenceNumber = packetHeaderAnalyzer_.getSequenceNumber(positionInDeferred->get());
  if(sequenceNumber < deferredSequenceNumber)
  {
    deferredQueue_.push_front(buffer);
    return;
  }
  if(sequenceNumber == deferredSequenceNumber)
  {
    releasePacket(buffer);
    return;
  }

  // the new packet doesn't belong at either end.
  // we have to walk the queue to insert the packet in-place
  // This requires too-much-knowledge of the BufferQueue, but the alternatives are worse.
  //
  // loop invariant: sequenceNumber > deferredSequenceNumber
  while(positionInDeferred->link() != 0)
  {
    deferredSequenceNumber = packetHeaderAnalyzer_.getSequenceNumber(positionInDeferred->link()->get());
    if(sequenceNumber == deferredSequenceNumber)
    {
      releasePacket(buffer);
      return;
    }
    if(sequenceNumber < deferredSequenceNumber)
    {
      buffer->link(positionInDeferred->link());
      positionInDeferred->link(buffer);
      return;
    }
    positionInDeferred = positionInDeferred->link();
  }
  // reached the end of the queue
  // THIS SHOULD NOT HAPPEN 'cause we checked this case above
  deferredQueue_.push(buffer);
}

bool
PacketSequencingAssembler::promoteDeferred()
{
  bool result = false;
  Communication::LinkedBuffer * buffer = deferredQueue_.peek();
  while(buffer != 0 && packetHeaderAnalyzer_.getSequenceNumber(buffer->get()) < nextSequenceNumber_)
  {
    releasePacket(deferredQueue_.pop());
    buffer = deferredQueue_.peek();
  }

  while(buffer != 0 && packetHeaderAnalyzer_.getSequenceNumber(buffer->get()) < nextSequenceNumber_ + lookAheadCount_)
  {
    buffer = deferredQueue_.pop();
    sequence_t sequenceNumber = packetHeaderAnalyzer_.getSequenceNumber(buffer->get());
    if(lookAhead_[sequenceNumber % lookAheadCount_] == 0)
    {
      lookAhead_[sequenceNumber % lookAheadCount_] = buffer;
      result = true;
    }
    else
    {
      releasePacket(buffer);
    }
    buffer = deferredQueue_.peek();
  }
  return result;
}

void
PacketSequencingAssembler::handleGap()
{
  sequence_t newGapEnd = findGapEnd();
  // If this is a new gap
  if(nextSequenceNumber_ >= gapEnd_)
  {
    gapEnd_ = newGapEnd;
    gapWait_ = false;
    if(recoveryFeed_)
    {
      // Initiate the process of filling the gap.
      gapWait_ = recoveryFeed_->reportGap(nextSequenceNumber_, gapEnd_);
    }
  }
  else
  {
    if(newGapEnd < gapEnd_)
    {
      gapEnd_ = newGapEnd;
    }
    if(recoveryFeed_)
    {
      // this gives the recovery feed a chance to:
      //    retry the refill request if it has taken too long, or
      //    reduce the number of packets needed if the gap has gotten smaller, or
      //    say "never mind" this gap will never be filled.
      // The new gap will always be completely contained within the previous gap, so
      // the recovery feed can ignore this call if it is of a mind to.
      gapWait_ = recoveryFeed_->stillWaiting(nextSequenceNumber_, gapEnd_);
    }
  }

  if(!gapWait_)
  {
    builder_.reportGap(nextSequenceNumber_, gapEnd_);
    nextSequenceNumber_ = gapEnd_;
  }
  else if(recoveryFeed_)
  {
    // We're waiting for the recovery feed to fill the gap
    // delay until the recovery feed has data (or times out)
    // The timeout is there in the unlikely event that the missing packet(s)
    // magically arrive(s) on one of the primary (A/B) feeds.
    recoveryFeed_->waitGapFill(boost::posix_time::millisec(10));
  }
}

sequence_t
PacketSequencingAssembler::findGapEnd() const
{
  sequence_t gapEnd = nextSequenceNumber_ + 1;
  while( gapEnd < nextSequenceNumber_ + lookAheadCount_)
  {
    if(lookAhead_[gapEnd % lookAheadCount_] != 0)
    {
      return gapEnd;
    }
    ++gapEnd;
  }
  Communication::LinkedBuffer * deferredBuffer = deferredQueue_.peek();
  // assert deferredBuffer != 0
  return packetHeaderAnalyzer_.getSequenceNumber(deferredBuffer->get());
}
