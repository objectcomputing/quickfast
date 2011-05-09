// Copyright (c) 2009, 2010, 2011, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef ASYNCHFILERECEIVER_FWD_H
#define ASYNCHFILERECEIVER_FWD_H

namespace QuickFAST{
  namespace Communication{
#if defined(_WIN32)
    class AsynchFileReceiver;
    /// @brief smart pointer to a AsynchFileReceiver
    typedef boost::shared_ptr<AsynchFileReceiver> AsynchFileReceiverPtr;
#endif // _WIN32
  }
}
#endif // ASYNCHFILERECEIVER_FWD_H
