// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONUINT8_H
#define FIELDINSTRUCTIONUINT8_H
#include <Codecs/FieldInstructionInteger.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief An implementation for the &lt;uint8> field instruction.
    ///
    /// This is NOT a FAST standard field type, but it is used by ARCA.
    typedef FieldInstructionInteger<uchar, ValueType::UINT8, false> FieldInstructionUInt8;
  }
}
#endif // FIELDINSTRUCTIONUINT8_H
