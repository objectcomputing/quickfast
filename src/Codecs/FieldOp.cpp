// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldOp.h"
#include <Codecs/DictionaryIndexer.h>
#include <Codecs/Context.h>
#include <Common/Exceptions.h>

using namespace QuickFAST;
using namespace Codecs;

FieldOp::FieldOp()
: valueIsDefined_(false)
, dictionaryIndex_(0)
, dictionaryIndexValid_(false)
, pmapBit_(0)
, pmapBitValid_(false)
{
}

void
FieldOp::indexDictionaries(
  DictionaryIndexer & indexer,
  const std::string & dictionaryName,
  const std::string & typeName,
  const std::string & typeNamespace,
  const std::string & fieldName,
  const std::string & fieldNamespace)
{
  if(usesDictionary())
  {
    std::string name = dictionaryName;
    if(!dictionaryName_.empty())
    {
      name = dictionaryName_;
    }
    std::string key = fieldName;
    std::string keyNamespace = fieldNamespace;
    if(!key_.empty())
    {
      key = key_;
      keyNamespace = keyNamespace_;
    }
    dictionaryIndex_ = indexer.getIndex(
      dictionaryName,
      typeName,
      typeNamespace,
      key,
      keyNamespace);
    dictionaryIndexValid_ = true;
  }
}

#if 0
void
FieldOp::setDictionaryValue(
  Context & context,
  const Messages::FieldCPtr & value)
{
  if(!dictionaryIndexValid_)
  {
    throw TemplateDefinitionError("No index available for field.");
  }
  context.setDictionaryValue(dictionaryIndex_, value);
}

bool
FieldOp::findDictionaryField(
  Context & context,
  Messages::FieldCPtr & value)
{
  if(!dictionaryIndexValid_)
  {
    throw TemplateDefinitionError("No index available for field.");
  }
  return context.findDictionaryField(dictionaryIndex_, value);
}

#endif
