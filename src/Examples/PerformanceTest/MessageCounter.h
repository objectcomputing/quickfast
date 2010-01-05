// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGECOUNTER_H
#define MESSAGECOUNTER_H
#include <Codecs/MessageConsumer.h>
#include <Messages/MessageBuilder.h>
//#include <Messages/Field_fwd.h>
#include <Messages/FieldIdentity.h>
namespace QuickFAST{
  namespace Examples{

    /// @brief A MessageConsumer that simply counts the number of
    /// messages decoded.
    class MessageCounter : public Codecs::MessageConsumer
    {
    public:
      MessageCounter();
      virtual ~MessageCounter();

      ////////////////////////////
      // Implement MessageConsumer

      virtual bool consumeMessage(Messages::Message & message);
      virtual void decodingStopped();

      ///////////////////
      // Implement Logger

      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);

      /// @brief get the count
      /// @returns the number of calls to consumeMessage()
      size_t getMesssageCount()const
      {
        return messageCount_;
      }
    private:
      size_t messageCount_;
    };
  }
}
#endif /* MESSAGECOUNTER_H */
