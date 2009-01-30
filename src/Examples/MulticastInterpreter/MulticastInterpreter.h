// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTINTERPRETER_H
#define MULTICASTINTERPRETER_H
#include <Codecs/DataSource.h>
#include <Codecs/MulticastDecoder.h>

#include <Examples/CommandArgParser.h>

#include <boost/asio.hpp>
#include <Codecs/MessageConsumer_fwd.h>


namespace QuickFAST{
  class MulticastInterpreter : public CommandArgHandler
  {
  public:
    MulticastInterpreter();
    ~MulticastInterpreter();

    bool init(int argc, char * argv[]);
    int run();
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
    bool verboseDecode_;
    bool verboseExecution_;
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

    Codecs::MulticastDecoder * decoder_;

  };
}
#endif // MULTICASTINTERPRETER_H
