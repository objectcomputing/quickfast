// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDOP_FWD_H
#define FIELDOP_FWD_H
namespace QuickFAST{
  namespace Codecs{
    class FieldOp;
    /// @brief A smart pointer to a FieldOp
    typedef boost::shared_ptr<FieldOp> FieldOpPtr;
    /// @brief A smart pointer to a const FieldOp
    typedef boost::shared_ptr<const FieldOp> FieldOpCPtr;
  }
}
#endif // FIELDOP_FWD_H
