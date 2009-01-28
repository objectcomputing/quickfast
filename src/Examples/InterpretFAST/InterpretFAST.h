// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef INTERPRETFAST_H
#define INTERPRETFAST_H

#include <Codecs/XMLTemplateParser.h>
#include <Codecs/DataSource.h>
#include <Examples/CommandArgParser.h>

namespace QuickFAST{
  class InterpretFAST : public CommandArgHandler
  {
  public:
    InterpretFAST();
    ~InterpretFAST();

    bool init(int argc, char * argv[]);
    int run();
    void fini();

  private:
    virtual int parseSingleArg(int argc, char * argv[]);
    virtual void usage(std::ostream & out) const;
    virtual bool applyArgs();
  private:
    bool verboseParse_;
    bool verboseDecode_;
    bool verboseExecution_;
    bool resetOnMessage_;
    bool strict_;
    std::string templateFileName_;
    std::ifstream templateFile_;
    std::string fastFileName_;
    std::ifstream fastFile_;
    std::string outputFileName_;
    std::ostream * outputFile_;
    std::string echoFileName_;
    std::ostream * echoFile_;
    Codecs::DataSource::EchoType echoType_;
    bool echoMessage_;
    bool echoField_;
    size_t head_;

    Codecs::XMLTemplateParser parser_;
    CommandArgParser commandArgParser_;
  };
}
#endif // INTERPRETFAST_H
