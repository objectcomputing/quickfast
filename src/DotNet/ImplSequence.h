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
        const Messages::FieldIdentity & lengthIdentity,
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
      const std::string lengthName()const;

      /// @brief access the namespace of the length field
      const std::string lengthNamespace()const;

      /// @brief access the id of the length field
      const std::string lengthId()const;

    private:
      size_t capacity_;
      size_t used_;
      const Messages::FieldIdentity & lengthIdentity_;
      typedef ImplFieldSet * EntryPtr;
      boost::scoped_array<EntryPtr> entries_;
    };
  }
}
