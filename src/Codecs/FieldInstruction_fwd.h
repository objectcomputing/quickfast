// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTION_FWD_H
#define FIELDINSTRUCTION_FWD_H

#include <boost/shared_ptr.hpp>

namespace QuickFAST{
  namespace Codecs{
    class FieldInstruction;
    /// @brief A smart pointer to a FieldInstruction
    typedef boost::shared_ptr<FieldInstruction> FieldInstructionPtr;
    /// @brief A smart pointer to a const FieldInstruction
    typedef boost::shared_ptr<const FieldInstruction> FieldInstructionCPtr;
  }
}
#endif // FIELDINSTRUCTION_FWD_H
