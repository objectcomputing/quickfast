// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldGroup.h"

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
FieldGroup::create(Messages::GroupCPtr group)
{
  return new FieldGroup(group);
}

