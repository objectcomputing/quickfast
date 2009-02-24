// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDIDENTITY_FWD_H
#define FIELDIDENTITY_FWD_H
#include <Common/QuickFAST_Export.h>

namespace QuickFAST{
  namespace Messages{
    class FieldIdentity;
    /// @brief An intrusive smart pointer to a const FieldIdentity
#define UNSAFE_BUT_FASTx
#ifdef UNSAFE_BUT_FAST
    typedef const FieldIdentity * FieldIdentityCPtr;
    typedef FieldIdentity * FieldIdentityPtr;
#else //UNSAFE_BUT_FAST
    typedef boost::intrusive_ptr<const FieldIdentity> FieldIdentityCPtr;
    typedef boost::intrusive_ptr<FieldIdentity> FieldIdentityPtr;
#endif // UNSAFE_BUT_FAST
    void QuickFAST_Export intrusive_ptr_add_ref(const FieldIdentity * ptr);
    void QuickFAST_Export intrusive_ptr_release(const FieldIdentity * ptr);
    void QuickFAST_Export intrusive_ptr_add_ref(FieldIdentity * ptr);
    void QuickFAST_Export intrusive_ptr_release(FieldIdentity * ptr);

  }
}
#endif // FIELDIDENTITY_FWD_H
