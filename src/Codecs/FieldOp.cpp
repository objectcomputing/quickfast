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
      name,
      typeName,
      typeNamespace,
      key,
      keyNamespace);
    dictionaryIndexValid_ = true;
  }
}

const std::string &
FieldOp::opName(OpType type)
{
  switch(type)
  {
  case NOP:
    {
      static const std::string nop("nop");
      return nop;
    }
  case CONSTANT:
    {
      static const std::string constant("constant");
      return constant;
    }
  case DEFAULT:
    {
      static const std::string defaultname("default");
      return defaultname;
    }
  case COPY:
    {
      static const std::string copy("copy");
      return copy;
    }
  case DELTA:
    {
      static const std::string delta("delta");
      return delta;
    }
  case INCREMENT:
    {
      static const std::string increment("increment");
      return increment;
    }
  case TAIL:
    {
      static const std::string tail("tail");
      return tail;
    }
  case UNKNOWN:
  default:
    break;
  }
  static const std::string unknown("UNKNOWN");
  return unknown;
}
