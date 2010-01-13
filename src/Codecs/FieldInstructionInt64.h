// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONINT64_H
#define FIELDINSTRUCTIONINT64_H
#include <Codecs/FieldInstructionInteger.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief An implementation for the &lt;int64> field instruction.
    typedef FieldInstructionInteger<int64, ValueType::INT64, true> FieldInstructionInt64;
  }
}
#endif // FIELDINSTRUCTIONINT64_H
