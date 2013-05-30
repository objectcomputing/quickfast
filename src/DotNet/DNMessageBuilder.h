// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#pragma unmanaged
#include <Messages/ValueMessageBuilder_fwd.h>
#pragma managed

namespace QuickFAST
{
  namespace DotNet
  {
    /// @ abstract base class for Dot Net Message Builder
    public ref class DNMessageBuilder abstract
    {
    public:
      virtual ~DNMessageBuilder()
      {
      }

      /// @brief Access a C++ message builder
      /// @returns the C++ builder
      virtual Messages::ValueMessageBuilder & builder() = 0;

    };
  }
}

