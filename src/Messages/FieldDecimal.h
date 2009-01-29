// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDDECIMAL_H
#define FIELDDECIMAL_H
#include "Field.h"
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
    public:
      /// @brief Construct the field from a Decimal value
      /// @param value the value to be stored in the field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(const Decimal & value);
      /// @brief Construct a NULL field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      /// @brief a typical virtual destructor.
      virtual ~FieldDecimal();

      // implement selected virtual methods from Field
      virtual Field::FieldType getType()const;
      virtual const Decimal & toDecimal() const;
    private:
      Decimal value_;
    };
  }
}
#endif // FIELDDECIMAL_H
