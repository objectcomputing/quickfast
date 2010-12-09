// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Examples/ExamplesPch.h>
#include "CommandArgParser.h"
#include <Common/Version.h>

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
CommandArgParser::parseArgs(int argc, char * argv[], int start)
{
  bool result = true;
  int argp = start;
  while(result && argp < argc)
  {
    int consumed = parseSingleArg(argc - argp, &argv[argp]);
    if(consumed == 0)
    {
      std::string arg(argv[argp]);
      std::cout << "checking for -options: " << arg << '[' << argp << '/' << argc << ']' << std::endl;
      if(arg == "-options" && argp + 1 < argc)
      {
        std::cout << "-options found.  Parsing file: " << argv[argp + 1] << std::endl;
        if(parseFile(argv[argp + 1]))
        {
          std::cout << "Parse file: " << argv[argp + 1] << " succeeded"<< std::endl;
          consumed = 2;
        }
        else
        {
        std::cout << "Parse file: " << argv[argp + 1] << " failed"<< std::endl;
          // parseFile will have reported the problem
          return false;
        }
      }
    }

    if(consumed == 0)
    {
      std::string arg(argv[argp]);
      if(arg == "-V")
      {
        std::cerr << QuickFAST_Product << std::endl;
        consumed = 1;
      }
      else
      {
        if(arg != "-?")
        {
          std::cerr << "Unknown argument: " << argv[argp] << std::endl;
        }
        usage(std::cerr);
      }
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
  }
  out << "  -options filename : Read addtional options from the file, filename." << std::endl;
  out << "  -V: Display version information and exit." << std::endl;
  out << "  -?: Display this information and exit" << std::endl;
}


bool
CommandArgParser::parseFile(const char * filename)
{
  FILE * optionsFile = std::fopen(filename, "r");
  if(optionsFile == 0)
  {
    std::cerr << "Cannot open options file: " << filename << std::endl;
    usage(std::cerr);
    return false;
  }
  std::fseek(optionsFile, 0L, SEEK_END);
  size_t optionsSize = std::ftell(optionsFile);
  std::rewind(optionsFile);
  if(optionsSize == 0)
  {
    return true;
  }
  boost::scoped_array<char> buffer(new char[optionsSize + 1]);
  size_t bytesRead = std::fread(buffer.get(), 1, optionsSize, optionsFile);
  std::fclose(optionsFile);
  buffer[bytesRead] = '\0';
  bool result = tokenizeAndParseLine(buffer.get(), bytesRead);
  if(!result)
  {
    std::cout << "Error detected while parsing file: " << filename << std::endl;
  }
  return result;
}

bool
CommandArgParser::tokenizeAndParseLine(char * buffer, size_t bytesRead)
{
  std::vector< std::string > args;

  // backslash is escape; whitespace is delimiter; single or double quotes
  typedef boost::escaped_list_separator<char> TokenFunction;
  typedef boost::tokenizer<TokenFunction, const char *, std::string> EscapedQuotedTokenizer;
  typedef EscapedQuotedTokenizer::iterator TokenIterator;
  TokenFunction tokenFunction("\\", " \t\r\n", "\"'");
  EscapedQuotedTokenizer tokenizer (buffer, buffer + bytesRead, tokenFunction);

  for(TokenIterator pToken  = tokenizer.begin(); pToken != tokenizer.end(); ++pToken)
  {
    args.push_back(*pToken);
  }

  size_t size = args.size();
  boost::scoped_array<char *> argv(new char *[size + 1]);
  size_t argc = 0;
  for(size_t pos = 0; pos < size; ++pos)
  {
    if(!args[pos].empty())
    {
      argv[argc++] = const_cast<char *>(args[pos].c_str());
    }
  }
  argv[argc] = 0;
  return parseArgs((int)argc, argv.get(), 0);
}

bool
CommandArgParser::parse(int argc, char * argv[], int start/* = 1*/)
{
  bool result = true;
  result = parseArgs(argc, argv, start);

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
