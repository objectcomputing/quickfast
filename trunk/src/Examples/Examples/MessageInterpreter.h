// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGEINTERPRETER_H
#define MESSAGEINTERPRETER_H
#include <Codecs/MessageConsumer.h>
#include <Messages/Field_fwd.h>
#include <Messages/FieldIdentity.h>
namespace QuickFAST{

  class MessageInterpreter : public Codecs::MessageConsumer
  {
  public:
    MessageInterpreter(std::ostream & out);
    virtual ~MessageInterpreter();

    virtual bool consumeMessage(Messages::Message & message);

    void formatMessage(Messages::Message & message);
    void formatSequence(
      const Messages::FieldIdentity & identity,
      Messages::FieldCPtr & field);
    void formatGroup(Messages::FieldCPtr & field);

    void displayFieldValue(Messages::FieldCPtr field);

  private:
    void newline();
  private:
    std::ostream & out_;
    size_t indent_;
    size_t recordCount_;
  };
}
#endif /* MESSAGEINTERPRETER_H */
