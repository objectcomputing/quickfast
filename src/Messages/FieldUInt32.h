// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDUINT32_H
#define FIELDUINT32_H
#include "Field.h"
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing an unsigned 32 bit integer
    ///
    /// In the XML template file this field is described as &lt;uint32>
    class QuickFAST_Export FieldUInt32 : public Field{
      /// @brief Construct the field from an initial value
      /// @param value the value to be stored in the field
      explicit FieldUInt32(uint32 value);
      /// @brief Construct a NULL field
      FieldUInt32();
    public:
      static FieldCPtr create(uint32 value);
      /// @brief Construct a NULL field (not an empty string)
      /// @param file is set to point to the created field
      static FieldCPtr createNull();

      /// @brief a typical virtual destructor.
      virtual ~FieldUInt32();

      /// @brief change value
      /// @param value the new value for the field
      void setValue(uint32 value);

      // implement selected virtual methods from Field
      virtual Field::FieldType getType()const;
      virtual uint32 toUInt32() const;
    private:
      uint32 value_;
    };
  }
}
#endif // FIELDUINT32_H
