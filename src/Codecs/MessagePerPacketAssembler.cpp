// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include <Communication/Receiver.h>
#include "MessagePerPacketAssembler.h"
#include <Messages/ValueMessageBuilder.h>
#include <Codecs/Decoder.h>

using namespace QuickFAST;
using namespace Codecs;

MessagePerPacketAssembler::MessagePerPacketAssembler(
      TemplateRegistryPtr templateRegistry,
      HeaderAnalyzer & packetHeaderAnalyzer,
      HeaderAnalyzer & messageHeaderAnalyzer,
      Messages::ValueMessageBuilder & builder)
  : BasePacketAssembler(
    templateRegistry, packetHeaderAnalyzer,
    messageHeaderAnalyzer,
    builder)
{
}

MessagePerPacketAssembler::~MessagePerPacketAssembler()
{
}

bool
MessagePerPacketAssembler::serviceQueue(Communication::Receiver & receiver)
{
  bool result = true;
  Communication::LinkedBuffer * buffer = receiver.getBuffer(false);
  while(result && buffer != 0)
  {
    try
    {
      result = decodeBuffer(buffer->get(), buffer->used());
    }
    catch(const std::exception &ex)
    {
      receiver.releaseBuffer(buffer);
      buffer = 0;
      result = reportDecodingError(ex.what());
      reset();
    }
    if(buffer != 0)
    {
      receiver.releaseBuffer(buffer);
      buffer = 0;
    }
    if(result)
    {
      buffer = receiver.getBuffer(false);
    }
  }
  return result;
}

