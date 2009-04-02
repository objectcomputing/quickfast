// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include "MulticastDecoder.h"

#include <Codecs/XMLTemplateParser.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/Decoder.h>
#include <Codecs/DataSourceBuffer.h>
#include <Codecs/MessageConsumer.h>
#include <Messages/Message.h>

using namespace QuickFAST;
using namespace Codecs;

MulticastDecoder::MulticastDecoder(
  TemplateRegistryPtr templateRegistry,
  const std::string & multicastAddressName,
  const std::string & listenAddressName,
  unsigned short portNumber)
: decoder_(templateRegistry)
, stopping_(false)
, error_(false)
, messageCount_(0)
, messageLimit_(0)
, bufferSize_(5000)
, verboseDecode_(false)
, verboseExecution_(false)
, listenAddress_(boost::asio::ip::address::from_string(listenAddressName))
, multicastAddress_(boost::asio::ip::address::from_string(multicastAddressName))
, endpoint_(listenAddress_, portNumber)
, socket_(ioService_)
{
}

MulticastDecoder::MulticastDecoder(
  TemplateRegistryPtr templateRegistry,
  boost::asio::io_service & ioService,
  const std::string & multicastAddressName,
  const std::string & listenAddressName,
  unsigned short portNumber)
: AsioService(ioService)
, decoder_(templateRegistry)
, messageCount_(0)
, messageLimit_(0)
, bufferSize_(5000)
, verboseDecode_(false)
, verboseExecution_(false)
, listenAddress_(boost::asio::ip::address::from_string(listenAddressName))
, multicastAddress_(boost::asio::ip::address::from_string(multicastAddressName))
, endpoint_(listenAddress_, portNumber)
, socket_(ioService_)
{
}

MulticastDecoder::~MulticastDecoder()
{
}

void
MulticastDecoder::start(
  MessageConsumerPtr consumer)
{
  consumer_ = consumer;

  buffer1_.reset(new unsigned char[bufferSize_]);
  buffer2_.reset(new unsigned char[bufferSize_]);

  socket_.open(endpoint_.protocol());
  socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
  socket_.bind(endpoint_);

  // Join the multicast group.
  socket_.set_option(
    boost::asio::ip::multicast::join_group(multicastAddress_));
  startReceive(&buffer1_, &buffer2_);
}

void
MulticastDecoder::stop()
{
  stopping_ = true;
  // attempt to cancel any requests in progress.
  socket_.cancel();
}

void
MulticastDecoder::startReceive(Buffer * buffer, Buffer * altBuffer)
{
  socket_.async_receive_from(
    boost::asio::buffer(buffer->get(), bufferSize_),
    senderEndpoint_,
    strand_.wrap(
      boost::bind(&MulticastDecoder::handleReceive,
        this,
        boost::asio::placeholders::error,
        buffer,
        boost::asio::placeholders::bytes_transferred,
        altBuffer)
        )
      );
}

void
MulticastDecoder::handleReceive(
  const boost::system::error_code& error,
  Buffer * buffer,
  size_t bytesReceived,
  Buffer * altBuffer)
{
  if(stopping_)
  {
    return;
  }
  if (!error)
  {
    // accept data into the other buffer while we process this buffer
    startReceive(altBuffer, buffer);
    ++messageCount_;
    if(consumer_->wantLog(MessageConsumer::LOG_VERBOSE))
    {
      std::stringstream message;
      message << "Received[" << messageCount_ << "]: " << bytesReceived << " bytes";
      consumer_->logMessage(MessageConsumer::LOG_VERBOSE, message.str());
    }
    try
    {
      DataSourceBuffer source(buffer->get(), bytesReceived);
      Messages::Message message(
        decoder_.getFieldSetCache(),
        decoder_.getTemplateRegistry()->maxFieldCount());
      decoder_.reset();
      decoder_.decodeMessage(source, message);

      if(!consumer_->consumeMessage(message))
      {
        if(consumer_->wantLog(MessageConsumer::LOG_INFO))
        {
          consumer_->logMessage(MessageConsumer::LOG_INFO, "Consumer requested early termination.");
        }
        stopping_ = true;
        socket_.cancel();
        return;
      }
    }
    catch (const std::exception &ex)
    {
      if(!consumer_->reportDecodingError(ex.what()))
      {
        error_ = true;
        errorMessage_ =
        stopping_ = true;
        socket_.cancel();
      }
    }
    if(messageCount_ > messageLimit_ && messageLimit_ != 0)
    {
      stopping_ = true;
      socket_.cancel();
    }
  }
  else
  {
    if(!consumer_->reportCommunicationError(error.message()))
    {
      error_ = true;
      errorMessage_ = error.message();
      stopping_ = true;
      socket_.cancel();
    }
  }
}

