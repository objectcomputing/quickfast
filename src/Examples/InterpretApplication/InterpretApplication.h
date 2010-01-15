// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef INTERPRETFAST_H
#define INTERPRETFAST_H

#include <Application/FastConnection.h>
#include <Examples/CommandArgParser.h>

namespace QuickFAST{
  namespace Examples{
    /// @brief Read FAST encoded messages from a file; decode them; and interpret the resulting messages.
    ///
    /// May also produce an echo file to be used by other example programs in QuickFAST.
    ///
    /// Run the program with a -? command line option for detailed usage information.
    class InterpretApplication : public CommandArgHandler
    {
    public:
      InterpretApplication();
      ~InterpretApplication();

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
      std::string templateFileName_;
      std::string fastFileName_;
      bool isBlocked_;
      std::string outputFileName_;
      std::ostream * outputFile_;
      std::string echoFileName_;
      std::ostream * echoFile_;
      Codecs::DataSource::EchoType echoType_;
      bool echoMessage_;
      bool echoField_;
      size_t head_;

      QuickFAST::Application::FastConnection connection_;
      CommandArgParser commandArgParser_;
    };
  }
}
#endif // INTERPRETFAST_H
