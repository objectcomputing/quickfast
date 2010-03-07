// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#include <Messages/FieldIdentity_fwd.h>

namespace QuickFAST
{
  namespace DotNet
  {
    class ImplFieldSet;

    class ImplSequence
    {
    public:
      ImplSequence(
        Messages::FieldIdentityCPtr & lengthIdentity,
        size_t length
        );

      ~ImplSequence();

      /// How many entries are defined
      size_t size();
      ImplFieldSet & operator[](size_t index);

      void append(ImplFieldSet * entry);

      const std::string lengthName()const
      {
        return lengthName_;
      }
      const std::string lengthNamespace()const
      {
        return lengthNamespace_;
      }
      const std::string lengthId()const
      {
        return lengthId_;
      }

    private:
      std::string lengthName_;
      std::string lengthNamespace_;
      std::string lengthId_;
      std::vector<ImplFieldSet *> entries_;
    };
  }
}
