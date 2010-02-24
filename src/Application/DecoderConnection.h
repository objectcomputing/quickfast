// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef DECODERCONNECTION_H
#define DECODERCONNECTION_H
#include <Common/QuickFAST_Export.h>
#include <Messages/ValueMessageBuilder.h>

#include <Common/Exceptions.h>
#include <Codecs/TemplateRegistry_fwd.h>
#include <Codecs/HeaderAnalyzer_fwd.h>
#include <Communication/Assembler_fwd.h>
#include <Communication/Receiver_fwd.h>
#include <Application/DecoderConfiguration.h>

namespace QuickFAST{
  namespace Application{
    class QuickFAST_Export DecoderConnection
    {
    public:
      DecoderConnection();
      ~DecoderConnection();
      void configure(Messages::ValueMessageBuilder & builder, Application::DecoderConfiguration &configuration);

      Codecs::TemplateRegistryPtr & registry()
      {
        if(!registry_)
        {
          throw UsageError("Coding Error","Using DecoderConnection registry before it is configured.");
        }
        return registry_;
      }

      Codecs::HeaderAnalyzer & analyzer()const
      {
        if(!analyzer_)
        {
          throw UsageError("Coding Error","Using DecoderConnection analyzer before it is configured.");
        }
        return *analyzer_;
      }

      Communication::Assembler & assembler()const
      {
        if(!assembler_)
        {
          throw UsageError("Coding Error","Using DecoderConnection assembler before it is configured.");
        }
        return *assembler_;
      }

      Communication::Receiver & receiver()const
      {
        if(!receiver_)
        {
          throw UsageError("Coding Error","Using DecoderConnection receiver before it is configured.");
        }
        return *receiver_;
      }

    private:
      std::istream * fastFile_;
      std::ostream * echoFile_;
      std::ostream * verboseFile_ ;
      bool ownEchoFile_;
      bool ownVerboseFile_;

      Codecs::TemplateRegistryPtr registry_;
      boost::scoped_ptr<Codecs::HeaderAnalyzer> analyzer_;
      boost::scoped_ptr<Communication::Assembler> assembler_;
      boost::scoped_ptr<Communication::Receiver> receiver_;
    };
  }
}
#endif // DECODERCONNECTION_H
