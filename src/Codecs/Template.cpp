// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "Template.h"
#include <Codecs/FieldInstruction.h>
#include <Common/Exceptions.h>
using namespace QuickFAST;
using namespace Codecs;

void
Template::qualifyName(
  std::string & out,
  const std::string & templateName,
  const std::string & templateNamespace)
{
  if(templateNamespace.empty())
  {
    out = templateName;
  }
  else
  {
    out  = templateName + "\t" + templateNamespace;
  }
}


