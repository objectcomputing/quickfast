// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGEFORMATTER_H
#define MESSAGEFORMATTER_H
#include <Common/QuickFASTPch.h>
#include <Common/QuickFAST_Export.h>
#include <Messages/FieldIdentity_fwd.h>
#include <Messages/Message_fwd.h>
#include <Messages/Field_fwd.h>

namespace QuickFAST
{
  namespace Messages
  {
    ///@brief Format message for display in human-readable format
    class QuickFAST_Export MessageFormatter
    {
    public:
      /// @brief construct
      /// @param out is the stream to which formatted data will be written
      MessageFormatter(std::ostream & out);
      ~MessageFormatter();

      /// @brief Format the entire message
      /// @param message is the message to be formatted
      void formatMessage(const Messages::Message & message);

      /// @brief Format a sequence
      /// @param identity identifies this sequence
      /// @param field is the field object containing the sequence
      void formatSequence(
        const Messages::FieldIdentity & identity,
        const Messages::FieldCPtr & field);
      /// @brief Format a group
      /// @param identity identifies this group
      /// @param field is the field object containing the group
      void formatGroup(
        const Messages::FieldIdentity & identity,
        const Messages::FieldCPtr & field);

      /// @brief Format a single field
      /// @param field contains the field to be formatted
      void displayFieldValue(const Messages::FieldCPtr & field);
    private:
      MessageFormatter & operator =(const MessageFormatter &); // do not autogenerate assignment operator
    private:
      void newline();
    private:
      std::ostream & out_;
      size_t indent_;
    };
  }
}
#endif /* MESSAGEFORMATTER_H */
