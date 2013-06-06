// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDUINT8_H
#define FIELDUINT8_H
#include <Messages/Field.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing an unsigned 8 bit integer
    ///
    /// In the XML template file this field is described as &lt;uint8>
    class QuickFAST_Export FieldUInt8 : public Field{
      /// @brief Construct the field from an initial value
      /// @param value the value to be stored in the field
      explicit FieldUInt8(uchar value);
      /// @brief Construct a NULL field
      FieldUInt8();
    public:
      /// Identify the type of data associated with this field
      const static ValueType::Type fieldType = ValueType::UINT8;

    public:
      /// @brief Construct the field from an uchar value
      /// @param value the value to be stored in the field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(uchar value);
      /// @brief Construct a NULL field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      /// @brief a typical virtual destructor.
      virtual ~FieldUInt8();

      // implement selected virtual methods from Field
      virtual uchar toUInt8() const;
      virtual void valueToStringBuffer()const;
      virtual bool isUnsignedInteger()const;
    private:
      virtual void freeField()const;
    private:
      static FieldCPtr nullField_;
    };
  }
}
#endif // FIELDUINT8_H
