// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef FILETOMULTICAST_H
#define FILETOMULTICAST_H
#include <Application/CommandArgParser.h>
#include <Communication/AsioService.h>
#include <Communication/MulticastSender_fwd.h>
#include <Communication/BufferRecycler.h>
#include <stdio.h>

namespace QuickFAST{
  namespace Examples{

    /// @brief Multicast the contents of a FAST encoded data file.
    ///
    /// This program uses an echo file produced by the InterpretFAST program
    /// to identify the message boundaries in a FAST encoded data file.
    /// It multicasts each message in a separate datagram.
    ///
    /// Use the -? command line option for more information.
    ///
    /// This program is not really FAST-aware. It is just part of a testing
    /// framework for other programs.
    class FileToMulticast : public Application::CommandArgHandler, public Communication::BufferRecycler
    {
    public:
      FileToMulticast();
      ~FileToMulticast();

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
      void recycle(Communication::LinkedBuffer * emptyBuffer);

    private:
      bool parseIndexFile();
      void sendBurst();

    private:
      virtual int parseSingleArg(int argc, char * argv[]);
      virtual void usage(std::ostream & out) const;
      virtual bool applyArgs();
    private:
      unsigned short portNumber_;
      std::string sendAddress_;
      std::string dataFileName_;
      std::string indexFileName_;
      size_t sendCount_;
      size_t sendMicroseconds_;
      size_t burst_;
      bool pauseEveryPass_;
      bool pauseEveryMessage_;
      bool verbose_;

      Communication::AsioService ioService_;
      boost::asio::strand strand_;
      boost::asio::deadline_timer timer_;

      Application::CommandArgParser commandArgParser_;
      FILE * dataFile_;

      typedef std::pair<size_t, size_t> MessagePosition; // position in file: start, length
      typedef std::vector<MessagePosition> MessageIndex;
      MessageIndex messageIndex_;

      boost::scoped_array<unsigned char> buffer_;
      size_t bufferSize_;
      size_t nPass_;
      size_t nMsg_;
      size_t totalMessageCount_;
      Communication::MulticastSenderPtr sender_;
    };
  }
}
#endif // FILETOMULTICAST_H
