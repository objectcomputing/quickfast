// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef SIDE_H
#define SIDE_H
#include <OrderBook_Export.h>
#include <Common/Types.h>
#include <Common/Decimal.h>

namespace QuickFAST{
  namespace OrderBook{
    class Order {
    public:
      Order() : orderCount_(0), quantity_(0), price_(0) {}
      Order(uint32 orderCount, int32 quantity, Decimal price)
        : orderCount_(orderCount), quantity_(quantity), price_(price) {}

      void changeTo(uint32 orderCount, int32 quantity, Decimal price);

      const uint32& orderCount() const { return orderCount_; }
      const int32& quantity() const { return quantity_; }
      const Decimal& price() const { return price_; }

    private:
      uint32 orderCount_;
      int32 quantity_;
      Decimal price_;
    };
  }
}

#endif // SIDE_H
