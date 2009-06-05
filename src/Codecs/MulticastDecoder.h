// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTDECODER_H
#define MULTICASTDECODER_H
//#include <Common/QuickFAST_Export.h>
#include "MulticastDecoder_fwd.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/TemplateRegistry_fwd.h>
#include <Codecs/MessageConsumer_fwd.h>
#include <Common/AsioService.h>

#include <Codecs/XMLTemplateParser.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/Decoder.h>
#include <Codecs/DataSourceBuffer.h>
#include <Codecs/MessageConsumer.h>
#include <Messages/Message.h>

namespace QuickFAST{
  namespace Codecs {

    /// @brief Support decoding of FAST messages received via multicast.
    template<
      typename MessageType,
      typename MessageConsumerType,
      typename DataSourceType>
    class /*QuickFAST_Export*/ MulticastDecoderT : public AsioService
    {
      typedef boost::scoped_array<unsigned char> Buffer;
    public:
      ///@brief declare a pointer to the [templated] consumer.
      typedef boost::shared_ptr<MessageConsumerType> ConsumerPtr;
      /// @brief construct given templates and multicast information
      /// @param templateRegistry the templates to use for decoding
      /// @param multicastAddressName multicast address as a text string
      /// @param listenAddressName listen address as a text string
      /// @param portNumber port number
      MulticastDecoderT(
        TemplateRegistryPtr templateRegistry,
        const std::string & multicastAddressName,
        const std::string & listenAddressName,
        unsigned short portNumber)
      : decoder_(templateRegistry)
      , stopping_(false)
      , error_(false)
      , messageCount_(0)
      , messageLimit_(0)
      , bufferSize_(5000)
      , verboseDecode_(false)
      , verboseExecution_(false)
      , listenAddress_(boost::asio::ip::address::from_string(listenAddressName))
      , multicastAddress_(boost::asio::ip::address::from_string(multicastAddressName))
      , endpoint_(listenAddress_, portNumber)
      , socket_(ioService_)
      {
      }

      /// @brief construct given templates, shared io_service, and multicast information
      /// @param templateRegistry the templates to use for decoding
      /// @param ioService an ioService to be shared with other objects
      /// @param multicastAddressName multicast address as a text string
      /// @param listenAddressName listen address as a text string
      /// @param portNumber port number
      MulticastDecoderT(
        TemplateRegistryPtr templateRegistry,
        boost::asio::io_service & ioService,
        const std::string & multicastAddressName,
        const std::string & listenAddressName,
        unsigned short portNumber)
      : AsioService(ioService)
      , decoder_(templateRegistry)
      , messageCount_(0)
      , messageLimit_(0)
      , bufferSize_(5000)
      , verboseDecode_(false)
      , verboseExecution_(false)
      , listenAddress_(boost::asio::ip::address::from_string(listenAddressName))
      , multicastAddress_(boost::asio::ip::address::from_string(multicastAddressName))
      , endpoint_(listenAddress_, portNumber)
      , socket_(ioService_)
      {
      }

      ~MulticastDecoderT()
      {
      }

      /// @brief Enable some debugging/diagnostic information to be written to an ostream
      /// @param out the ostream to receive the data
      void setVerboseOutput(std::ostream & out)
      {
        decoder_.setVerboseOutput(out);
      }


      /// @brief Clear the verbose output that was enabled by setVerboseOutput()
      void disableVerboseOutput()
      {
        decoder_.disableVerboseOutput();
      }


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

      /// @brief the largest expected multicast packet
      void setBufferSize(size_t bufferSize)
      {
        bufferSize_ = bufferSize;
      }

      /// @brief enables noisy decoding at the message level
      void setVerboseDecode(bool verbose)
      {
        verboseDecode_ = verbose;
      }

      /// @brief enables noisy decoding at the execution progress level
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

      /// @brief immediate reset
      void reset()
      {
        decoder_.reset();
      }

