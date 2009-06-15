// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include "MulticastReceiverHandle.h"
#include <Codecs/MulticastReceiver.h>

namespace QuickFAST{
  namespace Codecs {
    class MulticastReceiverHandle_i
    {
    public:
      MulticastReceiverHandle_i(
        const std::string & multicastAddressName,
        const std::string & listenAddressName,
        unsigned short portNumber)
        :ptr_(new MulticastReceiver(
                  multicastAddressName,
                  listenAddressName,
                  portNumber))
      {
      }

      MulticastReceiverPtr ptr_;
    };
  }
}


using namespace QuickFAST;
using namespace Codecs;

MulticastReceiverHandle::MulticastReceiverHandle(
    const std::string & multicastAddressName,
    const std::string & listenAddressName,
    unsigned short portNumber)
  : pImpl_(new MulticastReceiverHandle_i(
            multicastAddressName,
            listenAddressName,
            portNumber))
{
}

MulticastReceiverHandle::~MulticastReceiverHandle()
{
  delete pImpl_;
}


size_t
MulticastReceiverHandle::packetCount() const
{
  return pImpl_->ptr_->packetCount();
}

void
MulticastReceiverHandle::start(
  BufferConsumerPtr  bufferConsumer,
  size_t bufferSize,
  size_t bufferCount)
{
  pImpl_->ptr_->start(bufferConsumer, bufferSize, bufferCount);
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
