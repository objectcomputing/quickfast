// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDUINT64_H
#define FIELDUINT64_H
#include "Field.h"
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing an unsigned 64 bit integer
    ///
    /// In the XML template file this field is described as &lt;uint64>
    class QuickFAST_Export FieldUInt64 : public Field{
      /// @brief Construct the field from an initial value
      /// @param value the value to be stored in the field
      explicit FieldUInt64(uint64 value);
      /// @brief Construct a NULL field
      FieldUInt64();
      /// @brief a typical virtual destructor.
    public:
      static FieldCPtr create(uint64 value);
      static FieldCPtr createNull();

      virtual ~FieldUInt64();

      /// @brief change value
      /// @param value the new value for the field
      void setValue(uint64 value);

      // implement selected virtual methods from Field
      virtual Field::FieldType getType()const;
      virtual uint64 toUInt64() const;
    private:
      uint64 value_;
    };
  }
}
#endif // FIELDUINT64_H
