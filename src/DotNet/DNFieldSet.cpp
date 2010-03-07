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

size_t
DNFieldSet::size()
{
  if(impl_ == 0)
  {
    throw std::logic_error("Call size on cleared DNFieldSet");
  }
  return impl_->size();
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

DNField ^
DNFieldSet::getField(size_t index)
{
  if(impl_ == 0)
  {
    throw std::logic_error("Call getField on cleared DNFieldSet");
  }
  return gcnew DNField((*impl_)[index]);
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

