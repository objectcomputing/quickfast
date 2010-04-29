// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINT8_H
#define FIELDINT8_H
#include <Messages/Field.h>
namespace QuickFAST
{
  namespace Messages
  {
    /// @brief A field containing a signed 8 bit integer
    ///
    /// In the XML template file this field is described as &lt;int8>
    class QuickFAST_Export FieldInt8
      : public Field
    {

      /// @brief Construct the field from an initial value
      /// @param value the value to be stored in the field
      explicit FieldInt8(int8 value);
      /// @brief Construct a NULL field
      FieldInt8();
    public:
      /// Identify the type of data associated with this field
      const static ValueType::Type fieldType = ValueType::INT8;
    public:
      /// @brief Construct the field from am int8 value
      /// @param value the value to be stored in the field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(int8 value);
      /// @brief Construct a NULL field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      /// @brief a typical virtual destructor.
      virtual ~FieldInt8();

      // implement selected virtual methods from Field
      virtual int8 toInt8() const;
      virtual void valueToStringBuffer()const;
      virtual bool isSignedInteger()const;
    private:

    };
  }
}
#endif // FIELDINT8_H
