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
  : mandatory_(true)
  , localName_(anonName(this))
{
  qualifyName();
}

FieldIdentity::~FieldIdentity()
{
}

void
FieldIdentity::freeFieldIdentity()const
{
  delete this;
}

#if 0

void
Messages::intrusive_ptr_add_ref(const Messages::FieldIdentity * ptr)
{
  ++ptr->refcount_;
}

void
Messages::intrusive_ptr_release(const Messages::FieldIdentity * ptr)
{
  if(--ptr->refcount_ == 0)
  {
    ptr->freeFieldIdentity();
  }
}
void
Messages::intrusive_ptr_add_ref(Messages::FieldIdentity * ptr)
{
  ++ptr->refcount_;
}

void
Messages::intrusive_ptr_release(Messages::FieldIdentity * ptr)
{
  if(--ptr->refcount_ == 0)
  {
    ptr->freeFieldIdentity();
  }
}

#endif
