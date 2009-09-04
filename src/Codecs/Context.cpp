// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#include "Context.h"
#include <Codecs/TemplateRegistry.h>
#include <Common/Exceptions.h>
#include <Messages/Field.h>
#include <Messages/FieldIdentity.h>
#include <Messages/Field.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

Context::Context(Codecs::TemplateRegistryCPtr registry)
: verboseOut_(0)
, logOut_(0)
, templateRegistry_(registry)
, templateId_(~0)
, strict_(true)
, indexedDictionarySize_(registry->dictionarySize())
, indexedDictionary_(new Messages::FieldCPtr[indexedDictionarySize_])
{
}

Context::~Context()
{
}

void
Context::reset(bool resetTemplateId /*= true*/)
{
  for(size_t nDict = 0; nDict < indexedDictionarySize_; ++nDict)
  {
    indexedDictionary_[nDict].reset();
  }
  if(resetTemplateId)
  {
    templateId_ = ~0;
  }
}


bool
Context::findTemplate(const std::string & name, const std::string & nameSpace, TemplateCPtr & result) const
{
  return templateRegistry_->findNamedTemplate(
        name,
        nameSpace,
        result);
}


bool
Context::findDictionaryField(size_t index, Messages::FieldCPtr & field)
{
  if(index >= indexedDictionarySize_)
  {
    throw TemplateDefinitionError("Find invalid dictionary index: should not occur.");
  }
  field = indexedDictionary_[index];
  return bool(field);
}

void
Context::setDictionaryField(size_t index, const Messages::FieldCPtr & field)
{
  if(index >= indexedDictionarySize_)
  {
    throw TemplateDefinitionError("Set invalid dictionary index: should not occur.");
  }
  indexedDictionary_[index] = field;
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
Context::reportWarning(
  const std::string & errorCode,
  const std::string & message,
  const Messages::FieldIdentity & identity)
{
  if(logOut_)
  {
    (*logOut_) << errorCode << ' ' << message  << " Field: " << identity.name()<< std::endl;
  }
}

void
Context::reportError(const std::string & errorCode, const std::string & message)
{
  if(!strict_)
  {
    if(errorCode == "[ERR D2]")
    {
      return;
    }
  }
  throw EncodingError(errorCode + ' ' + message);
}

void
Context::reportError(
  const std::string & errorCode,
  const std::string & message,
  const Messages::FieldIdentity & identity)
{
  throw EncodingError(errorCode + ' ' + message + " Field: " + identity.name());
}

void
Context::reportFatal(const std::string & errorCode, const std::string & message)
{
  throw EncodingError(errorCode + ' ' +  message);
}
