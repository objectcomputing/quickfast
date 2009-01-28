// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGE_FWD_H
#define MESSAGE_FWD_H
namespace QuickFAST{
  namespace Messages{
    class Message;
    /// @brief Smart pointer to a Message.
    typedef boost::shared_ptr<Message> MessagePtr;
    /// @brief Smart pointer to a const Message.
    typedef boost::shared_ptr<const Message> MessageCPtr;
  }
}
#endif // MESSAGE_FWD_H
