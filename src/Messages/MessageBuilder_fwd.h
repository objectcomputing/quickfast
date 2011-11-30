// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGEBUILDER_FWD_H
#define MESSAGEBUILDER_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

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
