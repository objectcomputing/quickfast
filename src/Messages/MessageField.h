// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGEFIELD_H
#define MESSAGEFIELD_H

///@todo: think about how to get the performance improvement without compromising safety
/// The problem is that copying the identity is expensive (15% on the profiler) but keeping
/// a pointer to the identity gets into object lifetime issues.  The identity was not
/// originally designed to be heap-allocated, counted ptr managed, but maybe it should have
/// been because it's immutable at encode/decode time.
#define SAFE_BUT_SLOW

#include "MessageField_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Messages/Field_fwd.h>
#include <Messages/FieldIdentity.h>
#include <Common/Profiler.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief the representation of a field within a message.
    class QuickFAST_Export MessageField
    {
    public:
      /// @brief Construct from an identity and a typed value.
      MessageField(const FieldIdentity & identity, const FieldCPtr & field)
        : identity_(identity)
        , field_(field)
      {
      }

      /// @brief copy constructor
      /// @param rhs the source from which to copy
      MessageField(const MessageField & rhs)
        : identity_(rhs.identity_)
        , field_(rhs.field_)
      {
      }

    public:

      /// @brief get the name of the field
      /// @returns the fully qualified field name
      const std::string name()const
      {
        return identity_.name();
      }
      /// @brief get the identity of the field
      /// @returns the identifying information for this field
      const FieldIdentity & getIdentity()const
      {
        return identity_;
      }

      /// @brief get the value of the field
      /// @returns  a pointer to the Field
      const FieldCPtr & getField()const
      {
        return field_;
      }
    private:
      const FieldIdentity & identity_;
      FieldCPtr field_;
    };
  }
}
#endif // MESSAGEFIELD_H
