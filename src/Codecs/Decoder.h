// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DECODER_H
#define DECODER_H
#include "Decoder_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Codecs/Context.h>
#include <Codecs/DataSource_fwd.h>
#include <Codecs/PresenceMap_fwd.h>
#include <Codecs/Template.h>
#include <Codecs/SegmentBody_fwd.h>
#include <Messages/ValueMessageBuilder_fwd.h>

#include <Common/Exceptions.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief Decode incoming FAST messages.
    ///
    /// Create an instance of the Decoder providing a registry of the templates
    /// to be used to decode the message, then call decodeMessage to decode
    /// each message from a DataSource.
    class QuickFAST_Export Decoder : public Context
    {
    public:
      /// @brief Construct with a TemplateRegistry containing all templates to be used.
      /// @param registry A registry containing all templates to be used to decode messages.
      explicit Decoder(TemplateRegistryPtr registry);

      /// @brief Decode the next message.
      /// @param[in] source where to read the incoming message(s).
      /// @param[out] message an empty message into which the decoded fields will be stored.
      void decodeMessage(
        DataSource & source,
        Messages::ValueMessageBuilder & message);

      /// @brief Decode a group field.
      ///
      /// If the application type of the group matches the application type of the
      /// messageBuilder parameter then fields in the group will be added to the messageBuilder
      /// If they differ then a GroupField will be created and added to the messageBuilder,
      /// and the fields in the group will be added to the messageBuilder contained in the
      /// new GroupField.
      ///
      /// @param[in] source from which the FAST data comes
      /// @param[in] segment defines the group [a subset of a template]
      /// @param[in] messageBuilder to which the decoded fields will be added
      void
      decodeGroup(
        DataSource & source,
        const SegmentBodyCPtr & segment,
        Messages::ValueMessageBuilder & messageBuilder);

      /// @brief Decode a segment into a messageBuilder.
      ///
      /// @param[in] source supplies the FAST encoded data.
      /// @param[in] messageBuilder to which the decoded fields will be added
      /// @param[in] identity names the template.
      void decodeNestedTemplate(
        DataSource & source,
        Messages::ValueMessageBuilder & messageBuilder,
        const Messages::FieldIdentity & identity);

      /// @brief Decode the body of a segment into a messageBuilder.
      ///
      /// @param[in] source supplies the FAST encoded data.
      /// @param[in] pmap is used to determine which fields are present
      ///        in the input.
      /// @param[in] segment defines the expected fields [part of a template]
      /// @param[in] messageBuilder to which the decoded fields will be added
      void decodeSegmentBody(
        DataSource & source,
        PresenceMap & pmap,
        const SegmentBodyCPtr & segment,
        Messages::ValueMessageBuilder & messageBuilder);
    };
  }
}
#endif // DECODER_H
