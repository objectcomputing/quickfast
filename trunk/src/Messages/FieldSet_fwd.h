// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDSET_FWD_H
#define FIELDSET_FWD_H
namespace QuickFAST{
  namespace Messages{
    class FieldSet;
    /// @brief Smart pointer to a FieldSet.
    typedef boost::shared_ptr<FieldSet> FieldSetPtr;
    /// @brief Smart pointer to a const FieldSet.
    typedef boost::shared_ptr<const FieldSet> FieldSetCPtr;
  }
}
#endif // FIELDSET_FWD_H
