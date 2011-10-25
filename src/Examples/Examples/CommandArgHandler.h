// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef EXAMPLES_COMMANDARGHANDLER_H
#define EXAMPLES_COMMANDARGHANDLER_H
#include <Application/CommandArgHandler.h>
#pragma message("Warning: CommandArgHandler has moved to Application namespace.")
namespace QuickFAST
{
  using ::QuickFAST::Application::CommandArgHandler;
}
#endif // EXAMPLES_COMMANDARGHANDLER_H
