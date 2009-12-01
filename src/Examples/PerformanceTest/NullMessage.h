// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef NULLMESSAGE_H
#define NULLMESSAGE_H
#include <Messages/MessageBuilder.h>
#include <Common/Types.h>
namespace QuickFAST{
  namespace Examples{
    /// @brief Internal representation of a Null Message
    /// @todo: consider typedef FieldSet Message
    class NullMessage : public Messages::MessageBuilder
    {
      NullMessage();
    public:
      /// @brief Construct an empty NullMessage
      NullMessage(size_t expectedNumberOfFields);

      /// @brief Copy construct NullMessage
      NullMessage(const NullMessage & rhs);

      virtual void addField(const Messages::FieldIdentityCPtr & identity, const Messages::FieldCPtr & value);

      virtual Messages::MessageBuilder & startSequence(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t fieldCount,
        Messages::FieldIdentityCPtr lengthIdentity,
        size_t length);

      virtual void endSequence(
        Messages::FieldIdentityCPtr identity,
        Messages::MessageBuilder & sequenceBuilder );

      virtual Messages::MessageBuilder & startSequenceEntry(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);

      virtual void endSequenceEntry(
        Messages::MessageBuilder & entry
        );

      virtual Messages::MessageBuilder & startGroup(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual void endGroup(
        Messages::FieldIdentityCPtr identity,
        Messages::MessageBuilder & groupBuilder);

    private:
      int size_;
      std::string applicationType_;
      std::string applicationTypeNamespace_;

    };
  }
}
#include <Codecs/MessageConsumer.h>
namespace QuickFAST{
  namespace Examples{
    ///@brief A message consumer that simply counts the messages
    class NullMessageConsumer : public Codecs::MessageConsumer
    {
    public:
      NullMessageConsumer();
      virtual ~NullMessageConsumer();
      //////////////////////////////////////
      // Implement MessageConsumer Interface
      virtual bool consumeMessage(Messages::Message  & message);
      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);
      virtual void decodingStopped();

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
#endif // NULLMESSAGE_H
