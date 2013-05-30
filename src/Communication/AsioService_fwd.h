// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ASIOSERVICE_FWD_H
#define ASIOSERVICE_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

// forward declare io_service without including
// boost header
namespace boost
{
  namespace asio
  {
    class io_service;
  }
}

namespace QuickFAST
{
  namespace Communication
  {
    class AsioService;
  }
}
#endif // ASIOSERVICE_FWD_H
