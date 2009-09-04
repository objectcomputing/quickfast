// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <OrderBook/OrderBookPch.h>
#include "SecurityID.h"

namespace QuickFAST{
  namespace OrderBook{
    bool operator<(const SecurityID& lhs, const SecurityID& rhs) {
      if (lhs.securityIDSource_ != rhs.securityIDSource_) {
        return lhs.securityIDSource_ < rhs.securityIDSource_;
      } else {
        return lhs.securityID_ < rhs.securityID_;
      }
    }
  }
}
