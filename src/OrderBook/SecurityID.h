// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef SECURITYID_H
#define SECURITYID_H
#include <OrderBook_Export.h>
#include <Common/Types.h>

namespace QuickFAST{
  namespace OrderBook{
    class ORDERBOOK_Export SecurityID {
    public:
      SecurityID() : securityIDSource_(0), securityID_(0) {}
      SecurityID(uint32 securityIDSource, uint32 securityID)
        : securityIDSource_(securityIDSource), securityID_(securityID) {}

      SecurityID& operator=(const SecurityID& rhs) {
        securityIDSource_ = rhs.securityIDSource_;
        securityID_ = rhs.securityID_;
        return *this;
      }

      uint32 securityIDSource() const { return securityIDSource_; }
      uint32 securityID() const { return securityID_; }

      friend bool operator<(const SecurityID& lhs, const SecurityID& rhs);

    private:
      uint32 securityIDSource_;
      uint32 securityID_;
    };

    bool operator<(const SecurityID& lhs, const SecurityID& rhs);
  }
}


#endif // SECURITYID_H