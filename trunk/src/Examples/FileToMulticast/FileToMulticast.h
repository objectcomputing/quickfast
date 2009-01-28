// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef FILETOMULTICAST_H
#define FILETOMULTICAST_H
#include <Examples/CommandArgParser.h>
#include <boost/asio.hpp>
#include <stdio.h>

namespace QuickFAST{
  class FileToMulticast : public CommandArgHandler
  {
  public:
    FileToMulticast();
    ~FileToMulticast();

    bool init(int argc, char * argv[]);
    int run();
    void fini();

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

    boost::asio::io_service ioService_;
    boost::asio::ip::address multicastAddress_;
    boost::asio::ip::udp::endpoint endpoint_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::strand strand_;
    boost::asio::deadline_timer timer_;

    CommandArgParser commandArgParser_;
    FILE * dataFile_;

    typedef std::pair<size_t, size_t> MessagePosition; // position in file: start, length
    typedef std::vector<MessagePosition> MessageIndex;
    MessageIndex messageIndex_;

    boost::scoped_array<unsigned char> buffer_;
    size_t bufferSize_;
    size_t nPass_;
    size_t nMsg_;
    size_t totalMessageCount_;
  };
}
#endif // FILETOMULTICAST_H
