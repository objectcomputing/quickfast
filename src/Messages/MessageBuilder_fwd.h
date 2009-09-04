// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGEBUILDER_FWD_H
#define MESSAGEBUILDER_FWD_H
namespace QuickFAST{
  namespace Messages{
    class MessageBuilder;
    /// @brief Smart pointer to a FieldSet.
    typedef boost::shared_ptr<MessageBuilder> MessageBuilderPtr;
    /// @brief Smart pointer to a const FieldSet.
    typedef boost::shared_ptr<const MessageBuilder> MessageBuilderCPtr;
  }
}
#endif // MESSAGEBUILDER_FWD_H
