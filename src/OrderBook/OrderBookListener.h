// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ORDERBOOKLISTENER_H
#define ORDERBOOKLISTENER_H
#include <OrderBook/OrderBook_Export.h>
#include <OrderBook/Book.h>
#include <OrderBook/SecurityID.h>

namespace QuickFAST{
  namespace OrderBook{
    class ORDERBOOK_Export OrderBookListener
    {
    public:
      virtual void bookUpdated(
        const SecurityID& securityID,
        BookPtr book) = 0;
    };

    typedef boost::shared_ptr<OrderBookListener> OrderBookListenerPtr;
  }
}

#endif // ORDERBOOKLISTENER_H
