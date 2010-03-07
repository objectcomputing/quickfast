// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#include <DotNet/DNField.h>

namespace QuickFAST
{
  namespace DotNet
  {
    class ImplFieldSet
    {
    public:
      ImplFieldSet(size_t fieldCount);
      ~ImplFieldSet();
      ImplField & operator[](size_t index);
      ImplField & append();

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
