// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "TemplateRegistry.h"
#include <Codecs/Template.h>
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

TemplateRegistry::TemplateRegistry()
: presenceMapBits_(1) // every template requires 1 bit for the template ID
, maxFieldCount_(0)
{
}

void
TemplateRegistry::finalize()
{
  presenceMapBits_ = 1;
  maxFieldCount_ = 0;
  for(TemplateIdMap::const_iterator it = templates_.begin();
    it != templates_.end();
    ++it)
  {
    size_t bits = it->second->presenceMapBitCount();
    if(bits > presenceMapBits_)
    {
      presenceMapBits_ = bits;
    }
    size_t fieldCount = it->second->fieldCount();
    if(fieldCount > maxFieldCount_)
    {
      maxFieldCount_ = fieldCount;
    }
  }
}



void
TemplateRegistry::addTemplate(TemplateCPtr value)
{
  template_id_t id = value->getId();
  templates_[id] = value;
  size_t bits = value->presenceMapBitCount();
  if(bits > presenceMapBits_)
  {
    presenceMapBits_ = bits;
  }
}

size_t
TemplateRegistry::size()const
{
  return templates_.size();
}

bool
TemplateRegistry::getTemplate(template_id_t templateId, TemplateCPtr & valueFound)const
{
  TemplateIdMap::const_iterator it = templates_.find(templateId);
  if(it == templates_.end())
  {
    return false;
  }
  valueFound = it->second;
  return bool(valueFound);
}

bool
TemplateRegistry::findNamedTemplate(
  const std::string & name,
  const std::string & templateNamespace,
  TemplateCPtr & valueFound)const
{
  for(TemplateIdMap::const_iterator it = templates_.begin();
    it != templates_.end();
    ++it)
  {
    if(it->second->getTemplateName() == name
     &&(templateNamespace.empty()
        || templateNamespace == it->second->getNamespace()))
    {
      valueFound = it->second;
      return true;
    }
  }
  return true;
}