      /// @brief Start the decoding process.  Returns immediately
      void start(ConsumerPtr consumer)
      {
        consumer_ = consumer;

        buffer1_.reset(new unsigned char[bufferSize_]);
        buffer2_.reset(new unsigned char[bufferSize_]);

        socket_.open(endpoint_.protocol());
        socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket_.bind(endpoint_);

        // Join the multicast group.
        socket_.set_option(
          boost::asio::ip::multicast::join_group(multicastAddress_));
        startReceive(&buffer1_, &buffer2_);
      }


      /// @brief Stop the decoding process.
      ///
      /// Returns immediately, however decoding may continue until
      /// the decoder reaches a clean stopping point.  In particular
      /// the MessageConsumer may receive additional messages after
      /// stop is called.
      ///
      /// MessageConsumer::decodingStopped() will be called when
      /// the stop request is complete.
      void stop()
      {
        stopping_ = true;
        // attempt to cancel any requests in progress.
        socket_.cancel();
      }

      /// @brief did an error occur during decoding?
      /// @param message receives an error message if this function returns true.
      /// @returns true if the decoding ended due to an error.
      bool hadError(std::string & message);

    private:
      void handleReceive(
        const boost::system::error_code& error,
        Buffer * buffer,
        size_t bytesReceived,
        Buffer * altBuffer)
      {
        if(stopping_)
        {
          return;
        }
        if (!error)
        {
          // accept data into the other buffer while we process this buffer
          startReceive(altBuffer, buffer);
          ++messageCount_;
          if(consumer_->wantLog(MessageConsumer::LOG_VERBOSE))
          {
            std::stringstream message;
            message << "Received[" << messageCount_ << "]: " << bytesReceived << " bytes";
            consumer_->logMessage(MessageConsumer::LOG_VERBOSE, message.str());
          }
          try
          {
            DataSourceType source(buffer->get(), bytesReceived);
            decoder_.reset();
            while(source.bytesAvailable() > 0 && !stopping_)
            {
              MessageType message(decoder_.getTemplateRegistry()->maxFieldCount());
              decoder_.decodeMessage(source, message);

              if(!consumer_->consumeMessage(message))
              {
                if(consumer_->wantLog(MessageConsumer::LOG_INFO))
                {
                  consumer_->logMessage(MessageConsumer::LOG_INFO, "Consumer requested early termination.");
                }
                stopping_ = true;
                socket_.cancel();
                return;
              }
            }
          }
          catch (const std::exception &ex)
          {
            if(!consumer_->reportDecodingError(ex.what()))
            {
              error_ = true;
              errorMessage_ =
              stopping_ = true;
              socket_.cancel();
            }
          }
          if(messageCount_ > messageLimit_ && messageLimit_ != 0)
          {
            stopping_ = true;
            socket_.cancel();
          }
        }
        else
        {
          if(!consumer_->reportCommunicationError(error.message()))
          {
            error_ = true;
            errorMessage_ = error.message();
            stopping_ = true;
            socket_.cancel();
          }
        }
      }

      void startReceive(Buffer * buffer, Buffer * altBuffer)
      {
        socket_.async_receive_from(
          boost::asio::buffer(buffer->get(), bufferSize_),
          senderEndpoint_,
          strand_.wrap(
            boost::bind(&MulticastDecoderT::handleReceive,
              this,
              boost::asio::placeholders::error,
              buffer,
              boost::asio::placeholders::bytes_transferred,
              altBuffer)
              )
            );
      }

    private:
      Decoder decoder_;
      bool stopping_;
      bool error_;
      std::string errorMessage_;
      size_t messageCount_;
      size_t messageLimit_;
      size_t bufferSize_;
      bool verboseDecode_;
      bool verboseExecution_;
      boost::asio::ip::address listenAddress_;
      boost::asio::ip::address multicastAddress_;
      boost::asio::ip::udp::endpoint endpoint_;
      boost::asio::ip::udp::endpoint senderEndpoint_;
      boost::asio::ip::udp::socket socket_;

      Buffer buffer1_;
      Buffer buffer2_;
      ConsumerPtr consumer_;
    };

    ///@brief Instantiate the template for the most common case
    /// This provides the same functionality as the previous, nontemplatized, version of MulticastDecoder
    typedef MulticastDecoderT<
      Messages::Message, Codecs::MessageConsumer> MulticastDecoder;
  }
}
#endif // MULTICASTDECODER_H
