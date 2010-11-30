// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef ASIOSERVICE_FWD_H
#define ASIOSERVICE_FWD_H

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
