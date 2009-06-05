// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#include <DotNet/UnmanagedPtr.h>
#include <DotNet/BoostPtr.h>
#include <DotNet/Decoder.h>
#include "DataSourceBuffered.h"

#include "Messages/Message.h"
#include "Codecs/MessageConsumer.h"
#include "Codecs/SynchronousDecoder.h"
#include <iostream>

namespace QuickFASTDotNet{
  namespace Messages {
    ref class Message;
  }

  namespace Codecs{
#pragma unmanaged
    class MessageCounter : public QuickFAST::Codecs::MessageConsumer
    {
    public:
      MessageCounter(): messageCount_(0) {}
      virtual ~MessageCounter(){}

      ////////////////////////////
      // Implement MessageConsumer
      virtual bool consumeMessage(QuickFAST::Messages::DecodedFields & message)
      {
        messageCount_ += 1;
        return true;
      }
      virtual bool wantLog(unsigned short level) {return false;}
      virtual bool logMessage(unsigned short level, const std::string & logMessage){return true;}
      virtual bool reportDecodingError(const std::string & errorMessage)
      {
        //std::cerr << "Decoding error: " << errorMessage << std::endl;
        return false;
      }

      virtual bool reportCommunicationError(const std::string & errorMessage){
        //std::cerr << "Communication error: " << errorMessage << std::endl;
        return false;
      }

      virtual void decodingStopped() {}

      /// @brief get the count
      /// @returns the number of calls to consumeMessage()
      size_t getMesssageCount()const
      {
        return messageCount_;
      }
    private:
      size_t messageCount_;
    };
#pragma managed



    /// @brief Support Synchronous (blocking) decoding of a FAST data stream
    public ref class SynchronousDecoder
    {
    public:
      SynchronousDecoder(TemplateRegistry^ templateRegistry, System::IO::Stream^ instream);

      /// @brief Indicates whether the decoder is strict checking of conformance to the FAST standard
      ///
      /// If strict is false some conditions such as integer overflow or inefficient encoding
      /// will be ignored.
      property bool Strict
      {
        bool get() { return decoder_->getStrict(); }
        void set(bool strict) { decoder_->setStrict(strict); syncDecoder_->setStrict(strict); }
      }

      /// @brief Indicates whether the decoder is reset after each message when
      /// decoding more than one message.
      ///
      /// Some data streams reset the Xcoder context at the beginning of each message
      /// This is particulary true when the data stream uses a datagram-per-message
      /// approach (UDP and Unreliable Multicast).
      property bool ResetOnMessage
      {
        bool get() { return resetOnMessage_; }
        void set(bool reset) { resetOnMessage_ = reset; syncDecoder_->setResetOnMessage(reset);}
      }

      /// @brief The upper limit on the number of messages to be decoded
      ///
      property unsigned int MessageCountLimit
      {
        unsigned int get() { return messageCountLimit_; }
        void set(unsigned int limit) { messageCountLimit_ = limit; syncDecoder_->setLimit(limit);}
      }

      /// @brief The number of messages already decoded
      ///
      property unsigned int MessageCount
      {
        unsigned int get() { return messageCount_; }
      }

      /// @brief The number of milliseconds elapsed in the decoding process
      /// of the TestSyncDecode method (if enabled)
      property unsigned int DecodeTime
      {
        unsigned int get() { return decodeTime_; }
      }

      /// @brief Run the decoding process
      ///
      /// Runs until the Stream reports end of data
      /// or the consumer returns false,
      /// or messageCount messages have been decoded.
      void Decode(MessageReceivedDelegate^ callback);

      /// @brief Run a test of the decoding process using the sync decoder
      ///
      /// Runs until the Stream reports end of data
      /// or the consumer returns false,
      /// or messageCount messages have been decoded.
      void TestSyncDecode();

    private:
      System::IO::Stream^ stream_;
      TemplateRegistry^ templateRegistry_;
      UnmanagedPtr<QuickFASTDotNet::Codecs::DataSourceBuffered> dataSource_;
      UnmanagedPtr<QuickFAST::Codecs::Decoder> decoder_;
      UnmanagedPtr<QuickFAST::Codecs::SynchronousDecoder<QuickFAST::Messages::Message, QuickFAST::Codecs::MessageConsumer> > syncDecoder_;
      unsigned int maxFieldCount_;
      bool resetOnMessage_;
      unsigned int messageCount_;
      unsigned int messageCountLimit_;
      unsigned long decodeTime_;
    };
  }
}
