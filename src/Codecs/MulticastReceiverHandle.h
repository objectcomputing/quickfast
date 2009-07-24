// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTRECEIVERHANDLE_H
#define MULTICASTRECEIVERHANDLE_H
#include <Common/QuickFAST_Export.h>
#include <Codecs/BufferConsumer_fwd.h>

namespace QuickFAST{
  namespace Codecs {

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
      /// @param multicastAddressName multicast address as a text string
      /// @param listenAddressName listen address as a text string
      /// @param portNumber port number
      MulticastReceiverHandle(
        const std::string & multicastAddressName,
        const std::string & listenAddressName,
        unsigned short portNumber);
      ~MulticastReceiverHandle();
      ///////////////////////////////////////
      // expose the MulticastReceiver methods

      /// @brief How many packetd have been decoded.
      /// @returns the number of messages that have been decoded.
      size_t packetCount() const;

      /// @brief Approximately how many bytes are waiting to be read from the underlying socket.
      ///
      /// Not that packets which have already arrived, but have not been decoded will not be included.
      size_t bytesReadable() const;

      /// @brief Start accepting packets.  Returns immediately
      /// @param bufferConsumer accepts and processes the filled buffers
      /// @param bufferSize determines the maximum size of an incoming packet
      /// @param bufferCount is how many input buffers to use
      void start(
        BufferConsumer & bufferConsumer,
        size_t bufferSize = 1600,
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
