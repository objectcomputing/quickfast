// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

namespace QuickFASTDotNet{
  namespace Messages {
    ref class Message;
  }

  namespace Codecs{

    ref class TemplateRegistry;

    /// @brief Encode incoming FAST messages.
    ///
    /// Create an instance of the Encoder providing a registry of the templates
    /// to be used to encode the message, then call encodeMessage to encode
    /// each message to a output stream.
    public ref class Encoder
    {
    public:

      /// @brief Construct with a TemplateRegistry containing all templates to be used and a
      /// an output stream to recieve the encoded data.
      /// @param templateRegistry A registry containing all templates to be used to encode messages.
      /// @param outStream An output stream where the encoded data is to be written to.
      Encoder(TemplateRegistry^ templateRegistry, System::IO::Stream^ outStream);

      /// @brief Encode the next message.
      /// @param[in] templateId identifies the template to use for encoding.
      /// @param[in] message containing the fields to be encoded.
      void Encode(unsigned int templateId, Messages::Message^ message);

    private:
      TemplateRegistry^ templateRegistry_;
      System::IO::Stream^ outStream_;
    };
  }
}
