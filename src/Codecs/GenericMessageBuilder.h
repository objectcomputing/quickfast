// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef GENERICMESSAGEBUILDER_H
#define GENERICMESSAGEBUILDER_H
#include <Common/QuickFAST_Export.h>
#include <Codecs/MessageConsumer.h>
#include <Messages/MessageBuilder.h>
#include <Messages/Message_fwd.h>
#include <Messages/FieldSet_fwd.h>
#include <Messages/Sequence_fwd.h>
#include <Messages/Group_fwd.h>
namespace QuickFAST{
  namespace Codecs{
    class GenericSequenceBuilder;
    class GenericGroupBuilder;
    class GenericMessageBuilder;

    /// @brief Build a sequence during decoding
    class QuickFAST_Export GenericSequenceBuilder : public Messages::MessageBuilder
    {
    public:
      /// @brief Construct with reference to a parent
      explicit GenericSequenceBuilder(MessageBuilder & parent);

      /// @brief Virtual destructor
      virtual ~GenericSequenceBuilder();

      void initialize(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);

      const Messages::SequencePtr & getSequence()const;

      void reset();

      //////////////////////////
      // Implement MessageBuilder
      virtual size_t size()const;
      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;
      virtual void addField(
        const Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & value);

      virtual Messages::MessageBuilder & startMessage(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual bool endMessage(Messages::MessageBuilder & messageBuilder);

      virtual Messages::MessageBuilder & startSequence(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);

      virtual void endSequence(
        Messages::FieldIdentityCPtr identity,
        MessageBuilder & sequenceBuilder);

      virtual Messages::MessageBuilder & startSequenceEntry(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual void endSequenceEntry(
        Messages::MessageBuilder & entry);

      virtual Messages::MessageBuilder & startGroup(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) ;
      virtual void endGroup(
        Messages::FieldIdentityCPtr identity,
        Messages::MessageBuilder & entry);

      ///////////////////
      // Implement Logger
      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);

    private:
      const Messages::FieldSetPtr & fieldSet()const;

    private:
      Messages::MessageBuilder & parent_;
      Messages::FieldSetPtr fieldSet_;
      Messages::SequencePtr sequence_;
      boost::scoped_ptr<GenericSequenceBuilder> sequenceBuilder_;
      boost::scoped_ptr<GenericGroupBuilder> groupBuilder_;
    };

    /// @brief Build a Group during decoding
    class QuickFAST_Export GenericGroupBuilder : public Messages::MessageBuilder
    {
    public:
      /// @brief Construct with reference to a parent
      GenericGroupBuilder(MessageBuilder & parent);

      /// @brief Virtual destructor
      virtual ~GenericGroupBuilder();

      void initialize(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);

      const Messages::GroupPtr & getGroup()const;
      void reset();

      //////////////////////////
      // Implement MessageBuilder

      virtual size_t size()const;
      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;
      virtual void addField(
        const Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & value);
      virtual Messages::MessageBuilder & startMessage(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual bool endMessage(Messages::MessageBuilder & messageBuilder);

      virtual Messages::MessageBuilder & startSequence(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual void endSequence( Messages::FieldIdentityCPtr identity,
        Messages::MessageBuilder & sequenceBuilder);

      virtual Messages::MessageBuilder & startSequenceEntry(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual void endSequenceEntry(Messages::MessageBuilder & entry);

      virtual Messages::MessageBuilder & startGroup(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) ;
      virtual void endGroup(
        Messages::FieldIdentityCPtr identity,
        Messages::MessageBuilder & entry);

      ///////////////////
      // Implement Logger
      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);

    private:
      const Messages::FieldSetPtr & fieldSet()const;
    private:
      Messages::MessageBuilder & parent_;
      Messages::FieldSetPtr fieldSet_;
      Messages::GroupPtr group_;

      boost::scoped_ptr<GenericSequenceBuilder> sequenceBuilder_;
      boost::scoped_ptr<GenericGroupBuilder> groupBuilder_;
    };

    /// @brief Build a generic message during decoding
    class QuickFAST_Export GenericMessageBuilder : public Messages::MessageBuilder
    {
    public:
      GenericMessageBuilder(MessageConsumer & consumer);

      /// @brief Virtual destructor
      virtual ~GenericMessageBuilder();

      //////////////////////////
      // Implement MessageBuilder

      virtual size_t size()const;
      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;
      virtual void addField(
        const Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & value);

      virtual Messages::MessageBuilder & startMessage(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual bool endMessage(Messages::MessageBuilder & messageBuilder);

      virtual Messages::MessageBuilder & startSequence(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual void endSequence(
        Messages::FieldIdentityCPtr identity,
        Messages::MessageBuilder & sequenceBuilder);

      virtual Messages::MessageBuilder & startSequenceEntry(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual void endSequenceEntry(Messages::MessageBuilder & entry);

      virtual Messages::MessageBuilder & startGroup(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) ;
      virtual void endGroup(
        Messages::FieldIdentityCPtr identity,
        Messages::MessageBuilder & entry);

      ///////////////////
      // Implement Logger
      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);

    private:
      const Messages::MessagePtr & message()const;
    private:
      MessageConsumer & consumer_;
      Messages::MessagePtr message_;
      GenericSequenceBuilder sequenceBuilder_;
      GenericGroupBuilder groupBuilder_;
    };
  }
}
#endif // GENERICMESSAGEBUILDER_H
