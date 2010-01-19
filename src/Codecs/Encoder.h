// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ENCODER_H
#define ENCODER_H
#include "Encoder_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Codecs/Context.h>
#include <Codecs/DataDestination_fwd.h>
#include <Codecs/PresenceMap_fwd.h>
#include <Codecs/Template.h>
#include <Codecs/SegmentBody_fwd.h>
#include <Messages/MessageAccessor.h>

#include <Common/Exceptions.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief Encode incoming FAST messages.
    ///
    /// Create an instance of the Encoder providing a registry of the templates
    /// to be used to encode the message, then call encodeMessage to encode
    /// each message from a DataDestination.
    class QuickFAST_Export Encoder : public Context
    {
    public:
      /// @brief Construct with a TemplateRegistry containing all templates to be used.
      /// @param registry A registry containing all templates to be used to encode messages.
      Encoder(Codecs::TemplateRegistryPtr registry);

      /// @brief Encode the next message.
      /// @param[out] destination where to write the encoded messages.
      /// @param[in] templateId identifies the template to use for encoding.
      /// @param[in] message containing the fields to be encoded.
      void encodeMessage(
        DataDestination & destination,
        template_id_t templateId,
        const Messages::MessageAccessor & message);

      /// @brief Encode a group field.
      ///
      /// @param[in] destination to which FAST data goes.
      /// @param[in] group defines the group [a subset of a template]
      /// @param[in] fieldSet to which the encoded fields will be added
      void
      encodeGroup(
        DataDestination & destination,
        const Codecs::SegmentBodyPtr & group,
        const Messages::MessageAccessor & fieldSet);

      /// @brief Encode a segment into a fieldSet.
      ///
      /// @param[in] source supplies the FAST encoded data.
      /// @param[in] templateId identifies the template to be used during encoding
      /// @param[in] fieldSet to which the encoded fields will be added
      void encodeSegment(
        DataDestination & source,
        template_id_t templateId,
        const Messages::MessageAccessor & fieldSet);

      /// @brief Encode the body of a segment into a fieldSet.
      ///
      /// @param[in] destination receives the FAST encoded data.
      /// @param[in] presenceMap is used to determine which fields are present
      ///        in the input.
      /// @param[in] segment defines the expected fields [part of a template]
      /// @param[in] fieldSet to which the encoded fields will be added
      void encodeSegmentBody(
        DataDestination & destination,
        Codecs::PresenceMap & presenceMap,
        const Codecs::SegmentBodyCPtr & segment,
        const Messages::MessageAccessor & fieldSet);
    private:
    };
  }
}
#endif // ENCODER_H
