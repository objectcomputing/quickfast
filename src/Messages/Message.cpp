// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "Message.h"
#include <Messages/Field.h>
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

Message::Message(boost::shared_ptr<FieldSet::BufferCache> cache, size_t expectedNumberOfFields)
: FieldSet(cache, expectedNumberOfFields)
{
  applicationType_ = "any";
}

//bool
//Message::isPresent(const std::string & name) const
//{
//  for(size_t index = 0; index < fields_.size(); ++index)
//  {
//    if(name == fields_[index].name())
//    {
//      return fields_[index].getField()->isDefined();
//    }
//  }
//  return false;
//}
//
//void
//Message::addField(const FieldIdentity & identity, FieldCPtr & value)
//{
//  fields_.push_back(MessageField(identity, value));
//}
//
//bool
//Message::getField(const std::string &name, FieldCPtr & value) const
//{
//  for(size_t index = 0; index < fields_.size(); ++index)
//  {
//    if(name == fields_[index].name())
//    {
//      value = fields_[index].getField();
//      return value->isDefined();
//    }
//  }
//  return false;
//}
//
//bool
//Message::getIdentity(const std::string &name, FieldIdentity & identity) const
//{
//  for(size_t index = 0; index < fields_.size(); ++index)
//  {
//    if(name == fields_[index].name())
//    {
//      identity = fields_[index].getIdentity();
//      return true;
//    }
//  }
//  return false;
//}
