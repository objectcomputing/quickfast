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
#include <Messages/ValueMessageBuilder.h>
#include <Messages/MessageField.h>
#include <Messages/Logger.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief Interface to support building a message during decoding.
    class QuickFAST_Export MessageBuilder : public ValueMessageBuilder
    {
    public:

      /// @brief Virtual destructor
      virtual ~MessageBuilder(){};


      /// @brief Add a field to the set.
      ///
      /// The FieldCPtr is copied, not the actual Field object.
      /// @param identity identifies this field
      /// @param value is the value to be assigned.
      virtual void addField(const FieldIdentityCPtr & identity, const FieldCPtr & value) = 0;

      ////////////////////////////////
      // Implement ValueMessageBuilder
      virtual void addValue(const FieldIdentityCPtr & identity, Field::FieldType type, const int64 value);
      virtual void addValue(const FieldIdentityCPtr & identity, Field::FieldType type, const uint64 value);
      virtual void addValue(const FieldIdentityCPtr & identity, Field::FieldType type, const int32 value);
      virtual void addValue(const FieldIdentityCPtr & identity, Field::FieldType type, const uint32 value);
      virtual void addValue(const FieldIdentityCPtr & identity, Field::FieldType type, const int16 value);
      virtual void addValue(const FieldIdentityCPtr & identity, Field::FieldType type, const uint16 value);
      virtual void addValue(const FieldIdentityCPtr & identity, Field::FieldType type, const int8 value);
      virtual void addValue(const FieldIdentityCPtr & identity, Field::FieldType type, const uchar value);
      virtual void addValue(const FieldIdentityCPtr & identity, Field::FieldType type, const Decimal& value);
      virtual void addValue(const FieldIdentityCPtr & identity, Field::FieldType type, const unsigned char * value, size_t length);

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

      /// @brief prepare to accept decoded sequence entries
      ///
      /// @param identity identifies the sequence
      /// @param applicationType is the data type for a sequence entry
      /// @param applicationTypeNamespace qualifies applicationTYpe
      /// @param fieldCount is the maximum number of fields to expect in each entry
      /// @param lengthIdentity is the identity of the length field
      /// @param length is a count of how many entries will be in the sequence
      virtual MessageBuilder & startSequence(
        FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t fieldCount,
        FieldIdentityCPtr lengthIdentity,
        size_t length) = 0;


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

      /// @brief Prepare to accept a decoded Group
      /// @param identity identifies the group
      /// @param applicationType is the data type for the group
      /// @param applicationTypeNamespace qualifies applicationTYpe
      /// @param size is the maximum number of fields to expect in the group
      /// @returns a MessageBuilder to accumulate fields for this group (*this is ok)
      virtual MessageBuilder & startGroup(
        FieldIdentityCPtr identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size)  = 0;

    };
  }
}
#endif // MESSAGEBUILDER_H
