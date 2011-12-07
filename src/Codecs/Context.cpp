// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#include "Context.h"
#include <Codecs/TemplateRegistry.h>
#include <Common/Exceptions.h>
#include <Messages/FieldIdentity.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

Context::Context(Codecs::TemplateRegistryCPtr registry)
: verboseOut_(0)
, logOut_(0)
, templateRegistry_(registry)
, templateId_(~0U)
, strict_(true)
, indexedDictionarySize_(registry->dictionarySize())
//, indexedDictionary_(new Messages::FieldCPtr[indexedDictionarySize_])
, indexedDictionary_(new Value[indexedDictionarySize_])
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
    indexedDictionary_[nDict].erase();
  }
  if(resetTemplateId)
  {
    templateId_ = ~0U;
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
    (*logOut_) << errorCode << ' ' << message  << " Field: " << identity.name() << std::endl;
  }
}

void
Context::reportWarning(
  const std::string & errorCode,
  const std::string & message,
  const std::string & name)
{
  if(logOut_)
  {
    (*logOut_) << errorCode << ' ' << message  << " Field: " << name << std::endl;
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
Context::reportError(
  const std::string & errorCode,
  const std::string & message,
  const std::string & name)
{
  throw EncodingError(errorCode + ' ' + message + " Field: " + name);
}

void
Context::reportFatal(const std::string & errorCode, const std::string & message)
{
  throw EncodingError(errorCode + ' ' +  message);
}

void
Context::reportFatal(
  const std::string & errorCode,
  const std::string & message,
  const Messages::FieldIdentity & identity)
{
  throw EncodingError(errorCode + ' ' + message + " Field: " + identity.name());
}

void
Context::reportFatal(
  const std::string & errorCode,
  const std::string & message,
  const std::string & name)
{
  throw EncodingError(errorCode + ' ' + message + " Field: " + name);
}

