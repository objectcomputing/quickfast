// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <OrderBook/OrderBookPch.h>
#include "OrderBookMessageConsumer.h"

#include <OrderBook/Book.h>
#include <OrderBookContainer.h>
#include <OrderBookListener.h>
#include <Messages/Field.h>
#include <Messages/Sequence.h>

using namespace QuickFAST;
using namespace QuickFAST::OrderBook;

bool
OrderBookMessageConsumer::consumeMessage(Messages::Message & message)
{
  const std::string& messageType = getMessageType(message);
  if (messageType == "d") {
    // Security Definition
    handleSecurityDefinition(message);
  } else if (messageType == "X") {
    // Market Data - Incremental Refresh
    handleIncrementalRefresh(message);
  }

  return true;
}

void
OrderBookMessageConsumer::decodingStarted()
{
}

void
OrderBookMessageConsumer::decodingStopped()
{
}

void
OrderBookMessageConsumer::handleSecurityDefinition(const Messages::Message& message) {
}

bool
OrderBookMessageConsumer::wantLog(unsigned short level)
{
  return true;
}

bool
OrderBookMessageConsumer::logMessage(unsigned short level, const std::string & logMessage)
{
  return true;
}

bool
OrderBookMessageConsumer::reportDecodingError(const std::string & errorMessage)
{
  return true;
}

bool
OrderBookMessageConsumer::reportCommunicationError(const std::string & errorMessage)
{
  return true;
}

void
OrderBookMessageConsumer::handleIncrementalRefresh(const Messages::Message& message) {
  // Process all MDEntries in the message
  Messages::FieldCPtr mdEntriesField;
  message.getField("MDEntries", mdEntriesField);
  Messages::SequenceCPtr seq = mdEntriesField->toSequence();
  for (Messages::Sequence::const_iterator it = seq->begin(); it != seq->end(); ++it) {
    Messages::FieldSetCPtr mdEntry = *it;
    handleMDEntry(mdEntry);
  }
  notifyOrderBookListeners();
}

void
OrderBookMessageConsumer::handleMDEntry(const Messages::FieldSetCPtr& mdEntry) {
  const SecurityID key = getSecurityID(mdEntry);
  const BookKind kind = getBookKind(mdEntry);
  BookPtr book = orderBookContainer_.getBook(key);
  switch (kind) {
    case MULTIPLE_DEPTH:
      handleMultipleDepthBookAction(book, mdEntry);
      break;
    case IMPLIED:
      handleImpliedBookAction(book, mdEntry);
      break;
    case TOP_OF_BOOK:
      handleTopOfBookAction(book, mdEntry);
      break;
    case INDICATIVE_PRICES:
      handleIndicativePricesBookAction(book, mdEntry);
      break;
    default:
      // no-op
      break;
  }
  updatedSecurities_.insert(key);
}

const SecurityID
OrderBookMessageConsumer::getSecurityID(const Messages::FieldSetCPtr& mdEntry) {
  Messages::FieldCPtr securityIDSourceField;
  mdEntry->getField("SecurityIDSource", securityIDSourceField);
  uint32 securityIDSource = securityIDSourceField->toUInt32();
  Messages::FieldCPtr securityIDField;
  mdEntry->getField("SecurityID", securityIDField);
  uint32 securityID = securityIDField->toUInt32();
  return SecurityID(securityIDSource, securityID);
}

const BookKind
OrderBookMessageConsumer::getBookKind(const Messages::FieldSetCPtr& mdEntry) {
  if (mdEntry->isPresent("QuoteCondition")) {
    Messages::FieldCPtr quoteConditionField;
    mdEntry->getField("QuoteCondition", quoteConditionField);
    std::string quoteCondition = quoteConditionField->toAscii();
    if (quoteCondition == "K") {
      return IMPLIED;
    }
  }
  //Messages::FieldCPtr mdUpdateActionField;
  //mdEntry->getField("MDUpdateAction", mdUpdateActionField);
  //uint32 mdUpdateAction = mdUpdateActionField->toUInt32();
  uint32 mdUpdateAction = getMDUpdateAction(mdEntry);
  if (mdUpdateAction == 0 || mdUpdateAction == 1 || mdUpdateAction == 2) {
    return MULTIPLE_DEPTH;
  } else if (mdUpdateAction == 5) {
    if (mdEntry->isPresent("MDQuoteType")) {
      Messages::FieldCPtr mdQuoteTypeField;
      mdEntry->getField("MDQuoteType", mdQuoteTypeField);
      int32 mdQuoteType = mdQuoteTypeField->toInt32();
      if (mdQuoteType == 0) {
        return INDICATIVE_PRICES;
      }
    }
    return TOP_OF_BOOK;
  } else {
    // defaults to multiple depth
    return MULTIPLE_DEPTH;
  }
}
const StringBuffer&
OrderBookMessageConsumer::getMessageType(const Messages::Message& message) {
  Messages::FieldCPtr field;
  message.getField("MessageType", field);
  return field->toAscii();
}

const uint32
OrderBookMessageConsumer::getMDUpdateAction(const Messages::FieldSetCPtr& mdEntry) {
  Messages::FieldCPtr field;
  mdEntry->getField("MDUpdateAction", field);
  return field->toUInt32();
}

