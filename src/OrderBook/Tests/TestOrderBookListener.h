// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef TESTORDERBOOKLISTENER_H
#define TESTORDERBOOKLISTENER_H
#include <OrderBookPch.h>
#include <OrderBookListener.h>
#include <Book.h>
#include <SecurityID.h>

class TestOrderBookListener : public QuickFAST::OrderBook::OrderBookListener {
public:
  TestOrderBookListener(std::ostream* output) : output_(output) {}
  virtual void bookUpdated(const QuickFAST::OrderBook::SecurityID& securityID, QuickFAST::OrderBook::BookPtr book);

private:
  std::ostream* output_;
};

#endif // TESTORDERBOOKLISTENER_H