// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "Dictionary.h"
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

#include <Common/Profiler.h>

Dictionary::Dictionary()
{
}

Dictionary::~Dictionary()
{
}

void
Dictionary::add(const std::string & name, Messages::FieldCPtr field)
{
  PROFILE_POINT("Dictionary::add");
  dictionary_[name] = field;
}


bool
Dictionary::find(const std::string & name, Messages::FieldCPtr & field)const
{
  PROFILE_POINT("Dictionary::find");
  DictionaryImplementation::const_iterator position =
    dictionary_.find(name);
  if(position == dictionary_.end())
  {
    return false;
  }
  field = position->second;
  return true;
}
