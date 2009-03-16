// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include "FieldSet.h"

namespace QuickFASTDotNet{
  namespace Messages{

    [System::Diagnostics::DebuggerDisplay("Field count = {Count}")]
    [System::Diagnostics::DebuggerTypeProxy(FieldSet::DbView::typeid)]
    public ref class Message: public FieldSet
    {
    public:
      typedef QuickFAST::Messages::Message TMessage;

      Message();
      Message(unsigned int expectedNumberOfFields);

    internal:

      property TMessage& MessageRef
      {
        TMessage& get(){ return *pMessage_;}
      }

    private:
      //The lifetime of pMessage is managed in the base class.
      TMessage* pMessage_;
    };
  }
}
