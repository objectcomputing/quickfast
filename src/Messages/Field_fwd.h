// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELD_FWD_H
#define FIELD_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

#include <Common/QuickFAST_Export.h>
#include <boost/intrusive_ptr.hpp>

namespace QuickFAST{
  namespace Messages{
    class Field;
    /// @brief An intrusive smart pointer to a const Field
    typedef boost::intrusive_ptr<const Field> FieldCPtr;

    /// @brief Support for intrusive_ptr -- add a reference
    /// @param ptr points to the object managed by the pointer.
    void QuickFAST_Export intrusive_ptr_add_ref(const Field * ptr);
    /// @brief Support for intrusive_ptr -- release a reference
    /// @param ptr points to the object managed by the pointer.
    void QuickFAST_Export intrusive_ptr_release(const Field * ptr);
  }
}
#endif // FIELD_FWD_H
