// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTINTERPRETER_H
#define MULTICASTINTERPRETER_H
#include <Codecs/DataSource.h>
#include <Codecs/Decoder_fwd.h>

#include <Examples/CommandArgParser.h>
#include <boost/asio.hpp>
#include <Codecs/MessageConsumer_fwd.h>

namespace QuickFAST{
  class MulticastInterpreter : public CommandArgHandler
  {
    typedef boost::scoped_array<unsigned char> Buffer;
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
    void handleReceive(
      const boost::system::error_code& error,
      Buffer * data,
      size_t bytesReceived);

    void startReceive(Buffer * data);

  private:
    size_t bufferSize_;
    std::string templateFileName_;
    std::ifstream templateFile_;
    std::string outputFileName_;
    std::ostream * outputFile_;
    std::string echoFileName_;
    std::ostream * echoFile_;
    Codecs::DataSource::EchoType echoType_;
    bool echoMessage_;
    bool echoField_;
    size_t messageLimit_;
    bool verboseDecode_;
    bool verboseExecution_;
    bool strict_;

    unsigned short portNumber_;
    std::string listenAddressName_;
    std::string multicastAddressName_;

    boost::asio::io_service ioService_;
    boost::asio::strand strand_;
    boost::asio::ip::address listenAddress_;
    boost::asio::ip::address multicastAddress_;
    boost::asio::ip::udp::endpoint endpoint_;
    boost::asio::ip::udp::endpoint senderEndpoint_;
    boost::asio::ip::udp::socket socket_;

    Codecs::Decoder * decoder_;
    CommandArgParser commandArgParser_;
    Codecs::MessageConsumer * consumer_;

    size_t inputCount_;
    Buffer data_;
    Buffer data2_;
  };
}
#endif // MULTICASTINTERPRETER_H
