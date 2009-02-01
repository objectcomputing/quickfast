// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef ASIOSERVICE_H
#define ASIOSERVICE_H
#include <Common/QuickFAST_Export.h>
// In gcc including asio.hpp in precompiled headers causes problems
#include <boost/asio.hpp>

namespace QuickFAST{

  /// @brief Allow sharing a boost::io_service
  class QuickFAST_Export AsioService
  {
  public:
    /// @brief Construct using private io service
    AsioService();

    /// @brief Construct using shared io service
    AsioService(boost::asio::io_service & ioService);

    ~AsioService();

    /// @brief Run the event loop.
    void run(size_t threadCount = 1);

    /// @todo add poll, etc methods. and/or support external ioservice

    /// @ brief Allow external access (thereby blowing encapsulization)
    boost::asio::io_service & ioService()
    {
      return ioService_;
    }
  private:
    boost::scoped_ptr<boost::asio::io_service> privateIoService_;
  protected:
    /// Protected reference to the io_service.
    boost::asio::io_service & ioService_;
  };
}
#endif // ASIOSERVICE_H
