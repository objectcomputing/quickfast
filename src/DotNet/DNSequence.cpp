// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "QuickFASTDotNetPch.h"
#include "DNSequence.h"
#include <DotNet/ImplSequence.h>
#include <DotNet/DNFieldSet.h>
#include <DotNet/StringConversion.h>

using namespace QuickFAST;
using namespace DotNet;

DNSequence::DNSequence(ImplSequence & impl)
  : impl_(&impl)
{
}

DNSequence::~DNSequence()
{
}

System::String ^
DNSequence::lengthName()
{
  return string_cast(impl_->lengthName());
}

System::String ^
DNSequence::lengthNamespace()
{
  return string_cast(impl_->lengthNamespace());
}

System::String ^
DNSequence::lengthId()
{
  return string_cast(impl_->lengthId());
}

size_t
DNSequence::size()
{
  return impl_->size();
}

DNFieldSet ^
DNSequence::entry(size_t index)
{
  return gcnew DNFieldSet(impl_->operator[] (index));
}
