// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef VALUEMESSAGEBUILDER_FWD_H
#define VALUEMESSAGEBUILDER_FWD_H
namespace QuickFAST{
  namespace Messages{
    class ValueMessageBuilder;
    /// @brief a smart pointer to a heap allocated ValueMessageBuilder
    /// Note in most cases the ValueMessageBuilder will be stack allocated so this should not be used.
    typedef boost::shared_ptr<ValueMessageBuilder> ValueMessageBuilderPtr;
  }
}
#endif // VALUEMESSAGEBUILDER_FWD_H
