// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDUTF8_H
#define FIELDUTF8_H
#include <Messages/Field.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing unicode data.
    ///
    /// In the XML template file this field is described as &lt;string charset="unicode">
    class QuickFAST_Export FieldUtf8 : public Field{
    private:
      /// @brief Construct the field from a value in a std::string
      /// @param value the value to be stored in the field
      explicit FieldUtf8(const std::string & value);

      /// @brief Construct the field from a value in byte buffer
      /// @param buffer the start of the value to be stored in the field
      /// @param length how many bytes (not characters) are in the value
      FieldUtf8(const uchar * buffer, size_t length);

      /// @brief Construct a NULL field (not an empty string)
      FieldUtf8();

    public:
      /// Identify the type of data associated with this field
      const static ValueType::Type fieldType = ValueType::UTF8;

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
      /// @brief Construct a NULL field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      /// @brief a typical virtual destructor.
      virtual ~FieldUtf8();

      // implement selected virtual methods from Field
      virtual const StringBuffer & toUtf8() const;
      virtual bool isString() const;
    private:
      // temporary during transition to new field implementation
      std::string cachedValue_;
    };
  }
}
#endif // FIELDUTF8_H