const StringBuffer&
OrderBookMessageConsumer::getMDEntryType(const Messages::FieldSetCPtr& mdEntry) {
  Messages::FieldCPtr field;
  mdEntry->getField("MDEntryType", field);
  return field->toAscii();
}

const uint32
OrderBookMessageConsumer::getMDPriceLevel(const Messages::FieldSetCPtr& mdEntry) {
  Messages::FieldCPtr field;
  mdEntry->getField("MDPriceLevel", field);
  if (field) {
    return field->toUInt32();
  } else {
    return 0;
  }
}

const int32
OrderBookMessageConsumer::getMDEntrySize(const Messages::FieldSetCPtr& mdEntry) {
  Messages::FieldCPtr field;
  mdEntry->getField("MDEntrySize", field);
  return field->toInt32();
}

const Decimal
OrderBookMessageConsumer::getMDEntryPx(const Messages::FieldSetCPtr& mdEntry) {
  Messages::FieldCPtr field;
  mdEntry->getField("MDEntryPx", field);
  return field->toDecimal();
}

const uint32
OrderBookMessageConsumer::getNumberOfOrders(const Messages::FieldSetCPtr& mdEntry) {
  Messages::FieldCPtr field;
  mdEntry->getField("NumberOfOrders", field);
  return field->toUInt32();
}

void
OrderBookMessageConsumer::handleMultipleDepthBookAction(BookPtr book, const Messages::FieldSetCPtr& mdEntry) {
  uint32 mdPriceLevel = getMDPriceLevel(mdEntry);
  if (mdPriceLevel == 0) {
    // Not a message that contains book update
    return;
  }

  uint32 mdUpdateAction = getMDUpdateAction(mdEntry);
  std::string mdEntryType = getMDEntryType(mdEntry);
  int32 mdEntrySize = getMDEntrySize(mdEntry);
  Decimal mdEntryPx = getMDEntryPx(mdEntry);
  uint32 numberOfOrders = getNumberOfOrders(mdEntry);

  switch (mdUpdateAction) {
    case 0:
      book->addOrder(mdEntryType, mdPriceLevel, mdEntrySize, mdEntryPx, numberOfOrders);
      break;
    case 1:
      book->changeOrder(mdEntryType, mdPriceLevel, mdEntrySize, mdEntryPx, numberOfOrders);
      break;
    case 2:
      book->deleteOrder(mdEntryType, mdPriceLevel, mdEntrySize, mdEntryPx, numberOfOrders);
      break;
    default:
      // no-op
      break;
  }
}

void
OrderBookMessageConsumer::handleImpliedBookAction(BookPtr book, const Messages::FieldSetCPtr& mdEntry) {
  uint32 mdUpdateAction = getMDUpdateAction(mdEntry);
  std::string mdEntryType = getMDEntryType(mdEntry);
  uint32 mdPriceLevel = getMDPriceLevel(mdEntry);
  int32 mdEntrySize = getMDEntrySize(mdEntry);
  Decimal mdEntryPx = getMDEntryPx(mdEntry);

  switch (mdUpdateAction) {
    case 0:
      book->addOrder(mdEntryType, mdPriceLevel, mdEntrySize, mdEntryPx);
      break;
    case 1:
      book->changeOrder(mdEntryType, mdPriceLevel, mdEntrySize, mdEntryPx);
      break;
    case 2:
      book->deleteOrder(mdEntryType, mdPriceLevel, mdEntrySize, mdEntryPx);
      break;
    default:
      // no-op
      break;
  }
}

void
OrderBookMessageConsumer::handleTopOfBookAction(BookPtr book, const Messages::FieldSetCPtr& mdEntry) {
  std::string mdEntryType = getMDEntryType(mdEntry);
  uint32 mdPriceLevel = getMDPriceLevel(mdEntry);
  int32 mdEntrySize = getMDEntrySize(mdEntry);
  Decimal mdEntryPx = getMDEntryPx(mdEntry);
  uint32 numberOfOrders = getNumberOfOrders(mdEntry);
  book->changeOrder(mdEntryType, mdPriceLevel, mdEntrySize, mdEntryPx, numberOfOrders);
}

void
OrderBookMessageConsumer::handleIndicativePricesBookAction(BookPtr book, const Messages::FieldSetCPtr& mdEntry) {
  std::string mdEntryType = getMDEntryType(mdEntry);
  uint32 mdPriceLevel = getMDPriceLevel(mdEntry);
  int32 mdEntrySize = getMDEntrySize(mdEntry);
  Decimal mdEntryPx = getMDEntryPx(mdEntry);
  uint32 numberOfOrders = getNumberOfOrders(mdEntry);

  book->changeOrder(mdEntryType, mdPriceLevel, mdEntrySize, mdEntryPx, numberOfOrders);
}

void
OrderBookMessageConsumer::notifyOrderBookListeners() {
  for (std::set<SecurityID>::iterator it = updatedSecurities_.begin(); it != updatedSecurities_.end(); ++it) {
    SecurityID securityID = (*it);
    BookPtr book = orderBookContainer_.getBook(securityID);
    OrderBookListenerPtr listener = orderBookContainer_.getOrderBookListener(securityID);
    if (listener) {
      listener->bookUpdated(securityID, book);
    }
  }
  updatedSecurities_.clear();
}

