// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONINT8_H
#define FIELDINSTRUCTIONINT8_H
#include <Codecs/FieldInstructionInteger.h>
#include <Messages/FieldInt8.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief An implementation for the &lt;int8> field instruction.
    ///
    /// This is NOT a FAST standard field type.
    typedef FieldInstructionInteger<int8, Messages::FieldInt8, true> FieldInstructionInt8;
  }
}
#endif // FIELDINSTRUCTIONINT8_H
