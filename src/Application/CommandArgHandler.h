// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef COMMANDARGHANDLER_H
#define COMMANDARGHANDLER_H
#include <Common/QuickFAST_Export.h>

namespace QuickFAST
{
  namespace Application
  {
    /// @brief Abstract interface to be implemented by objects
    /// that handle command line options.
    class QuickFAST_Export CommandArgHandler
    {
    public:
      /// @brief Parse a single argument from the command line.
      ///
      /// Each call should consume one logical argument.  You will be called again
      /// to consume subsequent arguments.  For example if your implementation accepts
      /// arguments -f [file] and -v, and argc is -f output -x -v you should interpret
      /// [-f output] and return 2.  Do NOT go looking for the -v.
      /// @param argc the count of remaining, unparsed arguments
      /// @param argv the remaining, unparsed arguments from the command line
      /// @return the number of arguments consumed. 0 means argv[0] was not recognized.
      virtual int parseSingleArg(int argc, char * argv[]) = 0;

      /// @brief Report command line arguments recognized by this handler.
      /// @param out is the device to which the known arguments should be reported.
      virtual void usage(std::ostream & out) const = 0;

      /// @brief After all arguments a parsed apply them.
      ///
      /// This will be called only if all arguments are parsed successfully.
      /// @returns true if successful
      virtual bool applyArgs() = 0;
    };
  }
}
#endif // COMMANDARGHANDLER_H
