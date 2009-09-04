// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ORDERBOOKMESSAGECONSUMER_H
#define ORDERBOOKMESSAGECONSUMER_H
#include <OrderBook/OrderBook_Export.h>
#include <OrderBook/OrderBookContainer_fwd.h>
#include <OrderBook/SecurityID.h>
#include <OrderBook/Book.h>
#include <Codecs/MessageConsumer.h>
#include <Messages/Message.h>

namespace QuickFAST{
  namespace OrderBook{
    class ORDERBOOK_Export OrderBookMessageConsumer
      : public QuickFAST::Codecs::MessageConsumer
    {
    public:
      OrderBookMessageConsumer(OrderBookContainer& orderBookContainer)
        : orderBookContainer_(orderBookContainer), updatedSecurities_()
      {
      }

      ////////////////////////////
      // Implement MessageConsumer
      virtual bool consumeMessage(
        QuickFAST::Messages::Message & message);
      virtual void decodingStarted();
      virtual void decodingStopped();
      ///////////////////////
      /// Implement Logger
      virtual bool wantLog(
        unsigned short level);
      virtual bool logMessage(
        unsigned short level,
        const std::string & logMessage);
      virtual bool reportDecodingError(
        const std::string & errorMessage);
      virtual bool reportCommunicationError(
        const std::string & errorMessage);

    private:
      void handleIncrementalRefresh(const Messages::Message& message);
      void handleSecurityDefinition(const Messages::Message& message);
      void handleMDEntry(const Messages::FieldSetCPtr& mdEntry);
      const SecurityID getSecurityID(const Messages::FieldSetCPtr& mdEntry);
      const BookKind getBookKind(const Messages::FieldSetCPtr& mdEntry);
      const StringBuffer& getMessageType(const Messages::Message& message);
      const uint32 getMDUpdateAction(const Messages::FieldSetCPtr& mdEntry);
      const StringBuffer& getMDEntryType(const Messages::FieldSetCPtr& mdEntry);
      const uint32 getMDPriceLevel(const Messages::FieldSetCPtr& mdEntry);
      const int32 getMDEntrySize(const Messages::FieldSetCPtr& mdEntry);
      const Decimal getMDEntryPx(const Messages::FieldSetCPtr& mdEntry);
      const uint32 getNumberOfOrders(const Messages::FieldSetCPtr& mdEntry);

      void handleMultipleDepthBookAction(BookPtr book, const Messages::FieldSetCPtr& mdEntry);
      void handleImpliedBookAction(BookPtr book, const Messages::FieldSetCPtr& mdEntry);
      void handleTopOfBookAction(BookPtr book, const Messages::FieldSetCPtr& mdEntry);
      void handleIndicativePricesBookAction(BookPtr book, const Messages::FieldSetCPtr& mdEntry);
      void notifyOrderBookListeners();

      OrderBookContainer& orderBookContainer_;
      std::set<SecurityID> updatedSecurities_;
    };
  }
}

#endif // ORDERBOOKMESSAGECONSUMER_H
