// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef SINGLEMESSAGECONSUMER_H
#define SINGLEMESSAGECONSUMER_H
#include "MessageConsumer.h"
#include <Common/QuickFAST_Export.h>
#include <Messages/Message.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief interface to be implemented by a consumer of decoded messages.
    class QuickFAST_Export SingleMessageConsumer : public MessageConsumer
    {
    public:
      SingleMessageConsumer();
      virtual ~SingleMessageConsumer();
      /// @brief access the decoded message
      /// @returns the decoded message
      Messages::Message & message();

      ////////////////////////////
      // Implement MessageConsumer
      virtual bool consumeMessage(Messages::Message & message);
      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);
      virtual void decodingStarted();
      virtual void decodingStopped();
    private:
      Messages::Message message_;
    };
  }
}
#endif /* SINGLEMESSAGECONSUMER_H */
