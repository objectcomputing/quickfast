// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H
#include "MessageBuilder_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Messages/MessageField.h>
#include <Messages/Logger.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief Interface to support building a message during decoding.
    class MessageBuilder : public Logger
    {
    public:

      /// @brief Virtual destructor
      virtual ~MessageBuilder(){};

      /// @brief get the application type associated with
      /// this set of fields via typeref.
      virtual const std::string & getApplicationType()const = 0;

      /// @brief get the namespace for the application type
      virtual const std::string & getApplicationTypeNs()const = 0;

      /// @brief Add a field to the set.
      ///
      /// The FieldCPtr is copied, not the actual Field object.
      /// @param identity identifies this field
      /// @param value is the value to be assigned.
      virtual void addField(const FieldIdentityCPtr & identity, const FieldCPtr & value) = 0;

      /// @brief prepare to accept an entire message
      ///
      /// @param applicationType is the data type for the message
      /// @param applicationTypeNamespace qualifies applicationTYpe
      /// @param size is the maximum number of fields to expect in the message
      /// @returns a message builder that will accept the fields in the message
      ///        (which may be *this)
      virtual MessageBuilder & startMessage(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) = 0;

      /// @brief Finish a message.  Process the result.
      ///
      /// @param messageBuilder is the builder provided by startMessage()
      /// @returns true if decoding should continue
      virtual bool endMessage(MessageBuilder & messageBuilder) = 0;

      /// @brief Finish a message.  Ignore the result.
      ///
      /// @param messageBuilder is the builder provided by startMessage()
      /// @returns true if decoding should continue
      virtual bool ignoreMessage(MessageBuilder & messageBuilder) = 0;

      /// @brief prepare to accept decoded sequence entries
      ///
      /// @param identity identifies the sequence
      /// @param applicationType is the data type for a sequence entry
      /// @param applicationTypeNamespace qualifies applicationTYpe
      /// @param size is the maximum number of fields to expect in each entry
      virtual MessageBuilder & startSequence(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) = 0;

      /// @brief Complete the entire sequence.
      ///
      /// Builders may assume that all Sequence Entries will be closed via endSequenceEntry
      ///
      /// @param identity identifies the sequence
      /// @param sequenceBuilder is the builder provided by startSequence
      virtual void endSequence(
        Messages::FieldIdentityCPtr identity,
        MessageBuilder & sequenceBuilder) = 0;

      /// @brief prepare to accept a single decoded sequence entry
      ///
      /// Builders may assume this call is nested within a startSequence/endSequence pair.
      ///
      /// @param applicationType is the data type for the entry
      /// @param applicationTypeNamespace qualifies applicationTYpe
      /// @param size is the maximum number of fields to expect in the entry
      /// @returns a MessageBuilder to accumulate fields for this entry (*this is ok).
      virtual MessageBuilder & startSequenceEntry(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size)  = 0;

      /// @brief Complete the current sequence entry.
      ///
      /// This method is called on the <b>containing</b> MessageBuilder
      ///
      /// Builders may assume this call follows a startSequenceEntry call within
      //  a startSequence/endSequence pair.
      ///
      /// @param entry the nested Message builder returned by startSequenceEntry.
      virtual void endSequenceEntry(MessageBuilder & entry) = 0;

      /// @brief Prepare to accept a decoded Group
      /// @param identity identifies the group
      /// @param applicationType is the data type for the group
      /// @param applicationTypeNamespace qualifies applicationTYpe
      /// @param size is the maximum number of fields to expect in the group
      /// @returns a MessageBuilder to accumulate fields for this group (*this is ok)
      virtual MessageBuilder & startGroup(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size)  = 0;

      /// @brief Complete the group
      ///
      /// @param identity identifies the group
      /// @param groupBuilder is the message builder returned by startGroup
      virtual void endGroup(
        Messages::FieldIdentityCPtr identity,
        MessageBuilder & groupBuilder) = 0;

    };
  }
}
#endif // MESSAGEBUILDER_H
