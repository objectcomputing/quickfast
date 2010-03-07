// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#include "QuickFASTDotNetPch.h"
#include "DNDecoderConnection.h"
#pragma unmanaged
#include <Application/DecoderConnection.h>
#include <Communication/Receiver.h>
#pragma managed

using namespace QuickFAST;
using namespace DotNet;


DNDecoderConnection::DNDecoderConnection()
  : configuration_(new Application::DecoderConfiguration)
  , connection_(new Application::DecoderConnection)
{
}

DNDecoderConnection::!DNDecoderConnection()
{
  this->~DNDecoderConnection();
}

DNDecoderConnection::~DNDecoderConnection()
{
  delete connection_;
  connection_ = 0;
  delete configuration_;
  configuration_ = 0;
}

void
DNDecoderConnection::run(DNMessageBuilder ^ dnBuilder, size_t extraThreadCount, bool useThisThread)
{
  try
  {
    Messages::ValueMessageBuilder & builder = dnBuilder->builder();
    connection_->configure(builder, *configuration_);
    connection_->receiver().runThreads(extraThreadCount, useThisThread);
  }
  catch (std::exception const & ex)
  {
    throw gcnew System::Exception(gcnew System::String(ex.what()));
  }
}
