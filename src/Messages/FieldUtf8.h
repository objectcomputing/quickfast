// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDUTF8_H
#define FIELDUTF8_H
#include "Field.h"
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
      /// @brief Construct the field from a value in a std::string
      /// @param file is set to point to the created field
      /// @param value the value to be stored in the field
      static FieldCPtr create(const std::string & value);
      /// @brief Construct the field from a value in byte buffer
      /// @param file is set to point to the created field
      /// @param buffer the start of the value to be stored in the field
      /// @param length how many bytes (not characters) are in the value
      static FieldCPtr create(const uchar * buffer, size_t length);
      /// @brief Construct a NULL field (not an empty string)
      /// @param file is set to point to the created field
      static FieldCPtr createNull();

      /// @brief Assign a value to the field
      /// @param value the value to be stored in the field
      void setValue(const std::string & value);

      /// @brief Assign a value to the field
      /// @param buffer the start of the value to be stored in the field
      /// @param length how many bytes (not characters) are in the value
      void setValue(const uchar * buffer, size_t length);

      /// @brief a typical virtual destructor.
      virtual ~FieldUtf8();

      // implement selected virtual methods from Field
      virtual Field::FieldType getType()const;
      virtual const std::string & toUtf8() const;
      virtual bool isString() const;
      virtual const std::string & toString() const;
    private:
      std::string value_;
    };
  }
}
#endif // FIELDUTF8_H
