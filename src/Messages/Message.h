// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGE_H
#define MESSAGE_H
#include "Message_fwd.h"
#include <Common/Types.h>
#include <Messages/FieldSet.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief Internal representation of a Message to be encoded or decoded.
    /// @todo: consider typedef FieldSet Message
    class QuickFAST_Export Message : public FieldSet
    {
      Message();
    public:
      /// @brief Construct an empty Message
      Message(boost::shared_ptr<FieldSet::BufferCache> cache, size_t expectedNumberOfFields);

      /// @brief Copy construct Message
      Message(const Message & rhs)
      : FieldSet(rhs)
      {
      }
    };
  }
}
#endif // MESSAGE_H
