// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONEXPONENT_H
#define FIELDINSTRUCTIONEXPONENT_H
#include <Codecs/FieldInstructionInteger.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief An implementation for the &lt;exponent> field instruction within a &lt;decimal>.
    typedef FieldInstructionInteger</*exponent_t*/int32, ValueType::EXPONENT, true> FieldInstructionExponent;
    /// @brief a pointer to a FieldInstrucionExponent
    typedef boost::shared_ptr<FieldInstructionExponent> FieldInstructionExponentPtr;
    /// @brief a const pointer to a FieldInstrucionExponent
    typedef boost::shared_ptr<const FieldInstructionExponent> FieldInstructionExponentCPtr;
  }
}
#endif // FIELDINSTRUCTIONEXPONENT_H
