// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include "MulticastReceiverHandle.h"
#include <Communication/MulticastReceiver.h>

namespace QuickFAST{
  namespace Communication {
    class MulticastReceiverHandle_i
    {
    public:
      MulticastReceiverHandle_i(
        const std::string & multicastGroupIP,
        const std::string & listenInterfaceIP,
        const std::string & bindIP,
        unsigned short portNumber)
        :ptr_(new MulticastReceiver(
                  multicastGroupIP,
                  listenInterfaceIP,
                  bindIP,
                  portNumber))
      {
      }

      MulticastReceiverPtr ptr_;
    };
  }
}


using namespace QuickFAST;
using namespace Communication;

MulticastReceiverHandle::MulticastReceiverHandle(
    const std::string & multicastGroupIP,
    const std::string & listenInterfaceIP,
    const std::string & bindIP,
    unsigned short portNumber)
  : pImpl_(new MulticastReceiverHandle_i(
            multicastGroupIP,
            listenInterfaceIP,
            bindIP,
            portNumber))
{
}

MulticastReceiverHandle::~MulticastReceiverHandle()
{
  delete pImpl_;
}

size_t
MulticastReceiverHandle::bytesReadable() const
{
  return pImpl_->ptr_->bytesReadable();
}

size_t
MulticastReceiverHandle::noBufferAvailable() const
{
  return pImpl_->ptr_->noBufferAvailable();
}

size_t
MulticastReceiverHandle::packetsReceived() const
{
  return pImpl_->ptr_->packetsReceived();
}

size_t
MulticastReceiverHandle::packetsQueued() const
{
  return pImpl_->ptr_->packetsQueued();
}

size_t
MulticastReceiverHandle::batchesProcessed() const
{
  return pImpl_->ptr_->batchesProcessed();
}

size_t
MulticastReceiverHandle::packetsProcessed() const
{
  return pImpl_->ptr_->packetsProcessed();
}

size_t
MulticastReceiverHandle::packetsWithErrors() const
{
  return pImpl_->ptr_->packetsWithErrors();
}

size_t
MulticastReceiverHandle::emptyPackets() const
{
  return pImpl_->ptr_->emptyPackets();
}

size_t
MulticastReceiverHandle::bytesReceived() const
{
  return pImpl_->ptr_->bytesReceived();
}

size_t
MulticastReceiverHandle::bytesProcessed() const
{
  return pImpl_->ptr_->bytesProcessed();
}

size_t
MulticastReceiverHandle::largestPacket() const
{
  return pImpl_->ptr_->largestPacket();
}


void
MulticastReceiverHandle::start(
  Communication::Assembler & assembler,
  size_t bufferSize,
  size_t bufferCount)
{
  pImpl_->ptr_->start(assembler, bufferSize, bufferCount);
}

void
MulticastReceiverHandle::run(unsigned short additionalThreads, bool useThisThread)
{
  pImpl_->ptr_->runThreads(additionalThreads, useThisThread);
}

void
MulticastReceiverHandle::joinThreads()
{
  pImpl_->ptr_->joinThreads();
}

void
MulticastReceiverHandle::stop()
{
  pImpl_->ptr_->stop();
}
