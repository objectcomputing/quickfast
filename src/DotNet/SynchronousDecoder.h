// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include "UnmanagedPtr.h"
#include "Decoder.h"

namespace QuickFASTDotNet{
  namespace Messages {
    ref class Message;
  }

  namespace Codecs{


    ///// @brief Delegate used by the Decoder class to callback to an entity waiting on a Message
    /////
    ///// This is used by the Decoder.Decode() method.  The caller caller calls Decode passing
    ///// a MessageReceivedDelegate object.  When a message arrives on the stream, the Decoder
    ///// class uses the MessageReceivedDelegate to signal the caller and return the newly arrived
    ///// message.
    //public delegate bool MessageReceivedDelegate(QuickFASTDotNet::Messages::Message^ message);



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
        void set(bool strict) { decoder_->setStrict(strict); }
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
        void set(bool reset) { resetOnMessage_ = reset; }
      }

      /// @brief The upper limit on the number of messages to be decoded
      ///
      property unsigned int MessageCountLimit
      {
        unsigned int get() { return messageCountLimit_; }
        void set(unsigned int limit) { messageCountLimit_ = limit; }
      }

      /// @brief The number of messages already decoded
      ///
      property unsigned int MessageCount
      {
        unsigned int get() { return messageCount_; }
      }

      /// @brief Run the decoding process
      ///
      /// Runs until the Stream reports end of data
      /// or the consumer returns false,
      /// or messageCount messages have been decoded.
      void Decode(MessageReceivedDelegate^ callback);

    private:
      System::IO::Stream^ stream_;
      bool endOfStream_;
      TemplateRegistry^ templateRegistry_;
      UnmanagedPtr<DotNetDataSource> dataSource_;
      UnmanagedPtr<QuickFAST::Codecs::Decoder> decoder_;
      unsigned int maxFieldCount_;
      bool resetOnMessage_;
      unsigned int messageCount_;
      unsigned int messageCountLimit_;
    };
  }
}
