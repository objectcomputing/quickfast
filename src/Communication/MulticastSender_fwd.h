// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTSENDER_FWD_H
#define MULTICASTSENDER_FWD_H

namespace QuickFAST{
  namespace Communication{
    class MulticastSender;
    /// @brief smart pointer to a MulticastSender
    typedef boost::shared_ptr<MulticastSender> MulticastSenderPtr;
  }
}
#endif // MULTICASTSENDER_FWD_H
