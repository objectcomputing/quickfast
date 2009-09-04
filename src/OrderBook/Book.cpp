// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <OrderBook/OrderBookPch.h>
#include "Book.h"

using namespace QuickFAST;
using namespace QuickFAST::OrderBook;

Book::Book(int depth)
: bid_(depth), ask_(depth), depth_(depth), kind_(MULTIPLE_DEPTH)
{
}

Book::Book(int depth, BookKind kind)
: bid_(depth), ask_(depth), depth_(depth), kind_(kind)
{
}

void Book::addOrder(std::string mdEntryType, uint32 mdPriceLevel, int32 mdEntrySize, Decimal mdEntryPx, uint32 numberOfOrders) {
  ensureBookDepth(mdPriceLevel);
  Order order(numberOfOrders, mdEntrySize, mdEntryPx);
  if (mdEntryType == "0") {
    std::list<Order>::iterator it = walkToMDPriceLevel(bid_, mdPriceLevel);
    bid_.insert(it, order);
    trimToBookDepth(bid_);
  } else if (mdEntryType == "1") {
    std::list<Order>::iterator it = walkToMDPriceLevel(ask_, mdPriceLevel);
    ask_.insert(it, order);
    trimToBookDepth(ask_);
  }
}

void Book::changeOrder(std::string mdEntryType, uint32 mdPriceLevel, int32 mdEntrySize, Decimal mdEntryPx, uint32 numberOfOrders) {
  ensureBookDepth(mdPriceLevel);
  if (mdEntryType == "0") {
    std::list<Order>::iterator it = walkToMDPriceLevel(bid_, mdPriceLevel);
    if (it != bid_.end()) {
      it->changeTo(numberOfOrders, mdEntrySize, mdEntryPx);
    } else {
      bid_.insert(it, Order(numberOfOrders, mdEntrySize, mdEntryPx));
    }
  } else if (mdEntryType == "1") {
    std::list<Order>::iterator it = walkToMDPriceLevel(ask_, mdPriceLevel);
    if (it != ask_.end()) {
      it->changeTo(numberOfOrders, mdEntrySize, mdEntryPx);
    } else {
      ask_.insert(it, Order(numberOfOrders, mdEntrySize, mdEntryPx));
    }
  }
}

void Book::deleteOrder(std::string mdEntryType, uint32 mdPriceLevel, int32 mdEntrySize, Decimal mdEntryPx, uint32 numberOfOrders) {
  ensureBookDepth(mdPriceLevel);
  if (mdEntryType == "0") {
    std::list<Order>::iterator it = bid_.begin();
    for (uint32 i = 0; i < mdPriceLevel - 1 && it != bid_.end(); ++i, ++it);
    if (it != bid_.end()) {
      bid_.erase(it);
    }
  } else if (mdEntryType == "1") {
    std::list<Order>::iterator it = ask_.begin();
    for (uint32 i = 0; i < mdPriceLevel - 1 && it != ask_.end(); ++i, ++it);
    if (it != ask_.end()) {
      ask_.erase(it);
    }
  }
}

std::list<Order>::iterator
Book::walkToMDPriceLevel(std::list<Order>& side, uint32 mdPriceLevel){
  std::list<Order>::iterator it = side.begin();
  for (uint32 i = 0; i < mdPriceLevel - 1; ++i, ++it);
  return it;
}

void Book::addOrder(std::string mdEntryType, uint32 mdPriceLevel, int32 mdEntrySize, Decimal mdEntryPx) {
  ensureBookDepth(mdPriceLevel);
  Order order(0, mdEntrySize, mdEntryPx);
  if (mdEntryType == "0") {
    std::list<Order>::iterator it = walkToMDPriceLevel(bid_, mdPriceLevel);
    bid_.insert(it, order);
    trimToBookDepth(bid_);
  } else if (mdEntryType == "1") {
    std::list<Order>::iterator it = walkToMDPriceLevel(ask_, mdPriceLevel);
    ask_.insert(it, order);
    trimToBookDepth(ask_);
  }
}

void Book::changeOrder(std::string mdEntryType, uint32 mdPriceLevel, int32 mdEntrySize, Decimal mdEntryPx) {
  ensureBookDepth(mdPriceLevel);
  if (mdEntryType == "0") {
    std::list<Order>::iterator it = walkToMDPriceLevel(bid_, mdPriceLevel);
    if (it != bid_.end()) {
      it->changeTo(0, mdEntrySize, mdEntryPx);
    } else {
      bid_.insert(it, Order(0, mdEntrySize, mdEntryPx));
    }
  } else if (mdEntryType == "1") {
    std::list<Order>::iterator it = walkToMDPriceLevel(ask_, mdPriceLevel);
    if (it != ask_.end()) {
      it->changeTo(0, mdEntrySize, mdEntryPx);
    } else {
      ask_.insert(it, Order(0, mdEntrySize, mdEntryPx));
    }
  }
}

void Book::deleteOrder(std::string mdEntryType, uint32 mdPriceLevel, int32 mdEntrySize, Decimal mdEntryPx) {
  ensureBookDepth(mdPriceLevel);
  if (mdEntryType == "0") {
    std::list<Order>::iterator it = bid_.begin();
    for (uint32 i = 0; i < mdPriceLevel - 1 && it != bid_.end(); ++i, ++it);
    if (it != bid_.end()) {
      bid_.erase(it);
    }
  } else if (mdEntryType == "1") {
    std::list<Order>::iterator it = ask_.begin();
    for (uint32 i = 0; i < mdPriceLevel - 1 && it != ask_.end(); ++i, ++it);
    if (it != ask_.end()) {
      ask_.erase(it);
    }
  }
}

void
Book::ensureBookDepth(uint32 depth) {
  if (depth_ < depth) {
    depth_ = depth;
  }
  growSideToDepth(bid_);
  growSideToDepth(ask_);
}

void
Book::growSideToDepth(std::list<Order>& side) {
  uint32 size = side.size();
  if (size < depth_) {
    for (uint32 i = 0; i < depth_ - size; ++i) {
      side.insert(side.end(), Order());
    }
  }
}

void
Book::trimToBookDepth(std::list<Order>& side) {
  uint32 s = side.size();
  if (side.size() <= depth_)  return;
  std::list<Order>::iterator it = walkToMDPriceLevel(side, depth_);
  ++it;
  side.erase(it, side.end());
}
