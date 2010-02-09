// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef TCPRECEIVER_FWD_H
#define TCPRECEIVER_FWD_H

namespace QuickFAST{
  namespace Communication{
    class TCPReceiver;
    /// @brief smart pointer to a TCPReceiver
    typedef boost::shared_ptr<TCPReceiver> TCPReceiverPtr;
  }
}
#endif // TCPRECEIVER_FWD_H
