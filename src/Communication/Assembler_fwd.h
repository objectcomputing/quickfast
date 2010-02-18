// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ASSEMBLER_FWD_H
#define ASSEMBLER_FWD_H
namespace QuickFAST{
  namespace Communication
  {
    class Assembler;
    /// @brief smart pointer to a BufferConsumer
    typedef boost::shared_ptr<Assembler> AssemblerPtr;

  }
}
#endif /* ASSEMBLER_FWD_H */
