// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef PCAPFILERECEIVER_FWD_H
#define PCAPFILERECEIVER_FWD_H

namespace QuickFAST{
  namespace Communication{
    class FileReceiver;
    /// @brief smart pointer to a FileReceiver
    typedef boost::shared_ptr<FileReceiver> FileReceiverPtr;
  }
}
#endif // FILERECEIVER_FWD_H
