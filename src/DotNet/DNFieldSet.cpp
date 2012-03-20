// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "QuickFASTDotNetPch.h"
#include "DNFieldSet.h"
#include <DotNet/ImplFieldSet.h>
#include <DotNet/ImplField.h>
#include <DotNet/StringConversion.h>

using namespace QuickFAST;
using namespace DotNet;

DNFieldSet::DNFieldSet(ImplFieldSet & impl)
  : impl_(&impl)
{
}

DNFieldSet::~DNFieldSet()
{
  impl_ = 0;
}

void
DNFieldSet::clear()
{
  impl_ = 0;
}

int
DNFieldSet::Count::get()
{
  if(impl_ == 0)
  {
    throw std::logic_error("Access Count property on cleared DNFieldSet");
  }
  return int(impl_->size());
}

int
DNFieldSet::findIndexByName(System::String ^ fieldName)
{
  if(impl_ == 0)
  {
    throw std::logic_error("Call findIndexByName on cleared DNFieldSet");
  }
  std::string name;
  assignString(name, fieldName);
  size_t size = impl_->size();
  for(size_t index = 0; index < size; ++index)
  {
    ImplField & field = (*impl_)[index];
    if(name == field.localName())
    {
      return static_cast<int>(index);
    }
  }
  return -1;
}

int
DNFieldSet::findIndexByQualifiedName(System::String ^ fieldName, System::String ^ fieldNamespace)
{
  if(impl_ == 0)
  {
    throw std::logic_error("Call findIndexByQualifiedName on cleared DNFieldSet");
  }
  std::string name;
  assignString(name, fieldName);
  std::string ns;
  assignString(ns, fieldNamespace);
  size_t size = impl_->size();
  for(size_t index = 0; index < size; ++index)
  {
    ImplField & field = (*impl_)[index];
    if(name == field.localName() && ns == field.fieldNamespace())
    {
      return static_cast<int>(index);
    }
  }
  return -1;
}


DNField ^
DNFieldSet::getField(int index)
{
  if(impl_ == 0)
  {
    throw std::logic_error("Call getField on cleared DNFieldSet");
  }
  return gcnew DNField((*impl_)[size_t(index)]);
}

DNField ^
DNFieldSet::findFieldByName(System::String ^ fieldName)
{
  if(impl_ == 0)
  {
    throw std::logic_error("Call getFieldByName on cleared DNFieldSet");
  }
  std::string name;
  assignString(name, fieldName);
  size_t size = impl_->size();
  for(size_t index = 0; index < size; ++index)
  {
    ImplField & field = (*impl_)[index];
    if(name == field.localName())
    {
      return gcnew DNField(field);
    }
  }
  return nullptr;
}

DNField ^
DNFieldSet::findFieldByQualifiedName(System::String ^ fieldName, System::String ^ fieldNamespace)
{
  if(impl_ == 0)
  {
    throw std::logic_error("Call getFieldByName on cleared DNFieldSet");
  }
  std::string name;
  assignString(name, fieldName);
  std::string ns;
  assignString(ns, fieldNamespace);
  size_t size = impl_->size();
  for(size_t index = 0; index < size; ++index)
  {
    ImplField & field = (*impl_)[index];
    if(name == field.localName() && ns == field.fieldNamespace())
    {
      return gcnew DNField(field);
    }
  }
  return nullptr;
}

System::Collections::IEnumerator^ DNFieldSet::GetEnumerator()
{
  return gcnew DNFieldSet::DNFieldSetEnumerator(impl_, this);
}

System::Collections::Generic::IEnumerator<DNField ^ >^
DNFieldSet::GetSpecializedEnumerator()
{
  return gcnew DNFieldSet::DNFieldSetEnumerator(impl_, this);
}

DNFieldSet::DNFieldSetEnumerator::DNFieldSetEnumerator(ImplFieldSet * impl, DNFieldSet^ parent)
  : parent_(parent)
  , impl_(impl)
  , position_(size_t(-1))
  , size_(impl_->size())
{
}

DNFieldSet::DNFieldSetEnumerator::~DNFieldSetEnumerator()
{
}

bool
DNFieldSet::DNFieldSetEnumerator::MoveNext()
{
  if(position_ < size_)
  {
    ++position_;
  }
  else if (position_ == size_t(-1))
  {
    position_ = 0;
  }
  return position_ < size_;
}

void
DNFieldSet::DNFieldSetEnumerator::Reset()
{
  position_ = size_t(-1);
}


DNField ^
DNFieldSet::DNFieldSetEnumerator::GenericCurrent::get()
{
  if(position_ < size_)
  {
    return gcnew DNField(impl_->operator[] (position_));
  }
  return nullptr;
}

System::Object^
DNFieldSet::DNFieldSetEnumerator::Current::get()
{
  if(position_ < size_)
  {
    return gcnew DNField(impl_->operator[] (position_));
  }
  return nullptr;
}
