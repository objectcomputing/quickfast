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

    /// @brief Implement the .NET FAST Sequence
    class ImplSequence
    {
    public:
      /// @brief Construct given info about the length field
      ImplSequence(
        Messages::FieldIdentityCPtr & lengthIdentity,
        size_t length
        );

      ~ImplSequence();

      /// @brief How many entries are defined
      size_t size();

      /// @brief Access an entry by index
      ImplFieldSet & operator[](size_t index);

      /// @brief Append a newly filled entry
      void append(ImplFieldSet * entry);

      /// @brief access the name of the length field
      const std::string lengthName()const
      {
        return lengthName_;
      }

      /// @brief access the namespace of the length field
      const std::string lengthNamespace()const
      {
        return lengthNamespace_;
      }

      /// @brief access the id of the length field
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
