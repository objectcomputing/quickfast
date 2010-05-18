// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELD_H
#define FIELD_H
#include "Field_fwd.h"
#include <Common/Types.h>
#include <Common/Decimal.h>
#include <Common/BitMap.h>
#include <Common/StringBuffer.h>
#include <Messages/Group_fwd.h>
#include <Messages/Sequence_fwd.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief The value of a field -- for use in Message and Dictionary.
    ///
    /// An abstract class intended to be specialized into the particular field type.
    class QuickFAST_Export Field
    {
    protected:
      /// Protected constructor reinforces pure virtual
      Field(ValueType::Type type, bool valid = false);
    public:
      /// @brief a typical virtual destructor.
      virtual ~Field() = 0;

      /// @brief compare to field for type and value
      ///
      /// The default implementation handles all string, integer, and decimal types.
      /// Override this for other types.
      /// @param rhs is the target of the comparison.
      virtual bool operator == (const Field & rhs) const;

      /// @brief inequality operator
      /// @param rhs is the target of the comparison.
      bool operator != (const Field & rhs)const
      {
        return ! (*this == rhs);
      }

      /// @brief display the value as a string.  Low performance
      virtual const StringBuffer & displayString() const
      {
        if(valid_ && !isString() && string_.empty())
        {
          valueToStringBuffer();
        }
        return string_;
      }

      /// @brief Does this field have a value?
      /// @return true if the field has a value.
      virtual bool isDefined() const;

      /// @brief Check for a particular type of field
      /// @returns true if the field is the desired type
      bool isType(ValueType::Type type) const
      {
        return type == getType();
      }

      /// @brief Get the field type
      /// @returns the enumerated field type
      ValueType::Type getType() const
      {
        return type_;
      }

      ////////
      // UINT8

      /// @brief Retrieve the field value as an uchar
      /// @returns the value
      virtual uchar toUInt8() const;

      /// @brief An alternative way to check data type.
      ///
      /// This is convenenient for template-based code.
      bool isType(uchar /*type*/)const
      {
        return isType(ValueType::UINT8);
      }

      /// @brief An alternative way to retrieve the value.
      ///
      /// This is convenenient for template-based code.
      void getValue(uchar & value)const
      {
        value = toUInt8();
      }

      ////////
      // INT8

      /// @brief Retrieve the field value as an int8
      /// @returns the value
      virtual int8 toInt8() const;


      /// @brief An alternative way to check the type
      ///
      /// This is convenenient for template-based code.
      bool isType(int8 /*type*/)const
      {
        return isType(ValueType::INT8);
      }

      /// @brief An alternative way to retrieve the value.
      ///
      /// This is convenenient for template-based code.
      void getValue(int8 & value)const
      {
        value = toInt8();
      }

      /////////
      // UINT16

      /// @brief Retrieve the field value as an uint16
      /// @returns the value
      virtual uint16 toUInt16() const;

      /// @brief An alternative way to check data type.
      ///
      /// This is convenenient for template-based code.
      bool isType(uint16 /*type*/)const
      {
        return isType(ValueType::UINT16);
      }

      /// @brief An alternative way to retrieve the value.
      ///
      /// This is convenenient for template-based code.
      void getValue(uint16 & value)const
      {
        value = toUInt16();
      }

      /////////
      // INT16

      /// @brief Retrieve the field value as an int16
      /// @returns the value
      virtual int16 toInt16() const;

      /// @brief An alternative way to check the type
      ///
      /// This is convenenient for template-based code.
      bool isType(int16 /*type*/)const
      {
        return isType(ValueType::INT16);
      }

      /// @brief An alternative way to retrieve the value.
      ///
      /// This is convenenient for template-based code.
      void getValue(int16 & value)const
      {
        value = toInt16();
      }

      /////////
      // UINT32

      /// @brief Retrieve the field value as an uint32
      /// @returns the value
      virtual uint32 toUInt32() const;

      /// @brief An alternative way to check data type.
      ///
      /// This is convenenient for template-based code.
      bool isType(uint32 /*type*/)const
      {
        return isType(ValueType::UINT32);
      }

      /// @brief An alternative way to retrieve the value.
      ///
      /// This is convenenient for template-based code.
      void getValue(uint32 & value)const
      {
        value = toUInt32();
      }


      /////////
      // INT32

      /// @brief Retrieve the field value as an int32
      /// @returns the value
      virtual int32 toInt32() const;

      /// @brief An alternative way to check the type
      ///
      /// This is convenenient for template-based code.
      bool isType(int32 /*type*/)const
      {
        return isType(ValueType::INT32);
      }

      /// @brief An alternative way to retrieve the value.
      ///
      /// This is convenenient for template-based code.
      void getValue(int32 & value)const
      {
        value = toInt32();
      }

      /////////
      // UINT64

      /// @brief Retrieve the field value as an uint64
      /// @returns the value
      virtual uint64 toUInt64() const;

      /// @brief An alternative way to check the type
      ///
      /// This is convenenient for template-based code.
      bool isType(uint64 /*type*/)const
      {
        return isType(ValueType::UINT64);
      }

      /// @brief An alternative way to retrieve the value.
      ///
      /// This is convenenient for template-based code.
      void getValue(uint64 & value)const
      {
        value = toUInt64();
      }

      /////////
      // INT64

      /// @brief Retrieve the field value as an int64
      /// @returns the value
      virtual int64 toInt64() const;

      /// @brief An alternative way to check the type
      ///
      /// This is convenenient for template-based code.
      bool isType(int64 /*type*/)const
      {
        return isType(ValueType::INT64);
      }

      /// @brief An alternative way to retrieve the value.
      ///
      /// This is convenenient for template-based code.
      void getValue(int64 & value)const
      {
        value = toInt64();
      }

      /// @brief Retrieve the field value as a Decimal
      /// @returns the value
      virtual const Decimal toDecimal() const;

      /// @brief Retrieve the field value as Ascii
      /// @returns the value
      virtual const StringBuffer & toAscii() const;

      /// @brief Retrieve the field value as a utf8
      /// @returns the value
      virtual const StringBuffer & toUtf8() const;

      /// @brief Retrieve the field value as a byte vector
      /// @returns the value
      virtual const StringBuffer & toByteVector() const;

      /// @brief Retrieve the field value as a byte vector
      /// @returns the value
      virtual const BitMap & toBitMap() const;

      /// @brief Is this field a kind of string (Ascii, Utf8, or ByteVector)?
      /// @returns true if this is a string
      virtual bool isString()const;

      /// @brief Is this field a kind of unsigned integer
      /// @returns true if this is an unsigned integer
      virtual bool isUnsignedInteger()const;

      /// @brief Is this field a kind of signed integer
      /// @returns true if this is a signed integer
      virtual bool isSignedInteger()const;

      /// @brief Retrieve the field value as a string
      /// @returns the value
      const StringBuffer & toString()const;

      /// @brief Retrieve the field value as a signed integer
      /// @returns the value
      int64 toSignedInteger()const;

      /// @brief Retrieve the field value as an unsigned integer
      /// @returns the value
      uint64 toUnsignedInteger()const;

      /// @brief Retrieve the field value as a group
      /// @returns the value
      virtual const Messages::GroupCPtr & toGroup() const;

      /// @brief Retrieve the field value as a sequence
      /// @returns the value
      virtual const Messages::SequenceCPtr & toSequence() const;

    protected:
      /// @brief Data types that do NOT store their value in string_ should
      /// override this and put a value in string_ for display purposes only.
      virtual void valueToStringBuffer()const;

    protected:
      /// What type of data does this field contain?
      ValueType::Type type_;
      /// false means this is a NULL value
      bool valid_;

      ///////////////////////////////////////////////////
      // Field contents, except for groups and sequences,
      // is stored in the base class.
      // This simplifies the .NET code and somewhat avoids
      // extra memory allocations.

      ///@brief Data for any of the unsigned integral types.
      unsigned long long unsignedInteger_;
      ///@brief Data for any of the signed integral types. Also Decimal mantissa.
      signed long long signedInteger_;
      ///@brief Exponent for Decimal types (mantissa is in signedInteger_)
      QuickFAST::exponent_t exponent_;
      ///@brief Length of locally allocated string_ buffer
      size_t stringLength_;
      ///@brief Buffer containing string value. Owned by this object
      mutable StringBuffer string_;

    private:
      friend void QuickFAST_Export intrusive_ptr_add_ref(const Field * ptr);
      friend void QuickFAST_Export intrusive_ptr_release(const Field * ptr);
      virtual void freeField()const;
      mutable unsigned long refcount_;
    };

    inline
    void
    intrusive_ptr_add_ref(const Field * ptr)
    {
      ++ptr->refcount_;
    }

    inline
    void
    intrusive_ptr_release(const Field * ptr)
    {
      if(--ptr->refcount_ == 0)
      {
        ptr->freeField();
      }
    }
  }
}
#endif // FIELD_H
