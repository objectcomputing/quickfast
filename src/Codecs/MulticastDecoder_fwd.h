// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef MULTICASTDECODER_FWD_H
#define MULTICASTDECODER_FWD_H
//#include <Messages/Message_fwd.h>
#include <Codecs/GenericMessageBuilder.h>
#include <Codecs/MessageConsumer.h>
#include <Codecs/DataSourceBuffer.h>

namespace QuickFAST{
  namespace Codecs{
    template<typename MessageType, typename MessageConsumerType, typename DataSourceType = DataSourceBuffer>
    class MulticastDecoderT;

    typedef MulticastDecoderT<
      Codecs::GenericMessageBuilder, Codecs::MessageConsumer> MulticastDecoder;
  }
}
#endif // MULTICASTDECODER_FWD_H
