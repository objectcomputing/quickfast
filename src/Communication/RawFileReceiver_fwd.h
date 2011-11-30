// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef RAWFILERECEIVER_FWD_H
#define RAWFILERECEIVER_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

namespace QuickFAST{
  namespace Communication{
    class RawFileReceiver;
    /// @brief smart pointer to a FileReceiver
    typedef boost::shared_ptr<RawFileReceiver> RawFileReceiverPtr;
  }
}
#endif // RAWFILERECEIVER_FWD_H
