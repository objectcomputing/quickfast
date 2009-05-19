// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include <DotNet/Encoder.h>
#include <DotNet/TemplateRegistry.h>
#include <DotNet/DataDestination.h>
#include <DotNet/FieldSet.h>
#include <DotNet/Exceptions.h>
#include <Codecs/Encoder.h>

using namespace System;
using namespace System::IO;

namespace QuickFASTDotNet{
  namespace Codecs{

    Encoder::Encoder(TemplateRegistry^ templateRegistry, System::IO::Stream^ outStream)
      :outStream_(outStream)
      ,templateRegistry_(templateRegistry)
    {

    }

    void Encoder::Encode(unsigned int templateId, Messages::FieldSet^ message)
    {
      try
      {
        DataDestinationImpl dataDestination(outStream_);
        QuickFAST::Codecs::Encoder encoder(templateRegistry_->NativeTemplateRegistry);
        DataDestinationImpl dataDest(outStream_);
        encoder.encodeMessage(dataDest, templateId, message->FieldSetRef);
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
  }
}
