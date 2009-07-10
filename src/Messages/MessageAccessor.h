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
    class MessageAccessor
    {
    public:
      /// Constant iterator for the collection
      typedef const MessageField * const_iterator;

      /// @brief Virtual destructor
      virtual ~MessageAccessor()
      {}

      /// @brief support indexing the set
      /// @param index 0 <= index < size()
      virtual const MessageField & operator[](size_t index)const = 0;

      /// @brief Is the specified field present in this set?
      /// @param name Identifies the field.
      /// @returns true if the field is present.  Returns false if the field is
      /// unknown or doesn't currently have a value in this set.
      virtual bool isPresent(const std::string & name)const = 0;


      /// @brief Get the value of the specified field.
      /// @param[in] name Identifies the desired field
      /// @param[out] value is the value that was found.
      /// @returns true if the field was found and has a value;
      virtual bool getField(const std::string &name, FieldCPtr & value) const  = 0;

      /// @brief Get the identity information for the specified field
      /// @param[in] name identifies the desired field
      /// @param[out] identity is the information for the field that was found
      /// @returns true if the field was found
      virtual bool getIdentity(const std::string &name, FieldIdentityCPtr & identity) const = 0;

      /// @brief support iterating through Fields in this MessageAccessor.
      virtual const_iterator begin() const = 0;

      /// @brief support iterating through Fields in this MessageAccessor.
      virtual const_iterator end() const = 0;

      /// @brief get the application type associated with
      /// this set of fields via typeref.
      virtual const std::string & getApplicationType()const = 0;

      /// @brief get the namespace for the application type
      virtual const std::string & getApplicationTypeNs()const = 0;
    };
  }
}
#endif // MESSAGEACCESSOR_H
