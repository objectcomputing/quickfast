// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include <DotNet/FieldSet.h>
#include <DotNet/Field.h>
#include <DotNet/StlDotNet.h>
#include <DotNet/FieldIdentity.h>

using namespace System;

using namespace QuickFASTDotNet;
using namespace Messages;

FieldSet::FieldSet(const QuickFAST::Messages::FieldSetCPtr& fieldSet)
  :spFieldSet_(fieldSet)
{
  // Because of the way the derived class constructor for MutableFieldSet works,
  // this class may *not* depend on spFieldSet_ being non-NULL here.
}

System::String ^
FieldSet::GetNameIndexed(int index)
{
  return gcnew System::String(spFieldSet_.GetRef()[index].name().c_str());
}

Field ^
FieldSet::GetFieldIndexed(int index)
{
  /*
  QuickFAST::Messages::FieldCPtr cppField;
  QuickFAST::Messages::Field::FieldType type;
  std::string name;
  spFieldSet_->getFieldInfo(index, name, type, cppField);
  return gcnew Field(name, type, cppField);
*/
  return gcnew Field(spFieldSet_.GetBoostPtr(), index);
//  return gcnew Field(spFieldSet_.GetRef()[index].getField());
}


#if 0
FieldSet::TKeyValuePair FieldSet::default::get(String^ fieldName)
{
    std::string stdFieldName = StlDotNet::string_cast<std::string>(fieldName);

    QuickFAST::Messages::FieldCPtr fieldPtr;
    spFieldSet_->getField(stdFieldName, fieldPtr);

    QuickFAST::Messages::FieldIdentityCPtr fieldIdentity;
    if(!spFieldSet_->getIdentity(stdFieldName, fieldIdentity))
    {
      throw gcnew System::Collections::Generic::KeyNotFoundException(System::String::Format("Field name '{0}' not found.", fieldName));
    }

    return TKeyValuePair(gcnew FieldIdentity(fieldIdentity), gcnew Field(fieldPtr));
}
#endif

String^ FieldSet::ApplicationType::get()
{
  return StlDotNet::string_cast(spFieldSet_->getApplicationType());
}

bool FieldSet::IsPresent(System::String^ name)
{
  std::string stdNameStr;
  StlDotNet::MarshalString(name, stdNameStr);

  return spFieldSet_->isPresent(stdNameStr);
}

/*
Field^ FieldSet::GetField(System::String^ name)
{
  QuickFAST::Messages::FieldCPtr field;
  spFieldSet_->getField(StlDotNet::string_cast<std::string>(name), field);
  return gcnew Field(field);
}
*/

FieldIdentity^ FieldSet::GetIdentity(System::String^ name)
{
  QuickFAST::Messages::FieldIdentityCPtr spFieldIdentity;
  spFieldSet_->getIdentity(StlDotNet::string_cast<std::string>(name), spFieldIdentity);
  return gcnew FieldIdentity(spFieldIdentity);
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
#if 0
FieldSet::TKeyValuePair FieldSet::FieldEnumerator::GenericCurrent::get()
{
  return TKeyValuePair(gcnew FieldIdentity(itHolder_->it->getIdentity()), gcnew Field(itHolder_->it->getField()));
}
#endif

Object^ FieldSet::FieldEnumerator::Current::get()
{
  return GenericCurrent::get();
}

bool FieldSet::FieldEnumerator::MoveNext()
{
  return (++itHolder_->it != itHolder_->end);
}

void FieldSet::FieldEnumerator::Reset()
{
  throw gcnew System::NotSupportedException();
}
