// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MULTICASTRECEIVERHANDLE_H
#define MULTICASTRECEIVERHANDLE_H
#include <Common/QuickFAST_Export.h>
#include <Communication/Assembler_fwd.h>

namespace QuickFAST{
  namespace Communication {

    class MulticastReceiverHandle_i;

    /// @brief Expose the interface to a MulticastReceiver without exposing the implementation
    ///
    /// This insulates higher level code from knowing any of the implementation details
    /// of the multicast Receiver.  In particular including boost::asio
    /// in managed code leads to great trauma and knashing of teeth.
    class QuickFAST_Export MulticastReceiverHandle
    {
    public:
      /// @brief construct a multicast receiver given multicast information and a consumer
      /// @param multicastGroupIP multicast address as a text string
      /// @param listenInterfaceIP listen address as a text string
      /// @param bindIP the IP to be used to bind the socket.
      /// @param portNumber port number
      MulticastReceiverHandle(
        const std::string & multicastGroupIP,
        const std::string & listenInterfaceIP,
        const std::string & bindIP,
        unsigned short portNumber);
      ~MulticastReceiverHandle();
      ///////////////////////////////////////
      // expose the MulticastReceiver methods

      /// @brief Approximately how many bytes are waiting to be decoded
      size_t bytesReadable() const;

      /// @brief how many times were all buffers busy?
      /// @returns the number of times no buffers were available to receive packets.
      size_t noBufferAvailable() const;

      /// @brief How many packets have been received
      /// @returns the number of packets that have been received
      size_t packetsReceived() const;

      /// @brief How many packets have been queued for processing
      size_t  packetsQueued() const;

      /// @brief How many batches from the queue have been processed
      size_t batchesProcessed() const;

      /// @brief How many packets have been processed
      /// @returns the number of packets that have been processed.
      size_t packetsProcessed() const;

      /// @brief How many received packets had errors
      /// @returns the number of packets that have encountered communication errors
      size_t packetsWithErrors() const;

      /// @brief How many received packets were empty
      /// @returns the number of packets that were empty
      size_t emptyPackets() const;

      /// @brief How many bytes have been received
      /// @returns the number of bytes that have been received
      size_t bytesReceived() const;

      /// @brief How many bytes have been processed
      /// @returns the number of bytes that have been processed
      size_t bytesProcessed() const;

      /// @brief What was the largest incoming packet
      /// @returns the number of bytes in the largest packet
      size_t largestPacket() const;

      /// @brief Start accepting packets.  Returns immediately
      /// @param assembler accepts and processes the filled buffers
      /// @param bufferSize determines the maximum size of an incoming packet
      /// @param bufferCount is how many input buffers to use
      void start(
        Communication::Assembler & assembler,
        size_t bufferSize = 1500,
        size_t bufferCount = 2
        );

      /// @brief service the multicast receiver event loop
      ///
      /// Call start() before calling this method.
      ///
      /// If useThisThread is false, returns "immediately" however incoming
      ///  messages may arrive before this method returns.  Be prepared.
      ///  Also if useThisThread is false, the application should call stop()
      ///  then joinThreads(), but before exiting.
      ///
      /// If useThisThread is true, does not return until receiving is stopped.
      ///
      /// @see joinThreads();
      void run(unsigned short additionalThreads, bool useThisThread);

    /// @brief wait for the threads started by run() when useThisThread is false
    ///
    /// You should probably call stop() first.
    /// @see startThreads();
      void joinThreads();

      /// @brief Stop accepting packets
      ///
      /// Returns immediately, however decoding may continue until
      /// the decoder reaches a clean stopping point.  In particular
      /// the MessageConsumer may receive additional messages after
      /// stop is called.
      ///
      /// MessageConsumer::decodingStopped() will be called when
      /// the stop request is complete.
      void stop();

    private:
      MulticastReceiverHandle_i * pImpl_;
    };
  }
}
#endif // MULTICASTRECEIVERHANDLE_H
