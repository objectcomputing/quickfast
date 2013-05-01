// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#include <DotNet/ImplField.h>
#pragma unmanaged
//#include <Common/Value.h>


namespace QuickFAST
{
  namespace DotNet
  {
    /// @brief .NET internal implementation of a Field Set
    class ImplFieldSet
    {
    public:
      /// @brief construct given the number of fields expected
      ImplFieldSet(size_t fieldCount);
      ~ImplFieldSet();
      /// @brief access a field by index
      ImplField & operator[](size_t index);
      /// @brief append a new, empty field, return a reference to it.
      ImplField & append();

      /// @brief How many fields are in this set
      size_t size() const;
    private:
      ImplFieldSet();
      ImplFieldSet(ImplFieldSet const &);
      ImplFieldSet& operator = (ImplFieldSet const &);
    private:
      size_t capacity_;
      size_t used_;
      boost::shared_array<ImplField> fields_;
    };
  }
}
#pragma managed
