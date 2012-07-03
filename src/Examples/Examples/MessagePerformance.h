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
    /// @brief Message Builder to be used in performance testing. Captures statistics about the test.
    class PerformanceBuilder: public Messages::ValueMessageBuilder
    {
    public:
      PerformanceBuilder();
      virtual ~PerformanceBuilder();
      /// @brief Access statistic
      size_t fieldCount() const
      {
        return fieldCount_;
      }
      /// @brief Access statistic
      size_t msgCount() const
      {
        return msgCount_;
      }
      /// @brief Access statistic
      size_t sequenceCount() const
      {
        return sequenceCount_;
      }
      /// @brief Access statistic
      size_t sequenceEntryCount() const
      {
        return sequenceEntryCount_;
      }
      /// @brief Access statistic
      size_t groupCount() const
      {
        return groupCount_;
      }

      ///////////////////////////////////////////
      // Implement ValueMessageBuilder interface
      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;
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
      virtual ValueMessageBuilder & startMessage(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual bool endMessage(Messages::ValueMessageBuilder & messageBuilder);
      virtual bool ignoreMessage(Messages::ValueMessageBuilder & messageBuilder);
      virtual Messages::ValueMessageBuilder & startSequence(
        const Messages::FieldIdentity & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t fieldCount,
        const Messages::FieldIdentity & lengthIdentity,
        size_t length);
      virtual void endSequence(
        const Messages::FieldIdentity & identity,
        Messages::ValueMessageBuilder & sequenceBuilder);
      virtual Messages::ValueMessageBuilder & startSequenceEntry(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) ;
      virtual void endSequenceEntry(Messages::ValueMessageBuilder & entry);
      virtual Messages::ValueMessageBuilder & startGroup(
        const Messages::FieldIdentity & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) ;
      virtual void endGroup(
        const Messages::FieldIdentity & identity,
        Messages::ValueMessageBuilder & groupBuilder);

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
#if 0
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
        const Messages::FieldIdentity & identity,
        const Messages::FieldCPtr & field);
      void retrieveGroup(const Messages::FieldCPtr & field);

      void retrieveFieldValue(const Messages::FieldCPtr & field);

    private:
      size_t recordLimit_;
      size_t recordDuplicate_;
      size_t recordCount_;
    };
#endif
  }
}
#endif /* MESSAGEPERFORMANCE_H */
