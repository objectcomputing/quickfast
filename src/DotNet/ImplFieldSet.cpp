// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "QuickFASTDotNetPch.h"
#include "ImplFieldSet.h"
#include <DotNet/ImplField.h>
#pragma unmanaged
using namespace QuickFAST;
using namespace DotNet;

ImplFieldSet::ImplFieldSet(size_t fieldCount)
  : capacity_(fieldCount)
  , used_(0)
  , fields_(new ImplField[fieldCount])
{
}

ImplFieldSet::~ImplFieldSet()
{
}

size_t
ImplFieldSet::size() const
{
  return used_;
}

ImplField &
ImplFieldSet::operator[](size_t index)
{
  if(index >= used_)
  {
    std::stringstream msg;
    msg << "Field Set index out of range: [0-" << used_ << ") :: " << index;
    throw std::range_error(msg.str());
  }
  return fields_[index];
}

ImplField &
ImplFieldSet::append()
{
  if(used_ >= capacity_)
  {
    std::stringstream msg;
    msg << "Field Set full: [0-" << capacity_ << ")";
    throw std::range_error(msg.str());
  }
  size_t index = used_;
  used_ += 1;
  return fields_[index];
}

