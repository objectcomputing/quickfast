// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTION_FWD_H
#define FIELDINSTRUCTION_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

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
