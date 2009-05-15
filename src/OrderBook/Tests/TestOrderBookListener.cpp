#include <OrderBookPch.h>
#include "TestOrderBookListener.h"
#include <Order.h>

using namespace QuickFAST;
using namespace QuickFAST::OrderBook;

void
TestOrderBookListener::bookUpdated(const SecurityID& securityID, QuickFAST::OrderBook::BookPtr book) {
  *output_ << "\nSecurity: securityIDSource: " << securityID.securityIDSource()
    << ", securityID: " << securityID.securityID() << std::endl;
  *output_ << "Book kind: " << book->kind() << std::endl;

  const std::list<Order>& bid = book->bid();
  const std::list<Order>& ask = book->ask();

  *output_ << "\tBids" << std::endl;
  int i = 1;
  for (std::list<Order>::const_iterator it = bid.begin(); it != bid.end(); ++it, ++i) {
    *output_ << "\t\tPriceLevel: " << i
      << ", orderCount: " << it->orderCount()
      << ", quantity: " << it->quantity()
      << ", price: " << it->price() << std::endl;
  }

  *output_ << "\tAsks" << std::endl;
  i = 1;
  for (std::list<Order>::const_iterator it = ask.begin(); it != ask.end(); ++it, ++i) {
    *output_ << "\t\tPriceLevel: " << i
      << ", orderCount: " << it->orderCount()
      << ", quantity: " << it->quantity()
      << ", price: " << it->price() << std::endl;
  }
}