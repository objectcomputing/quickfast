// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef LINKEDBUFFER_FWD_H
#define LINKEDBUFFER_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

namespace QuickFAST
{
  namespace Communication
  {
    class LinkedBuffer;
    /// Normal use will be via raw pointer for speed. This is here to manage buffer lifetime (separately from use)
    typedef boost::shared_ptr<LinkedBuffer> BufferLifetime;
    /// A collection of BufferLifetimes
    typedef std::vector<BufferLifetime> BufferLifetimeManager;

    class BufferQueue;
    ///  Formerly separate classes, but BufferQueue was just as fast as the unordered collection so they merged.
    typedef BufferQueue BufferCollection;
    class SingleServerBufferQueue;
  }
}
#endif // LINKEDBUFFER_FWD_H
