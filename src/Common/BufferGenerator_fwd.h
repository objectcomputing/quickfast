// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef BUFFERGENERATOR_FWD_H
#define BUFFERGENERATOR_FWD_H

namespace QuickFAST{
  namespace Common{
    class BufferGenerator;
    /// @brief smart pointer to a BufferConsumer
    typedef boost::shared_ptr<BufferGenerator> BufferGeneratorPtr;
    typedef boost::weak_ptr<BufferGenerator> BufferGeneratorWeak;
  }
}
#endif // BUFFERGENERATOR_FWD_H
