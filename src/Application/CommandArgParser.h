// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef COMMANDARGPARSER_H
#define COMMANDARGPARSER_H
#include <Application/CommandArgHandler.h>

namespace QuickFAST
{
  namespace Application
  {
    /// @brief parse arguments from the command line.
    ///
    /// Parses the arguments then sends to each registered TestScriptDriver::CommandArgHandler
    /// in turn until one of them consumes one or more of the arguments.
    class QuickFAST_Export CommandArgParser
    {
    public:
      CommandArgParser();
      virtual ~CommandArgParser();

      /// @brief Add a new handler for command line arguments.
      ///
      /// Does *not* take ownership of handler
      /// @param handler a handler for script commands.
      void addHandler(CommandArgHandler * handler);

      /// @brief accept the arguments from the command line and parse them.
      ///
      /// @param argc is a count of the args
      /// @param argv is an array c-style strings (i.e. char*'s null terminated)
      /// @param start is the offset to the first argument to parse -- default skips the command name
      /// @return true if parsing was successful; false if an error was detected and reported.
      bool parse(int argc,  char * argv[], int start = 1);

      /// @brief display a usage message
      /// @param out is the stream to which the message will be written.
      void usage(std::ostream & out) const;

      /// @todo DOxygen
      bool tokenizeAndParseLine(char * buffer, size_t bytesRead);
      /// @todo DOxygen
      bool parseFile(const char * filename);

    private:


      bool parseArgs(int argc, char * argv[], int start);
      int parseSingleArg(int argc, char * argv[]);

    private:
      typedef std::vector< CommandArgHandler *> CommandArgHandlerVec;
      CommandArgHandlerVec commandHandlers_;
    };
  }
}
#endif // TESTSCRIPTDRIVER_COMMANDARGPARSER_H
