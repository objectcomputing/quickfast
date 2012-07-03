// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldIdentity.h"

#ifdef _MSC_VER
#pragma warning(disable:4355) // disable warning C4355: 'this' : used in base member initializer list
#endif
using namespace QuickFAST;
using namespace Messages;

static
std::string anonName(void * address)
{
  return boost::lexical_cast<std::string>(address);
}

FieldIdentity::FieldIdentity()
  : localName_(anonName(this))
{
  qualifyName();
}

FieldIdentity::FieldIdentity(
  const std::string & name,
  const std::string & fieldNamespace /* = ""*/,
  const field_id_t & id /* = ""*/)
  : localName_(name)
  , fieldNamespace_(fieldNamespace)
  , id_(id)
{
  qualifyName();
}


FieldIdentity::~FieldIdentity()
{
}

void
FieldIdentity::display(std::ostream & output)const
{
  output << " name=\"" << localName_;
  if(!fieldNamespace_.empty())
  {
    output << "\" ns=\"" << fieldNamespace_;
  }
  if(!id_.empty())
  {
    output << "\" id=\"" << id_;
  }
  output << '"';
}
