// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONUINT32_H
#define FIELDINSTRUCTIONUINT32_H
#include <Codecs/FieldInstructionInteger.h>
//#include <Messages/FieldUInt32.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief An implementation for the &lt;uint32> field instruction.
    typedef FieldInstructionInteger<uint32, ValueType::UINT32, false> FieldInstructionUInt32;
  }
}
#endif // FIELDINSTRUCTIONUINT32_H
