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
//#include <Messages/FieldSet_fwd.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief Interface to support building a message during decoding.
    class MessageBuilder
    {
    public:

      /// @brief Virtual destructor
      virtual ~MessageBuilder(){};

      /// @brief clear current contents of the field set
      ///
      /// Optionally adjust the capacity.
      /// @param capacity is expected number of fields (0 is no change)
      virtual void clear(size_t capacity = 0)  = 0;

      /// @brief insure the MessageBuilder can hold the expected number of fields
      /// @param capacity is expected number of fields.
      virtual void reserve(size_t capacity)  = 0;

      /// @brief Get the count of fields in the set
      ///
      /// Group fields are counted individually.
      /// A Sequence is counted as one field.
      /// @returns the field count.
      virtual size_t size()const  = 0;

      /// @brief Add a field to the set.
      ///
      /// The FieldCPtr is copied, not the actual Field object.
      /// @param identity identifies this field
      /// @param value is the value to be assigned.
      virtual void addField(const FieldIdentityCPtr & identity, const FieldCPtr & value) = 0;

      /// @brief Get the identity information for the specified field
      /// @param[in] name identifies the desired field
      /// @param[out] identity is the information for the field that was found
      /// @returns true if the field was found
      virtual bool getIdentity(const std::string &name, FieldIdentityCPtr & identity) const = 0;

      /// @brief identify the application type associated with
      /// this set of fields via typeref.
      virtual void setApplicationType(const std::string & type, const std::string & ns) = 0;

      /// @brief get the application type associated with
      /// this set of fields via typeref.
      virtual const std::string & getApplicationType()const = 0;

      /// @brief get the namespace to qualify application type.
      virtual const std::string & getApplicationTypeNs()const = 0;

      /// @brief prepare to accept decoded sequence entries
      ///
      /// @param identity identifies the sequence
      /// @param applicationType is the data type for a sequence entry
      /// @param applicationTypeNamespace qualifies applicationTYpe
      /// @param size is the maximum number of fields to expect in each entry
      virtual void startSequence(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) = 0;

      /// @brief prepare to accept a single decoded sequence entry
      ///
      /// Builders may assume this call is nested within a startSequence/endSequence pair.
      ///
      /// @param applicationType is the data type for the entry
      /// @param applicationTypeNamespace qualifies applicationTYpe
      /// @param size is the maximum number of fields to expect in the entry
      /// @returns a MessageBuilder to accumulate fields for this entry.
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
      /// @param the nested Message builder returned by startSequenceEntry.
      virtual void endSequenceEntry(MessageBuilder & entry) = 0;

      /// @brief Complete the entire sequence.
      ///
      /// Builders may assume that all Sequence Entries will be closed via endSequenceEntry
      ///
      /// @param identity identifies the sequence
      virtual void endSequence( Messages::FieldIdentityCPtr identity) = 0;


      /// @brief Prepare to accept a decoded Group
      /// @param identity identifies the group
      /// @param applicationType is the data type for the group
      /// @param applicationTypeNamespace qualifies applicationTYpe
      /// @param size is the maximum number of fields to expect in the group
      /// @returns a MessageBuilder to accumulate fields for this group
      virtual MessageBuilder & startGroup(
        Messages::FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size)  = 0;

      /// @brief Complete the group
      ///
      /// @param identity identifies the group
      virtual void endGroup(
        Messages::FieldIdentityCPtr identity,
        MessageBuilder & entry) = 0;

    };
  }
}
#endif // MESSAGEBUILDER_H
