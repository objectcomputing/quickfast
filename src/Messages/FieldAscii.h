// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDASCII_H
#define FIELDASCII_H
#include <Messages/Field.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing ascii data.
    ///
    /// In the XML template file this field is described as &lt;string charset="ascii">
    class QuickFAST_Export FieldAscii : public Field
{
      /// @brief Construct the field from a value in a std::string
      /// @param value the value to be stored in the field
      explicit FieldAscii(const std::string & value);
      /// @brief Construct the field from a value in byte buffer
      /// @param value the start of the value to be stored in the field
      /// @param length how many characters are in the value
      FieldAscii(const uchar * value, size_t length);
      /// @brief Construct a NULL field (not an empty string)
      FieldAscii();
    public:
      /// Identify the type of data associated with this field
      const static ValueType::Type fieldType = ValueType::ASCII;
    public:
      /// @brief Construct the field from a value in a std::string
      /// @param value the value to be stored in the field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(const std::string & value);
      /// @brief Construct the field from a value in byte buffer
      /// @param buffer the start of the value to be stored in the field
      /// @param length how many bytes (not characters) are in the value
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(const uchar * buffer, size_t length);
      /// @brief Construct a NULL field (not an empty string)
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      /// @brief a typical virtual destructor.
      virtual ~FieldAscii();

      // implement selected virtual methods from Field
      virtual bool isString() const;
      virtual const StringBuffer & toAscii() const;
    private:
    };
  }
}
#endif // FIELDASCII_H
