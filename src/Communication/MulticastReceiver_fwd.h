// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTRECEIVER_FWD_H
#define MULTICASTRECEIVER_FWD_H

namespace QuickFAST{
  namespace Communication{
    class MulticastReceiver;
    /// @brief smart pointer to a MulticastReceiver
    typedef boost::shared_ptr<MulticastReceiver> MulticastReceiverPtr;
  }
}
#endif // MULTICASTRECEIVER_FWD_H
