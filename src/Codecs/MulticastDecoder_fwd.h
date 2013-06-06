// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MULTICASTDECODER_FWD_H
#define MULTICASTDECODER_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS
#include <Codecs/GenericMessageBuilder.h>
#include <Codecs/MessageConsumer.h>
#include <Codecs/DataSourceBuffer.h>

namespace QuickFAST{
  namespace Codecs{
    class MulticastDecoder;
  }
}
#endif // MULTICASTDECODER_FWD_H
