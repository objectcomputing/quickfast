// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef BUFFERDECODER_H
#define BUFFERDECODER_H
//#include <Common/QuickFAST_Export.h>
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
//#include <Codecs/TemplateRegistry_fwd.h>
#include <Codecs/BufferConsumer.h>

//#include <Codecs/XMLTemplateParser.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/DataSourceBuffer.h>
#include <Codecs/MessageConsumer.h>
#include <Messages/Message.h>

namespace QuickFAST{
  namespace Codecs {

    /// @brief Decode FAST messages from a buffer
    template<
      typename MessageType,
      typename MessageConsumerType,
      typename DataSourceType>
    class /*QuickFAST_Export*/ BufferDecoder : public BufferConsumer
    {
      explicit BufferDecoder(const BufferDecoder<MessageType, MessageConsumerType, DataSourceType> &);

    public:
      ///@brief declare a pointer to the [templated] consumer.
      typedef boost::shared_ptr<MessageConsumerType> ConsumerPtr;

      /// @brief construct given templates and multicast information
      /// @param templateRegistry the templates to use for decoding
      /// @param multicastAddressName multicast address as a text string
      /// @param listenAddressName listen address as a text string
      /// @param portNumber port number
      BufferDecoder(
        TemplateRegistryPtr templateRegistry,
        MessageConsumerPtr consumer)
      : decoder_(templateRegistry)
      , consumer_(consumer)
      , stopping_(false)
      , error_(false)
      , messageCount_(0)
      , messageLimit_(0)
      , bufferSize_(5000)
      , verboseDecode_(false)
      , verboseExecution_(false)
      {
      }

      ~BufferDecoder()
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

      /// @brief did an error occur during decoding?
      /// @param message receives an error message if this function returns true.
      /// @returns true if the decoding ended due to an error.
      bool hadError(std::string & message);

      ////////////////////////////
      // Implement BufferConsumer
      virtual bool consumeBuffer(const unsigned char * buffer, size_t bufferSize)
      {
        if(!stopping_)
        {
          ++messageCount_;
          if(consumer_->wantLog(MessageConsumer::LOG_VERBOSE))
          {
            std::stringstream message;
            message << "Received[" << messageCount_ << "]: " << bufferSize << " bytes";
            consumer_->logMessage(MessageConsumer::LOG_VERBOSE, message.str());
          }

          try
          {
            DataSourceType source(buffer, bufferSize);
            decoder_.reset();
            while(source.bytesAvailable() > 0 && !stopping_)
            {
              MessageType message(decoder_.getTemplateRegistry()->maxFieldCount());
              decoder_.decodeMessage(source, message);

              if(!consumer_->consumeMessage(message))
              {
                stopping_ = true;
                return false;
              }
            }
          }
          catch (const std::exception &ex)
          {
            error_ = true;
            errorMessage_ = ex.what();
            stopping_ = !consumer_->reportDecodingError(ex.what());
          }
          if(messageCount_ > messageLimit_ && messageLimit_ != 0)
          {
            stopping_ = true;
          }
        }
        return !stopping_;
      }

      virtual bool reportCommunicationError(const std::string & errorBuffer)
      {
        return consumer_->reportCommunicationError(errorBuffer);
      }
      virtual void receiverStarted()
      {
        // ok
      }
      virtual void receiverStopped()
      {
        // ok
      }

    private:

    private:
      Decoder decoder_;
      ConsumerPtr consumer_;
      bool stopping_;
      bool error_;
      std::string errorMessage_;
      size_t messageCount_;
      size_t messageLimit_;
      size_t bufferSize_;
      bool verboseDecode_;
      bool verboseExecution_;
    };
  }
}
#endif // BUFFERDECODER_H
