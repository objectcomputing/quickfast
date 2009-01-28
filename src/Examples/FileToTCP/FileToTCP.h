// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef FILETOTCP_H
#define FILETOTCP_H
#include <Examples/CommandArgParser.h>
#include <boost/asio.hpp>
#include <stdio.h>

namespace QuickFAST{
  class FileToTCP : public CommandArgHandler
  {
  public:
    FileToTCP();
    ~FileToTCP();

    bool init(int argc, char * argv[]);
    int run();
    void fini();

  private:
    virtual int parseSingleArg(int argc, char * argv[]);
    virtual void usage(std::ostream & out) const;
    virtual bool applyArgs();
  private:
    unsigned short portNumber_;
    size_t bufferSize_;
    std::string fileName_;
    size_t sendCount_;
    bool verbose_;

    boost::asio::io_service io_service_;
    CommandArgParser commandArgParser_;
    FILE * inFile_;

  };
}
#endif // FILETOTCP_H
