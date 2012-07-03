// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "QuickFASTDotNetPch.h"
#include "ImplSequence.h"
#include <DotNet/ImplFieldSet.h>
#pragma unmanaged
#include <Messages/FieldIdentity.h>

using namespace QuickFAST;
using namespace DotNet;


ImplSequence::ImplSequence(
    const Messages::FieldIdentity &  lengthIdentity,
    size_t length
  )
  : capacity_(length)
  , used_(0)
  , lengthIdentity_(lengthIdentity)
  , entries_(new EntryPtr[length])
{
}

ImplSequence::~ImplSequence()
{
  for(size_t nEntry = 0; nEntry < used_; ++ nEntry)
  {
    delete entries_[nEntry];
  }
}

const std::string
ImplSequence::lengthName()const
{
  return lengthIdentity_.getLocalName();
}

/// @brief access the namespace of the length field
const std::string
ImplSequence::lengthNamespace()const
{
  return lengthIdentity_.getNamespace();;
}

/// @brief access the id of the length field
const std::string
ImplSequence::lengthId()const
{
  return lengthIdentity_.id();
}

size_t
ImplSequence::size()
{
  return used_;
}

ImplFieldSet &
ImplSequence::operator[](size_t index)
{
  if(index >= used_)
  {
    throw std::range_error("Sequence index out of range.");
  }
  return *(entries_[index]);
}


void
ImplSequence::append(ImplFieldSet * entry)
{
  if(used_ >= capacity_)
  {
    throw std::range_error("Sequence full.");
  }
  entries_[used_] = entry;
  ++used_;
}

