// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include "FieldSet.h"
#include "Field.h"
#include "StlDotNet.h"
#include "FieldIdentity.h"

using namespace System;

namespace QuickFASTDotNet{
  namespace Messages {

    FieldSet::FieldSet()
      :spFieldSet_(QuickFAST::Messages::FieldSetPtr(new QuickFAST::Messages::FieldSet(20))) // @TODO get an accurate field count
      ,isReadOnly_(false)
    {
    }

    FieldSet::FieldSet(const QuickFAST::Messages::FieldSet& fieldSet)
      :spFieldSet_(QuickFAST::Messages::FieldSetPtr(new QuickFAST::Messages::FieldSet(fieldSet)))
      ,isReadOnly_(true)
    {
    }

    FieldSet::FieldSet(const QuickFAST::Messages::FieldSetPtr& fieldSet)
      :spFieldSet_(fieldSet)
      ,isReadOnly_(false)
    {
    }

    FieldSet::TKeyValuePair FieldSet::default::get(String^ fieldName)
    {
        std::string stdFieldName = StlDotNet::string_cast<std::string>(fieldName);

        QuickFAST::Messages::FieldCPtr fieldPtr;
        spFieldSet_->getField(stdFieldName, fieldPtr);

        QuickFAST::Messages::FieldIdentity fieldIdentity;
        if(!spFieldSet_->getIdentity(stdFieldName, fieldIdentity))
        {
          throw gcnew System::Collections::Generic::KeyNotFoundException(System::String::Format("Field name '{0}' not found.", fieldName));
        }

        return TKeyValuePair(gcnew FieldIdentity(fieldIdentity), cast_field(fieldPtr));
    }

    String^ FieldSet::ApplicationType::get()
    {
      return StlDotNet::string_cast(spFieldSet_->getApplicationType());
    }

    void FieldSet::SetApplicationType(System::String^ applicationType, System::String^ nameSpace)
    {
      spFieldSet_->setApplicationType(StlDotNet::string_cast<std::string>(applicationType),
                                      StlDotNet::string_cast<std::string>(nameSpace));     
    }

    bool FieldSet::IsPresent(System::String^ name)
    {
      std::string stdNameStr;
      StlDotNet::MarshalString(name, stdNameStr);

      return spFieldSet_->isPresent(stdNameStr);
    }

    void FieldSet::AddField(FieldIdentity^ identity, Field^ newField)
    {
      spFieldSet_->addField(identity->Ref, cast_field(newField));
    }

    Field^ FieldSet::GetField(System::String^ name)
    {
      QuickFAST::Messages::FieldCPtr field;
      spFieldSet_->getField(StlDotNet::string_cast<std::string>(name), field);
      return cast_field(field);
    }

    FieldIdentity^ FieldSet::GetIdentity(System::String^ name)
    {
      FieldIdentity^ retVal = gcnew FieldIdentity();
      spFieldSet_->getIdentity(StlDotNet::string_cast<std::string>(name), retVal->Ref);
      return retVal;
    }

    bool FieldSet::IsReadOnly::get()
    {
      return isReadOnly_;
    }

    void FieldSet::Add(TKeyValuePair item)
    {
      throw gcnew System::NotImplementedException();
    }

    void FieldSet::Clear()
    {
      throw gcnew System::NotImplementedException();
    }

    bool FieldSet::Contains(TKeyValuePair item)
    {
      throw gcnew System::NotImplementedException();
    }

    void FieldSet::CopyTo(array<TKeyValuePair>^ array, 	int arrayIndex)
    {
      throw gcnew System::NotImplementedException();
    }

    bool FieldSet::Remove(TKeyValuePair item)
    {
      throw gcnew System::NotImplementedException();
    }

    FieldSet::TKeyValuePair FieldSet::FieldEnumerator::GenericCurrent::get()
    {
      return TKeyValuePair(gcnew FieldIdentity(itHolder_->it->getIdentity()), cast_field(itHolder_->it->getField()));
    }

    Object^ FieldSet::FieldEnumerator::Current::get()
    {
      return GenericCurrent::get();
    }

    bool FieldSet::FieldEnumerator::MoveNext()
    {
      return (++itHolder_->it == itHolder_->end)? false: true;
    }

    void FieldSet::FieldEnumerator::Reset()
    {
      throw gcnew System::NotSupportedException();
    }
  }
}
