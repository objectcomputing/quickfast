// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGEINTERPRETER_H
#define MESSAGEINTERPRETER_H
#include <Codecs/MessageConsumer.h>
#include <Messages/MessageFormatter.h>

namespace QuickFAST{
  namespace Examples{

    /// @brief A message consumer that attempts to produce a human readable version
    /// of a message that has been decoded by QuickFAST.
    class MessageInterpreter : public Codecs::MessageConsumer
    {
    public:
      /// @brief Construct given a ostream to which to write the interpreted results.
      MessageInterpreter(std::ostream & out, bool silent = false);
      virtual ~MessageInterpreter();

      /// @brief set the level of log messages that we are interested in.
      /// @param level is the first level that will *NOT* be displayed.
      void setLogLevel(Common::Logger::LogLevel level);

      ////////////////////////////
      // Implement MessageConsumer
      virtual bool consumeMessage(Messages::Message & message);
      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);
      virtual void decodingStarted();
      virtual void decodingStopped();

    private:
      Messages::MessageFormatter formatter_;
      std::ostream & out_;
      size_t indent_;
      size_t recordCount_;
      Common::Logger::LogLevel logLevel_;
      bool silent_;
    };
  }
}
#endif /* MESSAGEINTERPRETER_H */
