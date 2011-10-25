// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef EXAMPLES_COMMANDARGPARSER_H
#define EXAMPLES_COMMANDARGPARSER_H
#pragma message("Warning:  CommandArgParser has moved to Application namespace.")

#include <Application/CommandArgParser.h>
#include <Examples/CommandArgHandler.h>
namespace QuickFAST
{
  using ::QuickFAST::Application::CommandArgParser;
}
#endif // EXAMPLES_COMMANDARGPARSER_H
