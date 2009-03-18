// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDBITMAP_H
#define FIELDBITMAP_H
#include <Messages/Field.h>
#include <Common/BitMap.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing arbitrary binary data
    ///
    /// In the XML template file this field is described as &lt;bitmap>
    class QuickFAST_Export FieldBitMap
      : public Field
    {
      /// @brief Construct the field from a value in a std::string
      /// @param value the value to be stored in the field
      explicit FieldBitMap(const BitMap & value);

      /// @brief Construct the field from a value in a std::string
      /// @param buffer the start of the value to be stored in the field
      /// @param length how many bytes are in the value
      FieldBitMap(const uchar * buffer, size_t length);
      /// @brief Construct a NULL field (not an empty field)
      FieldBitMap();

    public:
      /// @brief Construct the field from a value in a std::string
      /// @param value the value to be stored in the field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(const BitMap & value);

      /// @brief Construct the field from a value in byte buffer
      /// @param buffer the start of the value to be stored in the field
      /// @param length how many bytes (not characters) are in the value
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(const uchar * buffer, size_t length);

      /// @brief Construct a NULL field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      /// @brief a typical virtual destructor.
      virtual ~FieldBitMap();

      // implement selected virtual methods from Field
      virtual Field::FieldType getType()const;
      virtual const BitMap & toBitMap() const;
    private:
      BitMap value_;
    };
  }
}
#endif // FIELDBITMAP_H
