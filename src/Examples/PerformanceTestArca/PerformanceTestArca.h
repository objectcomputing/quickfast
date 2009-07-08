// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef PERFORMANCETESTARCA_H
#define PERFORMANCETESTARCA_H

#include <Codecs/DataSource.h>
#include <Common/PCapReader.h>
#include <Arca/ArcaMessageHandler.h>
#include <Codecs/BufferConsumer_fwd.h>
#include <Examples/CommandArgParser.h>

namespace QuickFAST{
  namespace Examples{

    class ArcaMessageCounter
      : public QuickFASTArca::ArcaMessageHandler
    {
    public:
      ArcaMessageCounter();
      virtual ~ArcaMessageCounter();

      size_t getMessageCount()const
      {
        return messages_;
      }
      size_t getPacketCount() const
      {
        return packets_;
      }


      ///////////////////////
      // Implement ArcaMessageHandler

      virtual bool handleMessage(
        size_t filterCount,
        const QuickFASTArca::ArcaFullHeader * header,
        const QuickFASTArca::ArcaMessageStructure * body);
      virtual bool reportCommunicationError(const std::string & message);
      virtual bool reportDecodingError(const std::string & message,
        const QuickFASTArca::ArcaFullHeader * header,
        const QuickFASTArca::ArcaMessageStructure * body);
      virtual void receiverStarted();
      virtual void receiverStopped();
    private:
      size_t messages_;
      size_t packets_;
    };


    /// @brief A simple performance test that measures the time used to decode the messages in a file.
    ///
    /// This is also a good program to run when profiling.
    ///
    /// Run the program with a -? command line option for detailed usage information.
    class PerformanceTestArca : public CommandArgHandler
    {
    public:
      PerformanceTestArca();
      ~PerformanceTestArca();

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
      bool resetOnMessage_;
      bool strict_;
      bool verbose_;
      std::string templateFileName_;
      std::ifstream templateFile_;
      std::string fastFileName_;
      PCapReader pcapFile_;
      std::string performanceFileName_;
      std::ostream * performanceFile_;
      std::string profileFileName_;
      std::ostream * profileFile_;
      size_t head_;
      size_t count_;
      size_t interpret_;

      CommandArgParser commandArgParser_;

      Codecs::BufferConsumerPtr bufferConsumer_;
      ArcaMessageCounter messageCounter_;
    };
  }
}
#endif // PERFORMANCETESTARCA_H
