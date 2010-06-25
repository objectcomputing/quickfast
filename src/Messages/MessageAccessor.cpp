// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "MessageAccessor.h"
#include <Messages/FieldSet.h>
#include <Messages/Sequence.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

MessageAccessor::MessageAccessor(template_id_t tid)
: tid_(tid)
{
}
MessageAccessor::~MessageAccessor()
{
}

bool
MessageAccessor::pickTemplate(template_id_t & tid)
{
  if(tid_ == template_id_t(-1))
  {
    return false;
  }
  tid = tid_;
  tid_ = template_id_t(-1);
  return true;
}


void
MessageAccessor::endGroup(const FieldIdentity & identity, const MessageAccessor * groupAccessor)const
{
}

void
MessageAccessor::endSequence(const FieldIdentity & identity)const
{
}

void
MessageAccessor::endSequenceEntry(const FieldIdentity & identity, size_t index, const MessageAccessor * entryAccessor)const
{
}
