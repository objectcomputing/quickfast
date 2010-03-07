// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "QuickFASTDotNetPch.h"
#include "ImplSequence.h"
#include <DotNet/ImplFieldSet.h>
#include <Messages/FieldIdentity.h>


using namespace QuickFAST;
using namespace DotNet;


ImplSequence::ImplSequence(
    Messages::FieldIdentityCPtr & lengthIdentity,
    size_t length
  )
  : lengthName_(lengthIdentity->getLocalName())
  , lengthNamespace_(lengthIdentity->getNamespace())
  , lengthId_(lengthIdentity->id())
{
}

ImplSequence::~ImplSequence()
{
  for(size_t nEntry = 0; nEntry < entries_.size(); ++ nEntry)
  {
    delete entries_[nEntry];
  }
}

size_t
ImplSequence::size()
{
  return entries_.size();
}

ImplFieldSet &
ImplSequence::operator[](size_t index)
{
  return *(entries_[index]);
}


void
ImplSequence::append(ImplFieldSet * entry)
{
  entries_.push_back(entry);
}

