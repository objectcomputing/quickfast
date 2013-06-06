// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDSET_H
#define FIELDSET_H
#include "FieldSet_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Messages/MessageAccessor.h>
#include <Messages/MessageField.h>

namespace QuickFAST{
  namespace Messages{
    /// @brief Internal representation of a set of fields to be encoded or decoded.
    class QuickFAST_Export FieldSet
      : public MessageAccessor
    {
      FieldSet();
      FieldSet(const FieldSet&);
      FieldSet& operator=(const FieldSet&);

    public:
      /// Constant iterator for the collection
      typedef const MessageField * const_iterator;
      /// @brief Construct an empty FieldSet
      explicit FieldSet(size_t res);

      /// @brief Virtual destructor
      virtual ~FieldSet();

      /// @brief clear current contents of the field set
      ///
      /// Optionally adjust the capacity.
      /// @param capacity is expected number of fields (0 is no change)
      void clear(size_t capacity = 0);

      /// @brief insure the FieldSet can hold the expected number of fields
      /// @param capacity is expected number of fields.
      void reserve(size_t capacity);

      /// @brief Get the count of fields in the set
      ///
      /// Group fields are counted individually.
      /// A Sequence is counted as one field.
      /// @returns the field count.
      size_t size()const
      {
        return used_;
      }

      /// @brief support indexing the set
      /// @param index 0 <= index < size()
      const MessageField & operator[](size_t index)const;

      /////////////////////////////////////
      /// Implement MessageAccessor methods
      virtual bool isPresent(const FieldIdentity & identity)const;
      virtual bool getUnsignedInteger(const FieldIdentity & identity, ValueType::Type type, uint64 & value)const;
      virtual bool getSignedInteger(const FieldIdentity & identity, ValueType::Type type, int64 & value)const;
      virtual bool getDecimal(const FieldIdentity & identity, ValueType::Type type, Decimal & value)const;
      virtual bool getString(const FieldIdentity & identity, ValueType::Type type, const StringBuffer *& value)const;
      virtual bool getGroup(const FieldIdentity & identity, const MessageAccessor *& group)const;
      virtual void endGroup(const FieldIdentity & identity, const MessageAccessor * group)const;
      virtual bool getSequenceLength(const FieldIdentity & identity, size_t & length)const;
      virtual bool getSequenceEntry(const FieldIdentity & identity, size_t index, const MessageAccessor *& entry)const;
      virtual void endSequenceEntry(const FieldIdentity & identity, size_t index, const MessageAccessor * entry)const;
      virtual void endSequence(const FieldIdentity & identity)const;


      /// @brief Add a field to the set.
      ///
      /// The FieldCPtr is copied, not the actual Field object.
      /// @param identity identifies this field
      /// @param value is the value to be assigned.
      void addField(const FieldIdentity & identity, const FieldCPtr & value);

      /// @brief Update an existing field in the set.
      ///
      /// The FieldCPtr is copied, not the actual Field object.
      /// @param identity identifies this field
      /// @param value is the value to be assigned.
      /// @return true if the field was found
      bool replaceField(const FieldIdentity & identity,
                        const FieldCPtr & value);

      /// @brief Get the value of the specified field.
      /// @param[in] name Identifies the desired field
      /// @param[out] value is the value that was found.
      /// @returns true if the field was found and has a value;
      bool getField(const std::string &name, FieldCPtr & value) const
      {
        return getField(Messages::FieldIdentity(name), value) ;
      }

      /// @brief Get the value of the specified field.
      /// @param[in] identity Identifies the desired field
      /// @param[out] value is the value that was found.
      /// @returns true if the field was found and has a value;
      bool getField(const Messages::FieldIdentity & identity, FieldCPtr & value) const;

      /// @brief support iterating through Fields in this FieldSet.
      const_iterator begin() const
      {
        return fields_;
      }

      /// @brief support iterating through Fields in this FieldSet.
      const_iterator end() const
      {
        return &fields_[used_];
      }

      /// @brief identify the application type associated with
      /// this set of fields via typeref.
      void setApplicationType(const std::string & type, const std::string & ns)
      {
        applicationType_ = type;
        applicationTypeNs_ = ns;
      }

      /// @brief get the application type associated with
      /// this set of fields via typeref.
      const std::string & getApplicationType()const
      {
        return applicationType_;
      }

      /// @brief get the namespace for the application type
      const std::string & getApplicationTypeNs()const
      {
        return applicationTypeNs_;
      }

      /// @brief swap the contents of this FieldSet with another one.
      /// Fast and no-throw.
      /// @param rhs the FieldSet with which to swap
      void swap(FieldSet & rhs)
      {
        applicationType_.swap(rhs.applicationType_);
        applicationTypeNs_.swap(rhs.applicationTypeNs_);
        swap_i(fields_, rhs.fields_);
        swap_i(capacity_, rhs.capacity_);
        swap_i(used_, rhs.used_);
      }

      ///// @brief access the field set
      /////
      ///// @returns this field set
      //virtual const FieldSet & getFieldSet() const
      //{
      //  return *this;
      //}

      /// @brief For DotNet: get everything in one call
      ///
      void getFieldInfo(
        size_t index,
        std::string & name,
        ValueType::Type & type,
        FieldCPtr & fieldPtr)const;

      /// @brief compare two field sets for equality
      /// @param rhs the target of the comparison
      /// @param reason a discription of the cause of the mismatch is written to this stream
      /// @returns true if the field sets are equal.
      bool equals(const FieldSet & rhs, std::ostream & reason) const;

    private:
      template<typename T>
      void swap_i(T & l, T & r)
      {
        T temp(l);
        l = r;
        r = temp;
      }
    protected:
      /// Application type as set by &lt;typeRef>
      std::string applicationType_;
      /// Namespace for the Application type as set by &lt;typeRef>
      std::string applicationTypeNs_;
    private:
      /// The collection of fields
      MessageField * fields_;
      size_t capacity_;
      size_t used_;
    };
  }
}
#endif // FIELDSET_H
