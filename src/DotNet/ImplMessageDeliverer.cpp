// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#include "QuickFASTDotNetPch.h"
#include "ImplMessageDeliverer.h"
#include <DotNet/StringConversion.h>
#include <DotNet/DNMessageDeliverer.h>

using namespace QuickFAST::DotNet;

ImplMessageDeliverer::ImplMessageDeliverer(DNMessageDeliverer ^ deliverer)
: deliverer_(deliverer)
{
}

ImplMessageDeliverer::~ImplMessageDeliverer()
{
}

bool
ImplMessageDeliverer::deliverMessage(ImplFieldSet & fieldSet)
{
    gcroot<DNMessage ^> message = gcnew DNMessage(fieldSet);
    bool result = deliverer_->signalMessageArrived(message);
    message->clear();
    return result;
}

bool
ImplMessageDeliverer::logMessage(unsigned short level, const std::string & logMessage)
{
  System::String ^ value = string_cast(logMessage);
  return deliverer_->signalLogMessage(level, value);
}

bool
ImplMessageDeliverer::reportDecodingError(const std::string & errorMessage)
{
  System::String ^ value = string_cast(errorMessage);
  return deliverer_->signalDecodingError(value);
}

bool
ImplMessageDeliverer::reportCommunicationError(const std::string & errorMessage)
{
  System::String ^ value = string_cast(errorMessage);
  return deliverer_->signalCommunicationError(value);
}
