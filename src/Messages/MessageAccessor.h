// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGEACCESSOR_H
#define MESSAGEACCESSOR_H
#include <Messages/MessageField_fwd.h>
#include <Messages/FieldIdentity_fwd.h>
#include <Messages/Field.h>

namespace QuickFAST
{
  namespace Messages
  {
    /// @brief Internal representation of a set of fields to be encoded or decoded.
    class QuickFAST_Export MessageAccessor
    {
    public:
      MessageAccessor();

      /// @brief Virtual destructor
      virtual ~MessageAccessor();

      /// @brief pick template to use for encoding next message
      ///
      /// @param[out] tid is the template chosen.
      /// @returns false if no more data to encode.
//      virtual bool pickTemplate(template_id_t & tid) = 0;

      /// backwards compatibility only.  Derived classes should implement "isPresent(FieldIdentity)"
      /// @deprecated
      virtual bool isPresent(const std::string & name)const;

      /// @brief Is the specified field present in this set?
      /// @param name Identifies the field.
      /// @returns true if the field is present.  Returns false if the field is
      /// unknown or doesn't currently have a value in this set.
      virtual bool isPresent(const FieldIdentity & identity)const;

// When we're ready to switch over
    private:
      /// backwards compatibility only.  Derived classes should implement "get[datatype]((FieldIdentity..."
      /// @brief Get the value of the specified field.
      /// @param[in] name Identifies the desired field
      /// @param[out] value is the value that was found.
      /// @returns true if the field was found and has a value;
      /// @deprecated
      virtual bool getField(const std::string & name, FieldCPtr & value) const;
      virtual bool getField(const FieldIdentity & identity, FieldCPtr & value) const;
    public:
      /// @brief Get a field from the application record.
      ///
      /// @param identity identifies this field
      /// @param type is the type of data requested
      /// @param value is the value to be returned.
      virtual bool getUnsignedInteger(const FieldIdentity & identity, ValueType::Type type, uint64 & value)const;// transition = 0;

      /// @brief Get a field from the application record.
      ///
      /// @param identity identifies this field
      /// @param type is the type of data requested
      /// @param value is the value to be returned.
      virtual bool getSignedInteger(const FieldIdentity & identity, ValueType::Type type, int64 & value)const;// transition = 0;

      /// @brief Get a field from the application record.
      ///
      /// @param identity identifies this field
      /// @param type is the type of data requested
      /// @param value is the value to be returned.
      virtual bool getDecimal(const FieldIdentity & identity, ValueType::Type type, Decimal & value)const;// transition = 0;

      /// @brief Get a field from the application record.
      ///
      /// @param identity identifies this field
      /// @param type is the type of data requested
      /// @param value is the value to be returned.
      virtual bool getString(const FieldIdentity & identity, ValueType::Type type, const StringBuffer *& value)const;// transition = 0;

      virtual bool getGroup(const FieldIdentity & identity, const MessageAccessor *& groupAccessor)const;// transition = 0;
      virtual void endGroup(const FieldIdentity & identity, const MessageAccessor * groupAccessor)const;// transition = 0;

      virtual bool getSequenceLength(const FieldIdentity & identity, size_t & length)const;// transition = 0;
      virtual bool getSequenceEntry(const FieldIdentity & identity, size_t index, const MessageAccessor *& entryAccessor)const;// transition = 0;
      virtual void endSequenceEntry(const FieldIdentity & identity, size_t index, const MessageAccessor * entryAccessor)const;// transition = 0;
      virtual void endSequence(const FieldIdentity & identity)const;// transition = 0;

    private:
      /// @brief Get the identity information for the specified field
      /// @param[in] name identifies the desired field
      /// @param[out] identity is the information for the field that was found
      /// @returns true if the field was found
      virtual bool getIdentity(const std::string &name, FieldIdentityCPtr & identity) const = 0;
    public:
      /// @brief get the application type associated with
      /// this set of fields via typeref.
      virtual const std::string & getApplicationType()const = 0;

      /// @brief get the namespace for the application type
      virtual const std::string & getApplicationTypeNs()const = 0;

    private:
      mutable bool recursive_; /// temporary during migration
    };
  }
}
#endif // MESSAGEACCESSOR_H
