// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef PCAPFILERECEIVER_FWD_H
#define PCAPFILERECEIVER_FWD_H

namespace QuickFAST{
  namespace Communication{
    class PCapFileReceiver;
    /// @brief smart pointer to a FileReceiver
    typedef boost::shared_ptr<PCapFileReceiver> PCapFileReceiverPtr;
  }
}
#endif // PCAPFILERECEIVER_FWD_H
