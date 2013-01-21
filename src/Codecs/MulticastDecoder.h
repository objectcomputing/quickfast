// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MULTICASTDECODER_H
#define MULTICASTDECODER_H
#include <Common/QuickFAST_Export.h>
#include "MulticastDecoder_fwd.h"
#include <Communication/MulticastReceiver.h>
#include <Codecs/MessagePerPacketAssembler.h>
#include <Codecs/NoHeaderAnalyzer.h>
#include <Codecs/TemplateRegistry_fwd.h>
#include <Messages/ValueMessageBuilder_fwd.h>

namespace QuickFAST{
  namespace Codecs {
    /// @brief Support decoding of FAST messages received via multicast.
    ///
    /// This class can handle one or more messages in a packet.  There can be a packet header
    /// and each message can have it's own header. [A header is a nonFAST preamble to the encoded message)
    /// Use the headerAnalyzer arguments to to the overloaded start() method if the incoming data contains headers.
    ///
    /// This class cannot handle messages split across packet boundararies.  These should neever appear in a
    /// multicast feed anyway because delivery of multicast packets is unreliable.
    /// @see MulticastReceiver

    class QuickFAST_Export MulticastDecoder
    {
    public:
      /// @brief construct given templates and multicast information
      ///
      /// @param templateRegistry the templates to use for decoding
      /// @param multicastGroupIP multicast address as a text string
      /// @param listenAddressIP listen address as a text string
      /// @param bindIP bind address as a text string
      /// @param portNumber port number
      MulticastDecoder(
        TemplateRegistryPtr templateRegistry,
        const std::string & multicastGroupIP,
        const std::string & listenAddressIP,
        const std::string & bindIP,
        unsigned short portNumber);

      /// @brief construct given templates, shared io_service, and multicast information
      ///
      /// @param templateRegistry the templates to use for decoding
      /// @param ioService an ioService to be shared with other objects
      /// @param multicastGroupIP multicast address as a text string
      /// @param listenAddressIP listen address as a text string
      /// @param bindIP bind address as a text string
      /// @param portNumber port number
      MulticastDecoder(
        TemplateRegistryPtr templateRegistry,
        boost::asio::io_service & ioService,
        const std::string & multicastGroupIP,
        const std::string & listenAddressIP,
        const std::string & bindIP,
        unsigned short portNumber);

      ~MulticastDecoder();

      /// @brief Enable some debugging/diagnostic information to be written to an ostream
      ///
      /// @param out the ostream to receive the data
      /// todo: use the Logger interface to eliminate the need for explicitly setting verbose.
      void setVerboseOutput(std::ostream & out);

      /// @brief Clear the verbose output that was enabled by setVerboseOutput()
      ///
      /// todo: use the Logger interface to eliminate the need for explicitly setting verbose.
      void disableVerboseOutput();

      /// @brief Enable/disable strict checking of conformance to the FAST standard
      ///
      /// If strict is false some conditions such as integer overflow or inefficient encoding
      /// will be ignored.  The default is true -- strict checking is enabled.
      /// @param strict true to enable; false to disable strict checking
      void setStrict(bool strict);

      /// @brief get the current status of the strict property.
      ///
      /// @returns true if strict checking is enabled.
      bool getStrict()const;

      /// @brief Get the id of the template driving the decoding
      template_id_t getTemplateId()const
      {
        return assembler_->decoder().getTemplateId();
      }

      /// @brief set an upper limit on the # of messages to be decoded
      ///
      /// must call before start
      /// @param messageLimit how many messages to decode (0 means "forever")
      void setLimit(size_t messageLimit)
      {
        messageLimit_ = messageLimit;
      }

      /// @brief How many messages have been decoded.
      ///
      /// @returns the number of messages that have been decoded.
      size_t messageCount() const;

      /// @brief immediate reset
      void reset();

      /// @brief Start the decoding process.  Returns immediately
      /// @param builder to accept the decoded output
      /// @param bufferSize should be >= the largest expected packet
      /// @param bufferCount is how many buffers to allocate (minimum 2 suggested)
      void start(Messages::ValueMessageBuilder & builder, size_t bufferSize=1500, size_t bufferCount=2);
      /// @brief Start the decoding process.  Returns immediately
      /// @param builder to accept the decoded output
      /// @param packetHeaderAnalyzer is an object to analyze the packet headers (if any)
      /// @param bufferSize should be >= the largest expected packet
      /// @param bufferCount is how many buffers to allocate (minimum 2 suggested)
      void start(Messages::ValueMessageBuilder & builder, HeaderAnalyzer & packetHeaderAnalyzer, size_t bufferSize=1500, size_t bufferCount=2);

      /// @brief Start the decoding process.  Returns immediately
      /// @param builder to accept the decoded output
      /// @param packetHeaderAnalyzer is an object to analyze the packet headers (if any)
      /// @param messageHeaderAnalyzer is an object to analyze the message headers (if any)
      /// @param bufferSize should be >= the largest expected packet
      /// @param bufferCount is how many buffers to allocate (minimum 2 suggested)
      void start(
        Messages::ValueMessageBuilder & builder,
        HeaderAnalyzer & packetHeaderAnalyzer,
        HeaderAnalyzer & messageHeaderAnalyzer,
        size_t bufferSize =1500,
        size_t bufferCount =2);

      /// @brief Run the event loop to accept incoming messages
      ///
      void run()
      {
        receiver_.run();
      }


      /// @brief Run the event loop to accept incoming messages in multiple threads
      void run(size_t threadCount, bool useThisThread)
      {
        receiver_.runThreads(threadCount,useThisThread);
      }


      /// @brief Stop the decoding process.
      ///
      /// Returns immediately, however decoding may continue until
      /// the decoder reaches a clean stopping point.  In particular
      /// the ValueMessageBuilder may receive additional messages after
      /// stop is called.
      ///
      /// ValueMessageBuilder::decodingStopped() will be called when
      /// the stop request is complete.
      void stop()
      {
        receiver_.stop();
      }


      /// @brief Join any threads created by the run method
      ///
      /// Should be called AFTER stop()
      void joinThreads()
      {
        receiver_.joinThreads();
      }

      /// @brief access the underlying multicast receiver.
      const Communication::MulticastReceiver & receiver()const
      {
        return receiver_;
      }

    private:
      Communication::MulticastReceiver receiver_;
      NoHeaderAnalyzer packetHeaderAnalyzer_;
      NoHeaderAnalyzer messageHeaderAnalyzer_;
      TemplateRegistryPtr templateRegistry_;
      Codecs::MessagePerPacketAssemblerPtr assembler_;
      Messages::ValueMessageBuilder * builder_;
      size_t messageLimit_;
      size_t byteCount_;
      size_t messageCount_;
      bool strict_;
      std::ostream * verboseOut_;
    };
  }
}
#endif // MULTICASTDECODER_H
