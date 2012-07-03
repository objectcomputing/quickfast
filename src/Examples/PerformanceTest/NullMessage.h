// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef NULLMESSAGE_H
#define NULLMESSAGE_H
#include <Messages/ValueMessageBuilder.h>
#include <Common/Types.h>
namespace QuickFAST{
  namespace Examples{
    /// @brief Internal representation of a Null Message
    /// @todo: consider typedef FieldSet Message
    class NullMessage : public Messages::ValueMessageBuilder
    {
      NullMessage();
    public:
      /// @brief Construct an empty NullMessage
      NullMessage(size_t expectedNumberOfFields);

      /// @brief Copy construct NullMessage
      NullMessage(const NullMessage & rhs);

//      virtual void addField(const Messages::FieldIdentity & identity, const Messages::FieldCPtr & value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int64 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint64 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int32 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint32 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int16 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint16 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int8 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uchar value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const Decimal& value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const unsigned char * value, size_t length);

      virtual Messages::ValueMessageBuilder & startSequence(
        const Messages::FieldIdentity & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t fieldCount,
        const Messages::FieldIdentity & lengthIdentity,
        size_t length);

      virtual void endSequence(
        const Messages::FieldIdentity & identity,
        Messages::ValueMessageBuilder & sequenceBuilder );

      virtual Messages::ValueMessageBuilder & startSequenceEntry(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);

      virtual void endSequenceEntry(
        Messages::ValueMessageBuilder & entry
        );

      virtual Messages::ValueMessageBuilder & startGroup(
        const Messages::FieldIdentity & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual void endGroup(
        const Messages::FieldIdentity & identity,
        Messages::ValueMessageBuilder & groupBuilder);

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
