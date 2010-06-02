// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGEFORMATTER_H
#define MESSAGEFORMATTER_H
#include <Common/QuickFAST_EXPORT.h>
#include <Messages/FieldIdentity_fwd.h>
#include <Messages/Message_fwd.h>
#include <Messages/Field_fwd.h>

namespace QuickFAST
{
  namespace Messages
  {
    class QuickFAST_Export MessageFormatter
    {
    public:
      MessageFormatter(std::ostream & out);
      ~MessageFormatter();

      void formatMessage(const Messages::Message & message);
      void formatSequence(
        const Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & field);
      void formatGroup(
        const Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & field);

      void displayFieldValue(const Messages::FieldCPtr & field);
    private:
      void newline();
    private:
      std::ostream & out_;
      size_t indent_;
    };
  }
}
#endif /* MESSAGEFORMATTER_H */
