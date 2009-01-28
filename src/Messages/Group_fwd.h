// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef GROUP_FWD_H
#define GROUP_FWD_H
#include <Messages/FieldSet_fwd.h>
namespace QuickFAST{
  namespace Messages{
//    class Group;
    /// @brief A group is simply a type of FieldSet
    typedef FieldSet Group;

    /// @brief Smart pointer to a Group.
    typedef boost::shared_ptr<Group> GroupPtr;
    /// @brief Smart pointer to a const Group.
    typedef boost::shared_ptr<const Group> GroupCPtr;
  }
}

#endif // GROUP_FWD_H
