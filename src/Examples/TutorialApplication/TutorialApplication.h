// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef TUTORIALAPPLICATION_H
#define TUTORIALAPPLICATION_H

#include <Codecs/DataSource.h>

namespace QuickFAST{
  namespace Examples{
    /// @brief A simple application to demonstrate how QuickFAST is intended to be used.
    ///
    /// See the QuickFAST tutorial Wiki page for more information.
    class TutorialApplication
    {
    public:
      TutorialApplication();
      ~TutorialApplication();

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
      std::string templateFileName_;
      std::string fastFileName_;
    };
  }
}
#endif // TUTORIALAPPLICATION_H
