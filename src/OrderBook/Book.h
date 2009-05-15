// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef BOOK_H
#define BOOK_H
#include <OrderBook_Export.h>
#include <Order.h>
#include <Codecs/MessageConsumer.h>
#include <Messages/Message.h>
#include <Common/Decimal.h>

namespace QuickFAST{
	namespace OrderBook{
    enum BookKind {
      MULTIPLE_DEPTH,
      IMPLIED,
      TOP_OF_BOOK,
      INDICATIVE_PRICES
    };

    class ORDERBOOK_Export Book
    {
    public:
      Book() : bid_(0), ask_(0), depth_(0), kind_(MULTIPLE_DEPTH) {}
      explicit Book(int depth);
      Book(int depth, BookKind kind);

      // Multiple-depth book
      void addOrder(std::string mdEntryType, uint32 mdPriceLevel, int32 mdEntrySize, Decimal mdEntryPx, uint32 numberOfOrders);
      void changeOrder(std::string mdEntryType, uint32 mdPriceLevel, int32 mdEntrySize, Decimal mdEntryPx, uint32 numberOfOrders);
      void deleteOrder(std::string mdEntryType, uint32 mdPriceLevel, int32 mdEntrySize, Decimal mdEntryPx, uint32 numberOfOrders);

      // implied book
      void addOrder(std::string mdEntryType, uint32 mdPriceLevel, int32 mdEntrySize, Decimal mdEntryPx);
      void changeOrder(std::string mdEntryType, uint32 mdPriceLevel, int32 mdEntrySize, Decimal mdEntryPx);
      void deleteOrder(std::string mdEntryType, uint32 mdPriceLevel, int32 mdEntrySize, Decimal mdEntryPx);

      const std::list<Order>& bid() { return bid_; }
      const std::list<Order>& ask() { return ask_; }
      const BookKind& kind() { return kind_; }
    private:
      std::list<Order> bid_;
      std::list<Order> ask_;
      unsigned int depth_;
      BookKind kind_;

      std::list<Order>::iterator walkToMDPriceLevel(std::list<Order>& side, uint32 mdPriceLevel);
      void ensureBookDepth(uint32 depth);
      void growSideToDepth(std::list<Order>& side);
      void trimToBookDepth(std::list<Order>& side);
    };

    typedef boost::shared_ptr<Book> BookPtr;
	}
}

#endif // BOOK_H
