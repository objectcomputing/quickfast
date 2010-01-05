// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGEPERFORMANCE_H
#define MESSAGEPERFORMANCE_H
#include <Codecs/MessageConsumer.h>
#include <Messages/Field_fwd.h>
#include <Messages/FieldIdentity.h>
#include <Messages/ValueMessageBuilder.h>
namespace QuickFAST{
  namespace Examples{
    class PerformanceBuilder: public Messages::ValueMessageBuilder
    {
    public:
      PerformanceBuilder();
      virtual ~PerformanceBuilder();
      size_t fieldCount() const
      {
        return fieldCount_;
      }
      size_t msgCount() const
      {
        return msgCount_;
      }
      size_t sequenceCount() const
      {
        return sequenceCount_;
      }

      size_t sequenceEntryCount() const
      {
        return sequenceEntryCount_;
      }

      size_t groupCount() const
      {
        return groupCount_;
      }

      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;
      virtual void addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const int64 value);
      virtual void addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const uint64 value);
      virtual void addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const int32 value);
      virtual void addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const uint32 value);
      virtual void addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const int16 value);
      virtual void addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const uint16 value);
      virtual void addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const int8 value);
      virtual void addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const uchar value);
      virtual void addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const Decimal& value);
      virtual void addValue(const Messages::FieldIdentityCPtr & identity, ValueType::Type type, const unsigned char * value, size_t length);
      virtual ValueMessageBuilder & startMessage(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual bool endMessage(ValueMessageBuilder & messageBuilder);
      virtual bool ignoreMessage(ValueMessageBuilder & messageBuilder);
      virtual ValueMessageBuilder & startSequence(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t fieldCount,
        Messages::FieldIdentityCPtr lengthIdentity,
        size_t length);
      virtual void endSequence(
        Messages::FieldIdentityCPtr identity,
        ValueMessageBuilder & sequenceBuilder);
      virtual ValueMessageBuilder & startSequenceEntry(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) ;
      virtual void endSequenceEntry(ValueMessageBuilder & entry);
      virtual ValueMessageBuilder & startGroup(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) ;
      virtual void endGroup(
        Messages::FieldIdentityCPtr identity,
        ValueMessageBuilder & groupBuilder);

      /////////////////////////////
      // Implement Logger interface
      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);

    private:
      size_t fieldCount_;
      size_t msgCount_;
      size_t sequenceCount_;
      size_t sequenceEntryCount_;
      size_t groupCount_;
    };

    /// @brief A message consumer that attempts to produce a human readable version
    /// of a message that has been decoded by QuickFAST.
    class MessagePerformance : public Codecs::MessageConsumer
    {
    public:
      /// @brief Construct given a ostream to which to write the interpreted results.
      MessagePerformance(size_t recordLimit, size_t recordDuplicate);
      virtual ~MessagePerformance();

      ////////////////////////////
      // Implement MessageConsumer
      virtual bool consumeMessage(Messages::Message & message);
      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);
      virtual void decodingStarted();
      virtual void decodingStopped();

      /// @brief How many messages did we process.
      ///
      /// @returns the number of messaeges handled
      size_t getMessageCount() const
      {
        return recordCount_;
      }
    private:
      void retrieveMessage(const Messages::Message & message);
      void retrieveSequence(
        const Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & field);
      void retrieveGroup(const Messages::FieldCPtr & field);

      void retrieveFieldValue(const Messages::FieldCPtr & field);

    private:
      size_t recordLimit_;
      size_t recordDuplicate_;
      size_t recordCount_;
    };
  }
}
#endif /* MESSAGEPERFORMANCE_H */
