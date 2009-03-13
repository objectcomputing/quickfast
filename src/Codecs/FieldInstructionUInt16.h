// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONUINT16_H
#define FIELDINSTRUCTIONUINT16_H
#include <Codecs/FieldInstructionInteger.h>
#include <Messages/FieldUInt16.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief An implementation for the &lt;uint16> field instruction.
    ///
    /// This is NOT a FAST standard field type, but it is used by ARCA.
    typedef FieldInstructionInteger<uint16, Messages::FieldUInt16, false> FieldInstructionUInt16;
  }
}
#endif // FIELDINSTRUCTIONUINT16_H
