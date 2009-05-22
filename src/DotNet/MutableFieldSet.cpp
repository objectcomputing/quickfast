// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include <DotNet/MutableFieldSet.h>
#include <DotNet/FieldIdentity.h>
#include <DotNet/Field.h>
#include <DotNet/StlDotNet.h>

namespace QuickFASTDotNet{
  namespace Messages{


    MutableFieldSet::MutableFieldSet()
      : FieldSet(QuickFAST::Messages::FieldSetCPtr())
      , mutableFieldSetPtr_(QuickFAST::Messages::FieldSetPtr(new TFieldSet(20))) // @TODO get an accurate field count
    {
      spFieldSet_.GetBoostPtr() = mutableFieldSetPtr_.GetBoostPtr();
    }

    MutableFieldSet::MutableFieldSet(unsigned int expectedNumberOfFields)
      : FieldSet(QuickFAST::Messages::FieldSetCPtr())
      , mutableFieldSetPtr_(QuickFAST::Messages::FieldSetPtr(new TFieldSet(expectedNumberOfFields)))
    {
      spFieldSet_.GetBoostPtr() = mutableFieldSetPtr_.GetBoostPtr();
    }

    void MutableFieldSet::SetApplicationType(System::String^ applicationType, System::String^ nameSpace)
    {
      mutableFieldSetPtr_->setApplicationType(StlDotNet::string_cast<std::string>(applicationType),
                                              StlDotNet::string_cast<std::string>(nameSpace));
    }

    void MutableFieldSet::AddField(FieldIdentity^ identity, Field^ newField)
    {
#if 0 // not implemented yet.  Get the decoder working
      mutableFieldSetPtr_->addField(identity->SmartPtr, cast_field(newField));
#endif
    }
  }
}
