// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDUINT32_H
#define FIELDUINT32_H
#include <Messages/Field.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing an unsigned 32 bit integer
    ///
    /// In the XML template file this field is described as &lt;uint32>
    class QuickFAST_Export FieldUInt32 : public Field{
      /// @brief Construct the field from an initial value
      /// @param value the value to be stored in the field
      explicit FieldUInt32(uint32 value);
      /// @brief Construct a NULL field
      FieldUInt32();
    public:
      /// Identify the type of data associated with this field
      const static ValueType::Type fieldType = ValueType::UINT32;
    public:
      /// @brief Construct the field from am uint32 value
      /// @param value the value to be stored in the field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(uint32 value);
      /// @brief Construct a NULL field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      /// @brief a typical virtual destructor.
      virtual ~FieldUInt32();

      // implement selected virtual methods from Field
      virtual uint32 toUInt32() const;
      virtual void valueToStringBuffer()const;
      virtual bool isUnsignedInteger()const;
    private:
      virtual void freeField()const;
    private:
      static FieldCPtr nullField_;
    };
  }
}
#endif // FIELDUINT32_H
