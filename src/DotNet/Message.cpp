// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include <DotNet/Message.h>

namespace QuickFASTDotNet {
  namespace Messages {

    Message::Message():
        pMessage_(new QuickFAST::Messages::Message(20)),        /// @TODO Get an accurate field count
        FieldSet(QuickFAST::Messages::FieldSetPtr(pMessage_))
    {

    }

    Message::Message(unsigned int expectedNumberOfFields):
        pMessage_(new QuickFAST::Messages::Message(expectedNumberOfFields)),
        FieldSet(QuickFAST::Messages::FieldSetPtr(pMessage_))
    {

    }
  }
}
