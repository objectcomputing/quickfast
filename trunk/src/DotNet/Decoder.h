// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include "UnmanagedPtr.h"

namespace QuickFASTDotNet{
  namespace Messages {
    ref class Message;
  }

  namespace Codecs{

    ref class TemplateRegistry;

    class DotNetDataSource: public QuickFAST::Codecs::DataSource
    {
    public:
      DotNetDataSource(System::IO::Stream^ inStream):inStream_(inStream){}

      virtual bool readByte(QuickFAST::uchar & byte)
      {
        byte = static_cast<QuickFAST::uchar>(inStream_->ReadByte());

        return static_cast<int>(byte) != -1;
      }

    private:
      gcroot<System::IO::Stream^> inStream_;
    };




    /// @brief Delegate used by the Decoder class to callback to an entity waiting on a Message
    ///
    /// This is used by the Decoder.Decode() method.  The caller caller calls Decode passing
    /// a MessageReceivedDelegate object.  When a message arrives on the stream, the Decoder
    /// class uses the MessageReceivedDelegate to signal the caller and return the newly arrived
    /// message.
    public delegate void MessageReceivedDelegate(QuickFASTDotNet::Messages::Message^ message);



    /// @brief Decode incoming FAST messages.
    ///
    /// Create an instance of the Decoder providing a registry of the templates
    /// to be used to decode the message and a stream representing the data source,
    /// then call decodeMessage to decode each message from a data source stream.
    public ref class Decoder
    {
    public:
      Decoder(TemplateRegistry^ templateRegistry, System::IO::Stream^ instream);

      /// @brief returns a reference to the data source stream.
      ///
      /// This is a reference to the object supplied in the constructor of this class.
      property System::IO::Stream^ Stream
      {
        System::IO::Stream^ get() { return stream_; }
      }

      /// @brief Indicates whether on not the end of the stream has been reached.
      ///
      /// The value of this property can be checked after a call to Decode has been made.
      /// It indicates whether or not underlying stream indicated there may be additional
      /// data to read at the end of the previous data read.
      property bool IsEndOfStream
      {
        bool get() { return endOfStream_; }
      }

      /// @brief Decodes the next message from the stream.
      ///
      Messages::Message^ Decode();

      /// @brief  Asychronously decodes the next message.
      ///
      void Decode(MessageReceivedDelegate^ callback);

    private:
      System::IO::Stream^ stream_;
      bool endOfStream_;
      TemplateRegistry^ templateRegistry_;
      UnmanagedPtr<DotNetDataSource> dataSource_;
      UnmanagedPtr<QuickFAST::Codecs::Decoder> decoder_;
    };
  }
}
