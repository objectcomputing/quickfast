// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#include "QuickFASTDotNetPch.h"
#include "DNMessageDeliverer.h"
#include <DotNet/ImplFieldSetBuilder.h>

#include <DotNet/StringConversion.h>
#include <DotNet/ImplFieldSet.h>

#pragma unmanaged
#include <Messages/FieldIdentity.h>
#include <Common/Value.h>
#pragma managed

using namespace QuickFAST;
using namespace DotNet;

///////////////////////
// DNMessageDeliverer

DNMessageDeliverer::DNMessageDeliverer()
: proxyDeliverer_(0)
, fieldSetBuilder_(0)
{
  // defer creating these to body of constructor to avoid
  //  warning C4355: 'this' : used in base member initializer list
  proxyDeliverer_ = new ImplMessageDeliverer(this);
  fieldSetBuilder_ = new ImplFieldSetBuilder(*proxyDeliverer_);
}

DNMessageDeliverer::~DNMessageDeliverer()
{
  release();
}

DNMessageDeliverer::!DNMessageDeliverer()
{
  release();
}

void
DNMessageDeliverer::release()
{
  delete fieldSetBuilder_;
  fieldSetBuilder_ = 0;
}

Messages::ValueMessageBuilder &
DNMessageDeliverer::builder()
{
  return *fieldSetBuilder_;
}


bool
DNMessageDeliverer::signalMessageArrived(DNMessage ^ message)
{
  MessageReceived(this, message);
  return true;
}

bool
DNMessageDeliverer::signalLogMessage(unsigned short level, System::String ^ message)
{
  LogMessage(this, level, message);
  return true;
}

bool
DNMessageDeliverer::signalCommunicationError(System::String ^ message)
{
  CommunicationError(this, message);
  return true;
}

bool
DNMessageDeliverer::signalDecodingError(System::String ^ message)
{
  DecodingError(this, message);
  return true;
}

void
DNMessageDeliverer::signalReceiverStarted()
{
  ReceiverStarted(this);
}

void
DNMessageDeliverer::signalReceiverStopped()
{
  ReceiverStopped(this);
}

void
DNMessageDeliverer::setLogLimit(unsigned short logLimit)
{
  fieldSetBuilder_->setLogLimit(logLimit);
}
