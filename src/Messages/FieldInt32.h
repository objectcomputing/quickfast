// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINT32_H
#define FIELDINT32_H
#include <Messages/Field.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing a signed 32 bit integer
    ///
    /// In the XML template file this field is described as &lt;int32>
    class QuickFAST_Export FieldInt32 : public Field{

      /// @brief Construct the field from an initial value
      /// @param value the value to be stored in the field
      explicit FieldInt32(int32 value);
      /// @brief Construct a NULL field
      FieldInt32();

    public:
      /// Identify the type of data associated with this field
      const static ValueType::Type fieldType = ValueType::INT32;

    public:
      /// @brief Construct the field from am int32 value
      /// @param value the value to be stored in the field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(int32 value);
      /// @brief Construct a NULL field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      /// @brief a typical virtual destructor.
      virtual ~FieldInt32();

      // implement selected virtual methods from Field
      virtual int32 toInt32() const;
      virtual void valueToStringBuffer()const;
      virtual bool isSignedInteger()const;

    private:
    };
  }
}
#endif // FIELDINT32_H
