// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <OrderBookPch.h>
#include "OrderBookContainer.h"

using namespace QuickFAST;
using namespace QuickFAST::OrderBook;

OrderBookContainer::OrderBookContainer(Codecs::DataSource& dataSource, Codecs::TemplateRegistryPtr& templates)
  : dataSource_(dataSource)
  , templates_(templates)
  , handler_(OrderBookMessageConsumer(*this))
  , decoder_(templates_)

{
  //Codecs::SynchronousDecoder<Messages::Message, Codecs::MessageConsumer> decoder(templates_);

	decoder_.setStrict(false);
	decoder_.setResetOnMessage(false);
}

void
OrderBookContainer::start() {
	decoder_.decode(dataSource_, handler_);
}

BookPtr
OrderBookContainer::monitorBook(const SecurityID& securityID, OrderBookListenerPtr listener) {
  addOrderBookListener(securityID, listener);
  return getOrCreateBook(securityID);
}

BookPtr
OrderBookContainer::getBook(const SecurityID& securityID) {
  return getOrCreateBook(securityID);
}

BookPtr
OrderBookContainer::getOrCreateBook(const SecurityID& securityID) {
  BooksIter it = books_.find(securityID);
  if (it == books_.end()) {
    books_[securityID] = BookPtr(new Book());
    it = books_.find(securityID);
  }
  return it->second;
}

void
OrderBookContainer::addOrderBookListener(const SecurityID& securityID, OrderBookListenerPtr listener) {
  listeners_[securityID] = listener;
}

OrderBookListenerPtr
OrderBookContainer::getOrderBookListener(const SecurityID& securityID) {
  return listeners_[securityID];
}
