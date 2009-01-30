// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGECONSUMER_FWD_H
#define MESSAGECONSUMER_FWD_H
namespace QuickFAST{
  namespace Codecs{
    class MessageConsumer;
    /// @brief A smart pointer to a MessageConsumer.
    typedef boost::shared_ptr<MessageConsumer> MessageConsumerPtr;
  }
}
#endif /* MESSAGECONSUMER_FWD_H */
