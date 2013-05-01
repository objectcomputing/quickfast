// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "DictionaryIndexer.h"

using namespace QuickFAST;
using namespace Codecs;

DictionaryIndexer::DictionaryIndexer()
: index_(0)
{
}

DictionaryIndexer::~DictionaryIndexer()
{
}


void
DictionaryIndexer:: newTemplate()
{
  templateNames_.clear();
}

size_t
DictionaryIndexer::getIndex(
  const std::string & dictionaryName,
  const std::string & typeName,
  const std::string & typeNamespace,
  const std::string & key,
  const std::string & keyNamespace)
{
  if(dictionaryName.empty() || dictionaryName == "global")
  {
    return getDictionaryIndex(
      globalNames_,
      keyNamespace + '\t' + key);
  }
  else if(dictionaryName == "type")
  {
    return getDictionaryIndex(
      typeNames_,
      typeNamespace + '\t' +typeName + '\t' + keyNamespace + '\t' + key);
  }
  else if(dictionaryName == "template")
  {
    return getDictionaryIndex(
      templateNames_,
      keyNamespace + '\t' + key);
  }
  else
  {
    return getDictionaryIndex(
      qualifiedNames_,
      dictionaryName + '\t' + keyNamespace + '\t' + key);
  }
}

size_t
DictionaryIndexer::getDictionaryIndex(NameToIndex & nameToIndex, const std::string & key)
{
  size_t result = 0;
  NameToIndex::const_iterator it = nameToIndex.find(key);
  if(it != nameToIndex.end())
  {
    result = it->second;
  }
  else
  {
    result = index_++;
    nameToIndex[key] = result;
  }
  return result;
}

size_t
DictionaryIndexer::size()const
{
  return index_;
}
