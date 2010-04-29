// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDUINT64_H
#define FIELDUINT64_H
#include <Messages/Field.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing an unsigned 64 bit integer
    ///
    /// In the XML template file this field is described as &lt;uint64>
    class QuickFAST_Export FieldUInt64 : public Field
    {
      /// @brief Construct the field from an initial value
      /// @param value the value to be stored in the field
      explicit FieldUInt64(uint64 value);
      /// @brief Construct a NULL field
      FieldUInt64();

    public:
      /// Identify the type of data associated with this field
      const static ValueType::Type fieldType = ValueType::UINT64;

    public:
      /// @brief Construct the field from am uint64 value
      /// @param value the value to be stored in the field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(uint64 value);
      /// @brief Construct a NULL field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      virtual ~FieldUInt64();

      // implement selected virtual methods from Field
      virtual uint64 toUInt64() const;
      virtual void valueToStringBuffer()const;
      virtual bool isUnsignedInteger()const;
    private:
    };
  }
}
#endif // FIELDUINT64_H
