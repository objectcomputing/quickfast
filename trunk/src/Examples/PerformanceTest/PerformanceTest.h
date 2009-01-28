// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef PERFORMANCETEST_H
#define PERFORMANCETEST_H

#include <Codecs/XMLTemplateParser.h>
#include <Codecs/DataSource.h>
#include <Examples/CommandArgParser.h>

namespace QuickFAST{
  class PerformanceTest : public CommandArgHandler
  {
  public:
    PerformanceTest();
    ~PerformanceTest();

    bool init(int argc, char * argv[]);
    int run();
    void fini();

  private:
    virtual int parseSingleArg(int argc, char * argv[]);
    virtual void usage(std::ostream & out) const;
    virtual bool applyArgs();
  private:
    bool resetOnMessage_;
    bool strict_;
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

    Codecs::XMLTemplateParser parser_;
    CommandArgParser commandArgParser_;
  };
}
#endif // PERFORMANCETEST_H
