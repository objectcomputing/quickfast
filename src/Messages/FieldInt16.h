// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINT16_H
#define FIELDINT16_H
#include <Messages/Field.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing a signed 16 bit integer
    ///
    /// In the XML template file this field is described as &lt;int16>
    class QuickFAST_Export FieldInt16 : public Field{

      /// @brief Construct the field from an initial value
      /// @param value the value to be stored in the field
      explicit FieldInt16(int16 value);
      /// @brief Construct a NULL field
      FieldInt16();
    public:
      /// Identify the type of data associated with this field
      const static ValueType::Type fieldType = ValueType::INT16;

    public:
      /// @brief Construct the field from am int16 value
      /// @param value the value to be stored in the field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(int16 value);
      /// @brief Construct a NULL field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      /// @brief a typical virtual destructor.
      virtual ~FieldInt16();

      // implement selected virtual methods from Field
      virtual int16 toInt16() const;
      virtual void valueToStringBuffer()const;
      virtual bool isSignedInteger()const;
    private:
    };
  }
}
#endif // FIELDINT16_H
