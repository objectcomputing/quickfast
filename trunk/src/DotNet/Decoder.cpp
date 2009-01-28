// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include "Decoder.h"
#include "Message.h"
#include "TemplateRegistry.h"
#include "Exceptions.h"

using namespace System;
using namespace System::IO;


namespace QuickFASTDotNet{
  namespace Codecs{

    Decoder::Decoder(TemplateRegistry^ templateRegistry, System::IO::Stream^ instream):
        stream_(instream),
        endOfStream_(false),
        templateRegistry_(templateRegistry),
        dataSource_(new DotNetDataSource(instream)),
        decoder_(new QuickFAST::Codecs::Decoder(templateRegistry->NativeTemplateRegistry))
    {

    }

    QuickFASTDotNet::Messages::Message^ Decoder::Decode()
    {
      try
      {
        typedef QuickFASTDotNet::Messages::Message FTManagedMessage;
        FTManagedMessage^ message = gcnew FTManagedMessage;
        decoder_->decodeMessage(dataSource_.GetRef(), message->MessageRef);

        return message;
      }
      catch(const QuickFAST::UnsupportedConversion& error)
      {
        throw gcnew UnsupportedConversion(error);
      }
      catch(const QuickFAST::OverflowError& error)
      {
        throw gcnew OverflowError(error);
      }
      catch(const QuickFAST::FieldNotPresent& error)
      {
        throw gcnew FieldNotPresent(error);
      }
      catch(const QuickFAST::EncodingError& error)
      {
        throw gcnew EncodingError(error);
      }
      catch(const QuickFAST::TemplateDefinitionError& error)
      {
        throw gcnew TemplateDefinitionError(error);
      }
      catch(const QuickFAST::UsageError& error)
      {
        throw gcnew UsageError(error);
      }
    }

    void Decoder::Decode(MessageReceivedDelegate^ callback)
    {
      throw gcnew System::NotImplementedException();
    }
  }
}
