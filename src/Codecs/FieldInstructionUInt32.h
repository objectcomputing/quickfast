// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONUINT32_H
#define FIELDINSTRUCTIONUINT32_H
#include <Codecs/FieldInstructionInteger.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief An implementation for the &lt;uint32> field instruction.
    typedef FieldInstructionInteger<uint32, ValueType::UINT32, false> FieldInstructionUInt32;
    /// @brief A special field instruction for the sequence length field
    typedef FieldInstructionInteger<uint32, ValueType::LENGTH, false> FieldInstructionLength;
  }
}
#endif // FIELDINSTRUCTIONUINT32_H
