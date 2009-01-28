// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef SEQUENCE_FWD_H
#define SEQUENCE_FWD_H
namespace QuickFAST{
  namespace Messages{
    class Sequence;
    /// @brief Smart pointer to a Sequence.
    typedef boost::shared_ptr<Sequence> SequencePtr;
    /// @brief Smart pointer to a const Sequence.
    typedef boost::shared_ptr<const Sequence> SequenceCPtr;
  }
}

#endif // SEQUENCE_FWD_H
