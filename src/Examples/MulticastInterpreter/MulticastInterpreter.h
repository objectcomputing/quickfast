// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTINTERPRETER_H
#define MULTICASTINTERPRETER_H
#include <Codecs/DataSource.h>
#include <Codecs/MulticastDecoder.h>
#include <Codecs/MessageConsumer_fwd.h>
#include <Examples/CommandArgParser.h>

#include <boost/asio.hpp>


namespace QuickFAST{
  namespace Examples{

    /// @brief Receive FAST encoded messages via multicast; decode them; and interpret the resulting messages.
    ///
    /// Run the program with a -? command line option for detailed usage information.
    class MulticastInterpreter : public CommandArgHandler
    {
    public:
      MulticastInterpreter();
      ~MulticastInterpreter();

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
      // set from command line options
      size_t bufferSize_;
      std::string templateFileName_;
      std::string outputFileName_;
      std::string echoFileName_;
      Codecs::DataSource::EchoType echoType_;
      size_t messageLimit_;
      bool strict_;
      unsigned short portNumber_;
      std::string listenAddressName_;
      std::string multicastAddressName_;

      CommandArgParser commandArgParser_;
      std::ifstream templateFile_;
      std::ostream * outputFile_;
      std::ostream * echoFile_;
      bool echoMessage_;
      bool echoField_;

      Codecs::TemplateRegistryPtr templateRegistry_;
      Codecs::MulticastDecoder * decoder_;

    };
  }
}
#endif // MULTICASTINTERPRETER_H
