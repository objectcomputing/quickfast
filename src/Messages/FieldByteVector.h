// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDBYTEVECTOR_H
#define FIELDBYTEVECTOR_H
#include <Messages/Field.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing arbitrary binary data
    ///
    /// In the XML template file this field is described as &lt;byteVector>
    class QuickFAST_Export FieldByteVector : public Field{
      /// @brief Construct the field from a value in a std::string
      /// @param value the value to be stored in the field
      explicit FieldByteVector(const std::string & value);

      /// @brief Construct the field from a value in a std::string
      /// @param buffer the start of the value to be stored in the field
      /// @param length how many bytes are in the value
      FieldByteVector(const uchar * buffer, size_t length);
      /// @brief Construct a NULL field (not an empty field)
      FieldByteVector();
    public:
      /// Identify the type of data associated with this field
      const static ValueType::Type fieldType = ValueType::BYTEVECTOR;

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
      virtual ~FieldByteVector();

      // implement selected virtual methods from Field
      virtual const StringBuffer & toByteVector() const;
      virtual bool isString() const;
    private:
//      std::string value_;
    };
  }
}
#endif // FIELDBYTEVECTOR_H
