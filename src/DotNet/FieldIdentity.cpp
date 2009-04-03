// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include <DotNet/FieldIdentity.h>
#include <Messages/FieldIdentity.h>

using namespace QuickFASTDotNet;
using namespace Messages;

FieldIdentity::FieldIdentity()
  :spFieldIdentity_(new TFieldIdentity)
{
}

FieldIdentity::FieldIdentity(FieldIdentity^ rhs)
  :spFieldIdentity_(new TFieldIdentity(rhs->spFieldIdentity_.GetRef()))
{
}

FieldIdentity::FieldIdentity(System::String^ fieldName)
  :spFieldIdentity_(new TFieldIdentity)
{
  this->LocalName = fieldName;
}

FieldIdentity::FieldIdentity(const TspFieldIdentity& fieldIdentity)
  :spFieldIdentity_(fieldIdentity)
{
}

FieldIdentity::FieldIdentity(const TcspFieldIdentity& fieldIdentity)
  :spFieldIdentity_(new TFieldIdentity(*fieldIdentity))
{

}

FieldIdentity::FieldIdentity(System::String^ fieldNamespace, System::String^ fieldName, System::String^ Id)
  :spFieldIdentity_(new TFieldIdentity)
{
  this->LocalName = fieldName;
  this->Namespace = fieldNamespace;
  this->Id = Id;
}


FieldIdentity::FieldIdentity(System::String^ name, System::String^ fieldNamespace)
  :spFieldIdentity_(new TFieldIdentity(StlDotNet::string_cast<std::string>(name),
                                      StlDotNet::string_cast<std::string>(fieldNamespace)))
{
}

System::String^ FieldIdentity::LocalName::get()
{
  return StlDotNet::string_cast(spFieldIdentity_->getLocalName());
}

void FieldIdentity::LocalName::set(System::String^ name)
{
  spFieldIdentity_->setName(StlDotNet::string_cast<std::string>(name));
}

System::String^ FieldIdentity::Namespace::get()
{
  return StlDotNet::string_cast(spFieldIdentity_->getNamespace());
}

void FieldIdentity::Namespace::set(System::String^ fieldNamespace)
{
  spFieldIdentity_->setNamespace(StlDotNet::string_cast<std::string>(fieldNamespace));
}

System::String^ FieldIdentity::Id::get()
{
  return StlDotNet::string_cast(spFieldIdentity_->id());
}

void FieldIdentity::Id::set(System::String^ id)
{
  spFieldIdentity_->setId(StlDotNet::string_cast<QuickFAST::field_id_t>(id));
}

System::String^ FieldIdentity::Name::get()
{
  return StlDotNet::string_cast<std::string>(spFieldIdentity_->name());
}

bool FieldIdentity::Mandatory::get()
{
  return spFieldIdentity_->mandatory();
}

void FieldIdentity::Mandatory::set(bool value)
{
  spFieldIdentity_->setMandatory(value);
}

FieldIdentity^ FieldIdentity::Clone()
{
  return gcnew FieldIdentity(this);
}

FieldIdentity::TFieldIdentity& FieldIdentity::Ref::get()
{
  return spFieldIdentity_.GetRef();
}

FieldIdentity::TspFieldIdentity& FieldIdentity::SmartPtr::get()
{
  return spFieldIdentity_.GetBoostPtr();
}
