// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDDECIMAL_H
#define FIELDDECIMAL_H
#include <Messages/Field.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing a scaled decimal number
    ///
    /// In the XML template file this field is described as &lt;decimal>
    class QuickFAST_Export FieldDecimal : public Field{
      /// @brief Construct a NULL field
      FieldDecimal();

      /// @brief Construct given a decimal value
      /// @param value is the initial value for the field.
      FieldDecimal(const Decimal & value);

      /// @brief Construct given a mantissa and exponent
      /// @param mantissa is the initial value for the mantissa
      /// @param exponent is the initial value for the exponent
      FieldDecimal(mantissa_t mantissa, exponent_t exponent);

    public:
      /// Identify the type of data associated with this field
      const static ValueType::Type fieldType = ValueType::DECIMAL;

    public:
      /// @brief Construct the field from a Decimal value
      /// @param value the value to be stored in the field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(const Decimal & value);

      /// @brief Construct the field from a Decimal value
      /// @param mantissa is the initial value for the mantissa
      /// @param exponent is the initial value for the exponent
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(mantissa_t mantissa, exponent_t exponent);

      /// @brief Construct a NULL field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      /// @brief a typical virtual destructor.
      virtual ~FieldDecimal();

      // implement selected virtual methods from Field
      virtual const Decimal toDecimal() const;
      virtual void valueToStringBuffer()const;
    private:
    };
  }
}
#endif // FIELDDECIMAL_H
