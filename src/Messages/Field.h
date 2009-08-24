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
#include <Messages/Group_fwd.h>
#include <Messages/Sequence_fwd.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief The value of a field -- for use in Message and Dictionary.
    ///
    /// An abstract class intended to be specialized into the particular field type.
    class QuickFAST_Export Field
    {
    public:
      /// @brief Identify the data type of the field.
      ///
      /// These values correspond directly to the types of fields defined in
      /// the FAST standard.
      enum FieldType
      {
        INT8,     // not FAST Standard
        UINT8,    // not FAST Standard
        INT16,    // not FAST Standard
        UINT16,   // not FAST Standard
        INT32,
        UINT32,
        INT64,
        UINT64,
        DECIMAL,
        ASCII,
        UTF8,
        BYTEVECTOR,
        BITMAP,   // not FAST Standard
        SEQUENCE,
        GROUP,
        UNDEFINED
      };

      /// @brief translate a FieldType to a string
      static const std::string & typeName(FieldType type);

    protected:
      /// Protected constructor reinforces pure virtual
      Field(FieldType type, bool valid = false);
    public:
      /// @brief a typical virtual destructor.
      virtual ~Field() = 0;
      /// @brief Does this field have a value?
      /// @return true if the field has a value.
      virtual bool isDefined() const;

      /// @brief Check for a particular type of field
      /// @returns true if the field is the desired type
      bool isType(FieldType type) const
      {
        return type == getType();
      }

      /// @brief Get the field type
      /// @returns the enumerated field type
//      virtual FieldType getType()const = 0;
      FieldType getType() const
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
      bool isType(uchar type)const
      {
        return isType(Field::UINT8);
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
      bool isType(int8 type)const
      {
        return isType(INT8);
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
      bool isType(uint16 type)const
      {
        return isType(Field::UINT16);
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
      bool isType(int16 type)const
      {
        return isType(INT16);
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
      bool isType(uint32 type)const
      {
        return isType(Field::UINT32);
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
      bool isType(int32 type)const
      {
        return isType(INT32);
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
      bool isType(uint64 type)const
      {
        return isType(UINT64);
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
      bool isType(int64 type)const
      {
        return isType(Field::INT64);
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
      virtual const Decimal & toDecimal() const;

      /// @brief Retrieve the field value as Ascii
      /// @returns the value
      virtual const std::string & toAscii() const;

      /// @brief Retrieve the field value as a utf8
      /// @returns the value
      virtual const std::string & toUtf8() const;

      /// @brief Retrieve the field value as a byte vector
      /// @returns the value
      virtual const std::string & toByteVector() const;

      /// @brief Retrieve the field value as a byte vector
      /// @returns the value
      virtual const BitMap & toBitMap() const;

      /// @brief Is this field a kind of string (Ascii, Utf8, or ByteVector)?
      virtual bool isString()const;

      /// @brief Retrieve the field value as a string
      /// @returns the value
      virtual const std::string & toString()const;

      /// @brief Retrieve the field value as a group
      /// @returns the value
      virtual const Messages::GroupCPtr & toGroup() const;

      /// @brief Retrieve the field value as a sequence
      /// @returns the value
      virtual const Messages::SequenceCPtr & toSequence() const;

    protected:
      /// What type of data does this field contain?
      FieldType type_;
      /// false means this is a NULL value
      bool valid_;

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
