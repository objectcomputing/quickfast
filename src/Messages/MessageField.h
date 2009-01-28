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
      MessageField(const FieldIdentity & identity, FieldCPtr & field)
#ifdef SAFE_BUT_SLOW
        : identity_(identity)
#else
        : identity_(&identity)
#endif
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

      /// @brief get the name of the field
      /// @returns the fully qualified field name
      const std::string name()const
      {
#ifdef SAFE_BUT_SLOW
        return identity_.name();
#else
        return identity_->name();
#endif
      }
      /// @brief get the identity of the field
      /// @returns the identifying information for this field
      const FieldIdentity & getIdentity()const
      {
#ifdef SAFE_BUT_SLOW
        return identity_;
#else
        return *identity_;
#endif
      }

      /// @brief get the value of the field
      /// @returns  a pointer to the Field
      const FieldCPtr & getField()const
      {
        return field_;
      }
    private:
#ifdef SAFE_BUT_SLOW
      FieldIdentity identity_;
#else
      const FieldIdentity * identity_;
#endif
      FieldCPtr field_;
    };
  }
}
#endif // MESSAGEFIELD_H
