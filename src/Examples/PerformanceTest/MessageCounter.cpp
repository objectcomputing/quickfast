// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include "Examples/ExamplesPch.h"
#include "MessageCounter.h"
#include <Messages/Message.h>
#include <Messages/Field.h>
#include <Messages/Sequence.h>
#include <Messages/Group.h>

using namespace QuickFAST;
using namespace Examples;

MessageCounter::MessageCounter()
  : messageCount_(0)
{
}

MessageCounter::~MessageCounter()
{
}

bool
MessageCounter::consumeMessage(Messages::Message & message)
{
  messageCount_ += 1;
  return true;
}

