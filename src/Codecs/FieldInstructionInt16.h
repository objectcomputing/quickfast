// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONINT16_H
#define FIELDINSTRUCTIONINT16_H
#include <Codecs/FieldInstructionInteger.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief An implementation for the &lt;int16> field instruction.
    ///
    /// This is NOT a FAST standard field type.
    typedef FieldInstructionInteger<int16, ValueType::INT16, true> FieldInstructionInt16;
  }
}
#endif // FIELDINSTRUCTIONINT16_H
