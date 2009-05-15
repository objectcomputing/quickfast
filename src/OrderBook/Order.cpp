// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <OrderBookPch.h>
#include "Order.h"

using namespace QuickFAST;
using namespace QuickFAST::OrderBook;

void
Order::changeTo(QuickFAST::uint32 orderCount, QuickFAST::int32 quantity, QuickFAST::Decimal price) {
  orderCount_ = orderCount;
  quantity_ = quantity;
  price_ = price;
}

