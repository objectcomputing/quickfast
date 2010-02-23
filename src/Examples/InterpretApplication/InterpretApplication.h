// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef INTERPRETFAST_H
#define INTERPRETFAST_H

#include <Examples/CommandArgParser.h>
#include <Codecs/DataSource.h>
#include <Communication/Receiver_fwd.h>
#include <Messages/ValueMessageBuilder.h>

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
      CommandArgParser commandArgParser_;

      std::istream * fastFile_;
      std::ostream * echoFile_;
      std::ostream * verboseFile_ ;


      struct DecoderConfiguration
      {
        size_t head_;                   // -head
        bool reset_;                    // -r
        bool strict_;                   // -strict

        std::string templateFileName_;  // -t
        std::string fastFileName_;      // -file
        std::string verboseFileName_;   // -vo
        std::string pcapFileName_;      // -pcap
        std::string echoFileName_;      // -e
        Codecs::DataSource::EchoType echoType_;  // -ehex; -eraw; -enone
        bool echoMessage_;              // -em
        bool echoField_;                // -ef


        enum HeaderType{
          NO_HEADER,                    // -hnone
          FIXED_HEADER,                 // -hfixed n
          FAST_HEADER}                  // -hfast
          headerType_;

        size_t headerMessageSizeBytes_; // -hfixed *N*
        bool headerBigEndian_;          // -hbig [no]
        size_t headerPrefixCount_;      // -hprefix N
        size_t headerSuffixCount_;      // -hsuffix N

        enum AssemblerType{
          MESSAGE_PER_PACKET_ASSEMBLER, // -streaming
          STREAMING_ASSEMBLER,          // -datagram
          UNSPECIFIED_ASSEMBLER
        } assemblerType_;

        bool waitForCompleteMessage_;   // -streaming [no]wait

        enum ReceiverType
        {
          MULTICAST_RECEIVER,           // -multicast
          TCP_RECEIVER,                 // -tcp
          RAWFILE_RECEIVER,             // -file
          PCAPFILE_RECEIVER,            // -pcap
          UNSPECIFIED_RECEIVER
        } receiverType_;

        std::string multicastGroupIP_;    // -multicast IP:port
        std::string listenInterfaceIP_;   // -mlisten IP
        unsigned short portNumber_;       // -multicast ip:PORT
        std::string portName_;            // -tcp host:PORT
        std::string hostName_;            // -tcp HOST:port

        size_t bufferSize_;               // -buffersize SIZE
        size_t bufferCount_;              // -buffers COUNT

        DecoderConfiguration()
          : head_(0)
          , reset_(false)
          , strict_(true)
          , echoType_(Codecs::DataSource::HEX)
          , echoMessage_(true)
          , echoField_(false)
          , headerType_(NO_HEADER)
          , headerMessageSizeBytes_(0)
          , headerBigEndian_(true)
          , headerPrefixCount_(0)
          , headerSuffixCount_(0)
          , assemblerType_(UNSPECIFIED_ASSEMBLER)
          , waitForCompleteMessage_(false)
          , receiverType_(UNSPECIFIED_RECEIVER)
          , multicastGroupIP_("224.1.2.133")
          , listenInterfaceIP_("0.0.0.0")
          , portNumber_(13014)
          , bufferSize_(1400)
          , bufferCount_(2)
        {
        }
      };

      DecoderConfiguration config_;
    };
  }
}
#endif // INTERPRETFAST_H
