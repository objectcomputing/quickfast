// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGECOUNTER_H
#define MESSAGECOUNTER_H
#include <Codecs/MessageConsumer.h>
#include <Messages/Field_fwd.h>
#include <Messages/FieldIdentity.h>
namespace QuickFAST{

  class MessageCounter : public Codecs::MessageConsumer
  {
  public:
    MessageCounter();
    virtual ~MessageCounter();

    virtual bool consumeMessage(Messages::Message & message);

    size_t getMesssageCount()const
    {
      return messageCount_;
    }
  private:
    size_t messageCount_;
  };
}
#endif /* MESSAGECOUNTER_H */
