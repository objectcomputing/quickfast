// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef PERFORMANCETEST_H
#define PERFORMANCETEST_H

#include <Codecs/XMLTemplateParser.h>
#include <Codecs/DataSource.h>
#include <Application/CommandArgParser.h>

namespace QuickFAST{
  namespace Examples{
    /// @brief A simple performance test that measures the time used to decode the messages in a file.
    ///
    /// This is also a good program to run when profiling.
    ///
    /// Run the program with a -? command line option for detailed usage information.
    class PerformanceTest : public Application::CommandArgHandler
    {
    public:
      PerformanceTest();
      ~PerformanceTest();

      /// @brief parse command line arguments, and initialize.
      /// @param argc from main
      /// @param argv from main
      /// @returns true if everything is ok.
      bool init(int argc, char * argv[]);
      /// @brief run the program
      /// @returns a value to be used as an exit code of the program (0 means all is well)
      int run();
      /// @brief do final cleanup after a run.
      void fini();

    private:
      virtual int parseSingleArg(int argc, char * argv[]);
      virtual void usage(std::ostream & out) const;
      virtual bool applyArgs();
    private:
      bool resetOnMessage_;
      bool strict_;
      bool useNullMessage_;
      std::string templateFileName_;
      std::ifstream templateFile_;
      std::string fastFileName_;
      std::ifstream fastFile_;
      std::string performanceFileName_;
      std::ostream * performanceFile_;
      std::string profileFileName_;
      std::ostream * profileFile_;
      size_t head_;
      size_t count_;
      size_t interpret_;
      size_t headerBytes_;
      bool echo_;

      Codecs::XMLTemplateParser parser_;
      Application::CommandArgParser commandArgParser_;
    };
  }
}
#endif // PERFORMANCETEST_H
