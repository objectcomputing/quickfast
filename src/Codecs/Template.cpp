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

void
Template::display(std::ostream & output, size_t indent) const
{
  std::string indentString(indent, ' ');
  output << std::endl << indentString << "<template id=\"" << templateId_;
  // name is required
  output << "\" name=\"" << templateName_;

  if(!templateNamespace_.empty())
  {
    output << "\" templateNs=\"" << templateNamespace_;
  }
  if(!namespace_.empty())
  {
    output << "\" ns=\"" << namespace_;
  }
  if(!dictionaryName_.empty())
  {
    output << "\" dictionary=\"" << dictionaryName_;
  }
  if(reset_)
  {
    output << "\" reset=\"Y";
  }
  output << "\">";
  if(ignore_)
  {
    output << indentString << "  <!-- NO OUTPUT WILL BE GENERATED FOR THIS TEMPLATE -->";
  }
  else
  {
    SegmentBody::display(output, indent + 2);
  }
  output << std::endl << indentString << "</template> <!-- " << templateId_ << "-->";
}


