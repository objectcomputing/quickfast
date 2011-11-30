// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ASYNCHFILESENDER_FWD_H
#define ASYNCHFILESENDER_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

namespace QuickFAST
{
  namespace Communication
  {
#if defined(_WIN32)
    class AsynchFileSender;
#endif // _WIN32
  }
}
#endif // ASYNCHFILESENDER_FWD_H
