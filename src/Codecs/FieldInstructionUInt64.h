// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONUINT64_H
#define FIELDINSTRUCTIONUINT64_H
#include <Codecs/FieldInstructionInteger.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief An implementation for the &lt;uint64> field instruction.
    typedef FieldInstructionInteger<uint64, ValueType::UINT64, false> FieldInstructionUInt64;
  }
}
#endif // FIELDINSTRUCTIONUINT64_H
