// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDASCII_H
#define FIELDASCII_H
#include "Field.h"
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

      /// @brief a typical virtual destructor.
      virtual ~FieldAscii();

      /// @brief Assign a value to the field
      /// @param buffer the start of the value to be stored in the field
      /// @param length how many characters are in the value
      void setValue(const uchar * buffer, size_t length);

      /// @brief change value
      /// @param value the new value for the field
      void setValue(uint64 value);
      /// @brief change value
      /// @param value the new value for the field
      void setValue(const std::string & value);

      // implement selected virtual methods from Field
      virtual Field::FieldType getType()const;
      virtual bool isString() const;
      virtual const std::string & toAscii() const;
      virtual const std::string & toString() const;
    private:
      std::string value_;
    };
  }
}
#endif // FIELDASCII_H
