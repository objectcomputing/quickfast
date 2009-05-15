// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ORDERBOOKCONTAINER_H
#define ORDERBOOKCONTAINER_H
#include <OrderBook_Export.h>
#include <Book.h>
#include <OrderBookListener.h>
#include <OrderBookMessageConsumer.h>
#include <SecurityID.h>
#include <Codecs/DataSource.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/SynchronousDecoder.h>

namespace QuickFAST{
  namespace OrderBook{
    class ORDERBOOK_Export OrderBookContainer
    {
    public:
      OrderBookContainer(QuickFAST::Codecs::DataSource& dataSource, QuickFAST::Codecs::TemplateRegistryPtr& templates);
      void start();
      BookPtr monitorBook(const SecurityID& securityID, OrderBookListenerPtr listener);
      BookPtr getBook(const SecurityID& securityID);
      OrderBookListenerPtr getOrderBookListener(const SecurityID& securityID);

    private:
      BookPtr getOrCreateBook(const SecurityID& securityID);
      void addOrderBookListener(const SecurityID& securityID, OrderBookListenerPtr listener);

      typedef std::map<SecurityID, BookPtr> Books;
      typedef Books::iterator BooksIter;
      Books books_;

      std::map<SecurityID, OrderBookListenerPtr> listeners_;

      QuickFAST::Codecs::DataSource& dataSource_;
      QuickFAST::Codecs::TemplateRegistryPtr& templates_;
      QuickFAST::Codecs::SynchronousDecoder<Messages::Message, Codecs::MessageConsumer> decoder_;
      OrderBookMessageConsumer handler_;
    };
  }
}

#endif // ORDERBOOKCONTAINER_H
