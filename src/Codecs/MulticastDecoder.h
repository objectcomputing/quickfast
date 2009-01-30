// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTDECODER_H
#define MULTICASTDECODER_H
#include <Common/QuickFAST_Export.h>
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/TemplateRegistry_fwd.h>

#include <boost/asio.hpp>
#include <Codecs/MessageConsumer_fwd.h>

namespace QuickFAST{
  namespace Codecs {
    class QuickFAST_Export MulticastDecoder
    {
      typedef boost::scoped_array<unsigned char> Buffer;
    public:
      MulticastDecoder(
        TemplateRegistryPtr templateRegistry,
        const std::string & multicastAddressName,
        const std::string & listendAddressName,
        unsigned short portNumber);
      ~MulticastDecoder();

      /// @brief Enable some debugging/diagnostic information to be written to an ostream
      /// @param out the ostream to receive the data
      void setVerboseOutput(std::ostream & out);

      /// @brief Clear the verbose output that was enabled by setVerboseOutput()
      void disableVerboseOutput();

      /// @brief Enable/disable strict checking of conformance to the FAST standard
      ///
      /// If strict is false some conditions such as integer overflow or inefficient encoding
      /// will be ignored.  The default is true -- strict checking is enabled.
      /// @param strict true to enable; false to disable strict checking
      void setStrict(bool strict)
      {
        decoder_.setStrict(strict);
      }

      /// @brief get the current status of the strict property.
      /// @returns true if strict checking is enabled.
      bool getStrict()const
      {
        return decoder_.getStrict();
      }


      /// @brief set an upper limit on the # of messages to be decoded
      /// @param messageLimit how many messages to decode (0 means "forever")
      void setLimit(size_t messageLimit)
      {
        messageLimit_ = messageLimit;
      }

      void setBufferSize(size_t bufferSize)
      {
        bufferSize_ = bufferSize;
      }

      void setVerboseDecode(bool verbose)
      {
        verboseDecode_ = verbose;
      }

      void setVerboseExecution(bool verbose)
      {
        verboseExecution_ = verbose;
      }


      /// @brief How many messages have been decoded.
      /// @returns the number of messages that have been decoded.
      size_t messageCount() const
      {
        return messageCount_;
      }

      void start(MessageConsumerPtr consumer);

      void run();

    private:
      void handleReceive(
        const boost::system::error_code& error,
        Buffer * data,
        size_t bytesReceived);

      void startReceive(Buffer * data);

    private:
      Decoder decoder_;
      size_t messageCount_;
      size_t messageLimit_;
      size_t bufferSize_;
      bool verboseDecode_;
      bool verboseExecution_;
      boost::asio::io_service ioService_;
      boost::asio::strand strand_;
      boost::asio::ip::address listenAddress_;
      boost::asio::ip::address multicastAddress_;
      boost::asio::ip::udp::endpoint endpoint_;
      boost::asio::ip::udp::endpoint senderEndpoint_;
      boost::asio::ip::udp::socket socket_;

      Buffer data_;
      Buffer data2_;
      MessageConsumerPtr consumer_;
    };
  }
}
#endif // MULTICASTDECODER_H
