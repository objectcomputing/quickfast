// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDIDENTITY_FWD_H
#define FIELDIDENTITY_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS
#include <Common/QuickFAST_Export.h>

namespace QuickFAST{
  namespace Messages{
    class FieldIdentity;
    ///// @brief An intrusive smart pointer to a const FieldIdentity
    //typedef boost::intrusive_ptr<const FieldIdentity> FieldIdentityCPtr;
    ///// @brief An intrusive smart pointer to a non-const FieldIdentity
    //typedef boost::intrusive_ptr<FieldIdentity> FieldIdentityPtr;
    ///// @brief Support for intrusive_ptr -- add a reference
    ///// @param ptr points to the object managed by the pointer.
    //void QuickFAST_Export intrusive_ptr_add_ref(const FieldIdentity * ptr);
    ///// @brief Support for intrusive_ptr -- relese a reference
    ///// @param ptr points to the object managed by the pointer.
    //void QuickFAST_Export intrusive_ptr_release(const FieldIdentity * ptr);
    ///// @brief Support for intrusive_ptr -- add a reference
    ///// @param ptr points to the object managed by the pointer.
    //void QuickFAST_Export intrusive_ptr_add_ref(FieldIdentity * ptr);
    ///// @brief Support for intrusive_ptr -- release a reference
    ///// @param ptr points to the object managed by the pointer.
    //void QuickFAST_Export intrusive_ptr_release(FieldIdentity * ptr);

  }
}
#endif // FIELDIDENTITY_FWD_H
