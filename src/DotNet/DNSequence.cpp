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
DNSequence::LengthName::get()
{
  return string_cast(impl_->lengthName());
}

System::String ^
DNSequence::LengthNamespace::get()
{
  return string_cast(impl_->lengthNamespace());
}

System::String ^
DNSequence::LengthId::get()
{
  return string_cast(impl_->lengthId());
}

int
DNSequence::Count::get()
{
  return static_cast<int>(impl_->size());
}

DNFieldSet ^
DNSequence::entry(int index)
{
  return gcnew DNFieldSet(impl_->operator[] (size_t(index)));
}

System::Collections::IEnumerator^ DNSequence::GetEnumerator()
{
  return gcnew DNSequenceEnumerator(impl_, this);
}

System::Collections::Generic::IEnumerator<DNFieldSet^>^ DNSequence::GetSpecializedEnumerator()
{
  return gcnew DNSequenceEnumerator(impl_, this);
}

DNSequence::DNSequenceEnumerator::DNSequenceEnumerator(ImplSequence * impl, DNSequence^ parent)
  : parent_(parent)
  , impl_(impl)
  , position_(size_t(-1))
  , size_(impl_->size())
{
}

DNSequence::DNSequenceEnumerator::~DNSequenceEnumerator()
{
}

bool
DNSequence::DNSequenceEnumerator::MoveNext()
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
DNSequence::DNSequenceEnumerator::Reset()
{
  position_ = size_t(-1);
}


DNFieldSet^
DNSequence::DNSequenceEnumerator::GenericCurrent::get()
{
  if(position_ < size_)
  {
    return gcnew DNFieldSet(impl_->operator[] (position_));
  }
  return nullptr;
}

System::Object^
DNSequence::DNSequenceEnumerator::Current::get()
{
  if(position_ < size_)
  {
    return gcnew DNFieldSet(impl_->operator[] (position_));
  }
  return nullptr;
}

