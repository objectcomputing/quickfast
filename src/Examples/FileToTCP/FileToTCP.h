// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef FILETOTCP_H
#define FILETOTCP_H
#include <Application/CommandArgParser.h>
#include <boost/asio.hpp>
#include <stdio.h>

namespace QuickFAST{
    namespace Examples{

    /// @brief Send the contents of a FAST encoded data file via TCP/IP.
    ///
    /// The contents of a data file are sent to a TCP/IP socket.  No attempt
    /// is made to identify message boundaries which is consistent with normal
    /// TCP/IP buffering behavior.
    ///
    /// Use the -? command line option for more information.
    ///
    /// This program is not really FAST-aware. It is just part of a testing
    /// framework for other programs.

    class FileToTCP : public Application::CommandArgHandler
    {
    public:
      FileToTCP();
      ~FileToTCP();

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
      unsigned short portNumber_;
      size_t bufferSize_;
      std::string fileName_;
      size_t sendCount_;
      FILE * inFile_;
      bool verbose_;
      bool hack_;

      boost::asio::io_service io_service_;
      Application::CommandArgParser commandArgParser_;
    };
  }
}
#endif // FILETOTCP_H
