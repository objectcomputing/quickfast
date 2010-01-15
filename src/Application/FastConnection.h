// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef FASTCONNECTION_H
#define FASTCONNECTION_H

#include <Common/QuickFAST_Export.h>
#include <Codecs/MulticastDecoder.h>
#include <Codecs/MessageConsumer_fwd.h>
#include <Codecs/DataSource.h>
#include <Codecs/TemplateRegistry.h>

namespace QuickFAST
{
  namespace Application
  {
    ///@brief This class is a facade around QuickFAST intended to make it easy to create the most commonly needed applications.
    ///
    /// It supports two types of data source: <ul>
    ///  <li>Reading FAST data from a PCap file or input stream</li>
    ///  <li>Accepting FAST data via multicast</li>
    /// </ul>
    /// Incoming data can be proceesed by: <ul>
    ///   <li>A Messages::ValueMessageBuilder which accepts each field as it is decoded (recommended).</li>
    ///   <li>A Codecs::MessageConsumer which accepts Generic messages.</li>
    /// </ul>
    ///
    /// To use a FastConnection<ul>
    ///   <li>Create the FastConnection</li>
    ///   <li>Prepare templates by either: <ul>
    ///     <li>Calling the FastConnection::parseTemplates() method to parse the templates, or</li>
    ///     <li>Calling the FastConnection::shareTemplates() method with an existing FastConnection to reuse the same set of templates.</li>
    ///   </ul>
    ///   <li>Process incoming FAST message by calling one of the FastConnection::run() methods.</li>
    ///   <li>Stop processing incoming messages by calling the FastConnection::stop() method.</li>
    ///   <li>Optional depending on the arguments to run:  Wait for threads to terminate by calling the FastConnection::join() method.</li>
    /// </ul>
    class QuickFAST_Export FastConnection
    {
    public:
      FastConnection();
      void parseTemplates(std::string const & templateFileName);
      void shareTemplates(FastConnection & sourceConnection);
      void shareTemplates(Codecs::TemplateRegistryPtr & templateRegistry);

      void decodeFile(
        std::string const & filename,
        bool isBlocked,
        Messages::ValueMessageBuilder & builder);

      void decodeFile(
        std::istream & dataStream,
        bool isBlocked,
        Messages::ValueMessageBuilder & builder);

      void decodeFile(
        std::string const & filename,
        bool isBlocked,
        Codecs::MessageConsumer & handler);

      void decodeFile(
        std::istream & dataStream,
        bool isBlocked,
        Codecs::MessageConsumer & handler);

      void decodeFromDataSource(Codecs::DataSourcePtr & source, Messages::ValueMessageBuilder &builder);

      void listenForMulticast(
        std::string const & multicastAddressName,
        std::string const & listenAddressName,
        unsigned short portNumber);

      void run(
        Codecs::MessageConsumer & handler,
        size_t threadCount,
        bool useThisThread,
        size_t bufferSize,
        size_t bufferCount);

      void run(
        Messages::ValueMessageBuilder & builder,
        size_t threadCount,
        bool useThisThread,
        size_t bufferSize,
        size_t bufferCount);

      void stop();

      void joinThreads();

      void setVerbose(bool verbose);
      void setResetOnMessage(bool reset);
      void setStrict(bool strict);
      void setEcho(
        std::ostream * echoFile = 0,
        Codecs::DataSource::EchoType echoType = Codecs::DataSource::HEX,
        bool echoMessageBoundary = true,
        bool echoFieldBoundary = false);

    private:
      bool verbose_;
      bool resetOnMessage_;
      bool strict_;

      Codecs::TemplateRegistryPtr templateRegistry_;

      // control echoing of input
      // pass these parameters to DataSource
      std::ostream * echoFile_;
      Codecs::DataSource::EchoType echoType_;
      bool echoMessage_;
      bool echoField_;

      boost::scoped_ptr<Codecs::GenericMessageBuilder> builder_;
      boost::scoped_ptr<Codecs::MulticastDecoder> multicastDecoder_;
    };
  }
}
#endif // FASTCONNECTION_H
