// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldGroup.h"
#include <Messages/MessageBuilder.h>
#include <Messages/FieldSet.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldGroup::FieldGroup(Messages::GroupCPtr group)
  : Field(Field::GROUP, true)
  , group_(group)
{
}

FieldGroup::~FieldGroup()
{
}

const Messages::GroupCPtr &
FieldGroup::toGroup() const
{
  return group_;
}

FieldCPtr
FieldGroup::create(Messages::MessageBuilderPtr & builder)
{
  return new FieldGroup(builder->getFieldSet().shared_from_this());
}

FieldCPtr
FieldGroup::create(Messages::GroupCPtr group)
{
  return new FieldGroup(group);
}

