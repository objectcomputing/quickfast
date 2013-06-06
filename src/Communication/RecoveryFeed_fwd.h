// Copyright (c) 2009, 2010, 2011, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef RECOVERYFEED_FWD_H
#define RECOVERYFEED_FWD_H

namespace QuickFAST
{
  namespace Communication
  {
    /// @brief A class to assist with gap recovery.
    class RecoveryFeed;
    /// @brief A smart pointer to a recovery feed.
    typedef boost::shared_ptr<RecoveryFeed> RecoveryFeedPtr;
  }
}

#endif // RECOVERYFEED_FWD_H
