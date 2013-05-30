// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef INTERPRETFAST_H
#define INTERPRETFAST_H

#include <Codecs/DataSource.h>
#include <Communication/Receiver_fwd.h>
#include <Application/CommandArgParser.h>
#include <Application/DecoderConnection.h>

namespace QuickFAST{
  namespace Examples{
    /// @brief Read FAST encoded messages from a file; decode them; and interpret the resulting messages.
    ///
    /// May also produce an echo file to be used by other example programs in QuickFAST.
    ///
    /// Run the program with a -? command line option for detailed usage information.
    class InterpretApplication : public Application::CommandArgHandler
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
      Application::CommandArgParser commandArgParser_;
      typedef boost::shared_ptr<Application::DecoderConfiguration> ConfigurationPtr;
      typedef std::vector<ConfigurationPtr> Configurations;
      typedef boost::shared_ptr<Application::DecoderConnection> ConnectionPtr;
      typedef std::vector<ConnectionPtr> Connections;
      typedef std::vector<Messages::ValueMessageBuilderPtr> Builders;

      // the currently active configuration
      ConfigurationPtr configuration_;

      // the default configuration
      ConfigurationPtr defaultConfiguration_;

      // a collection of all known configurations
      Configurations configurations_;

      // the connections corresponding to configurations_;
      Connections connections_;

      // the builders attached to the connections
      Builders builders_;

      std::string bufferFilename_;
      bool console_;
      bool fixOutput_;
      size_t threads_;
      bool silent_;
    };
  }
}
#endif // INTERPRETFAST_H
