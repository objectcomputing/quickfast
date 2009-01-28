// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "Examples/ExamplesPch.h"
#include "CommandArgParser.h"

using namespace ::QuickFAST;

CommandArgParser::CommandArgParser()
{
}

CommandArgParser::~CommandArgParser()
{
}

void
CommandArgParser::addHandler(CommandArgHandler * handler)
{
  commandHandlers_.push_back(handler);
}


int
CommandArgParser::parseSingleArg(int argc, char * argv[])
{
  int consumed = 0;
  for(CommandArgHandlerVec::iterator it = commandHandlers_.begin();
    consumed == 0 && it != commandHandlers_.end();
    ++it)
  {
    consumed = (*it)->parseSingleArg(argc, argv);
  }
  return consumed;
}

bool
CommandArgParser::parseArgs(int argc, char * argv[])
{
  bool result = true;
  int argp = 1;
  while(result && argp < argc)
  {
    int consumed = parseSingleArg(argc - argp, &argv[argp]);
    if(consumed == 0)
    {
      std::string arg(argv[argp]);
      if(arg != "-?")
      {
        std::cerr << "Unknown argument: " << argv[argp] << std::endl;
      }
      usage(std::cerr);
      result = false;
    }
    argp += consumed;
  }
  return result;
}

void
CommandArgParser::usage(std::ostream & out) const
{
  out << "Command line arguments are:" << std::endl;
  for(CommandArgHandlerVec::const_iterator it = commandHandlers_.begin();
    it != commandHandlers_.end();
    ++it)
  {
    (*it)->usage(out);
    out << "  -?: Display this information" << std::endl;
  }
}

bool
CommandArgParser::parse(int argc, char * argv[])
{
  bool result = true;
  result = parseArgs(argc, argv);

  for(CommandArgHandlerVec::iterator it = commandHandlers_.begin();
    result && it != commandHandlers_.end();
    ++it)
  {
    if(!(*it)->applyArgs())
    {
      result = false;
    }
  }
  return result;
}
