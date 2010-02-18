// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef RAWFILERECEIVER_FWD_H
#define RAWFILERECEIVER_FWD_H

namespace QuickFAST{
  namespace Communication{
    class RawFileReceiver;
    /// @brief smart pointer to a FileReceiver
    typedef boost::shared_ptr<RawFileReceiver> RawFileReceiverPtr;
  }
}
#endif // RAWFILERECEIVER_FWD_H
