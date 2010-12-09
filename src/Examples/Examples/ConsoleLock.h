// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef CONSOLELOCK_H
#define CONSOLELOCK_H
namespace QuickFAST{
  namespace Examples{
    class ConsoleLock
    {
    public:
      static boost::mutex consoleMutex;
    };
  }
}
#endif // CONSOLELOCK_H
