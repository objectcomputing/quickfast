// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTDECODER_H
#define MULTICASTDECODER_H
#include <Common/QuickFAST_Export.h>
#include "MulticastDecoder_fwd.h"
#include <Codecs/MulticastReceiver.h>
#include <Codecs/Decoder.h>

#include <Codecs/TemplateRegistry_fwd.h>
#include <Messages/MessageBuilder_fwd.h>

namespace QuickFAST{
  namespace Codecs {
    /// @brief Support decoding of FAST messages received via multicast.
    ///
    /// This class assumes that incoming FAST messages are packed
    /// one or more per packet with no additional information (no headers, etc.)
    /// in the packet.
    ///
    /// If this is not the case, see MulticastReceiver.
    class QuickFAST_Export MulticastDecoder
    {
    public:
      /// @brief construct given templates and multicast information
      ///
      /// @param templateRegistry the templates to use for decoding
      /// @param multicastGroupIP multicast address as a text string
      /// @param listenAddressIP listen address as a text string
      /// @param portNumber port number
      MulticastDecoder(
        TemplateRegistryPtr templateRegistry,
        const std::string & multicastGroupIP,
        const std::string & listenAddressIP,
        unsigned short portNumber);

      /// @brief construct given templates, shared io_service, and multicast information
      ///
      /// @param templateRegistry the templates to use for decoding
      /// @param ioService an ioService to be shared with other objects
      /// @param multicastGroupIP multicast address as a text string
      /// @param listenAddressIP listen address as a text string
      /// @param portNumber port number
      MulticastDecoder(
        TemplateRegistryPtr templateRegistry,
        boost::asio::io_service & ioService,
        const std::string & multicastGroupIP,
        const std::string & listenAddressIP,
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

      /// @brief set an upper limit on the # of messages to be decoded
      ///
      /// @param messageLimit how many messages to decode (0 means "forever")
      void setLimit(size_t messageLimit);

      /// @brief How many messages have been decoded.
      ///
      /// @returns the number of messages that have been decoded.
      size_t messageCount() const;

      /// @brief immediate reset
      void reset();

      /// @brief Start the decoding process.  Returns immediately
      void start(Messages::MessageBuilder & builder, size_t bufferSize=1600, size_t bufferCount=2);

      /// @brief Run the event loop to accept incoming messages
      ///
      /// Todo: provide access to other variations of run
      void run();

      /// @brief Stop the decoding process.
      ///
      /// Returns immediately, however decoding may continue until
      /// the decoder reaches a clean stopping point.  In particular
      /// the MessageBuilder may receive additional messages after
      /// stop is called.
      ///
      /// MessageBuilder::decodingStopped() will be called when
      /// the stop request is complete.
      void stop();

    private:
      MulticastReceiver receiver_;

      BufferConsumerPtr bufferConsumer_;
      Decoder decoder_;
      Messages::MessageBuilder * builder_;
//      bool verbose_;
      size_t messageLimit_;
      size_t messageCount_;
    };
  }
}
#endif // MULTICASTDECODER_H
