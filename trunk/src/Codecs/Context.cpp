// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#include "Context.h"
#include <Codecs/TemplateRegistry.h>
#include <Codecs/Dictionary.h>
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

Context::Context(Codecs::TemplateRegistryCPtr registry)
: verboseOut_(0)
, logOut_(0)
, templateRegistry_(registry)
, globalDictionary_(new Dictionary)
, templateDictionary_(new Dictionary)
{
}

Context::~Context()
{
}

void
Context::reset()
{
  globalDictionary_.reset(new Dictionary);
  templateDictionary_.reset();
  namedDictionaries_.clear();
  typeDictionaries_.clear();
  templateDictionaries_.clear();
}


bool
Context::findTemplate(const std::string & name, const std::string & nameSpace, TemplateCPtr & result) const
{
  return templateRegistry_->findNamedTemplate(
        name,
        nameSpace,
        result);
}


const DictionaryPtr &
Context::getGlobalDictionary()const
{
  return globalDictionary_;
}

const DictionaryPtr &
Context::getTemplateDictionary()const
{
  return templateDictionary_;
}

const DictionaryPtr &
Context::getTypeDictionary(const std::string & appName)
{
  NamedDictionaryMap::iterator pEntry = typeDictionaries_.find(appName);
  if(pEntry == typeDictionaries_.end())
  {
    typeDictionaries_[appName] = DictionaryPtr(new Dictionary);
  }
  return typeDictionaries_[appName];
}

const DictionaryPtr &
Context::getNamedDictionary(const std::string & name)
{
  NamedDictionaryMap::iterator pEntry = namedDictionaries_.find(name);
  if(pEntry == namedDictionaries_.end())
  {
    namedDictionaries_[name] = DictionaryPtr(new Dictionary);
  }
  return namedDictionaries_[name];
}

void
Context::logMessage(const std::string & message)
{
  if(logOut_)
  {
    (*logOut_) << message << std::endl;
  }
}

void
Context::reportWarning(const std::string & errorCode, const std::string & message)
{
  if(logOut_)
  {
    (*logOut_) << errorCode << ' ' << message << std::endl;
  }
}

void
Context::reportError(const std::string & errorCode, const std::string & message)
{
  throw EncodingError(errorCode + message);
}

void
Context::reportFatal(const std::string & errorCode, const std::string & message)
{
  throw EncodingError(errorCode + message);
}
