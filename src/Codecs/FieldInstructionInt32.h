// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONINT32_H
#define FIELDINSTRUCTIONINT32_H
#include <Codecs/FieldInstructionInteger.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief An implementation for the &lt;int32> field instruction.
    typedef FieldInstructionInteger<int32, ValueType::INT32, true> FieldInstructionInt32;
  }
}
#endif // FIELDINSTRUCTIONINT32_H
