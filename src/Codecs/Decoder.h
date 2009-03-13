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
#include <Messages/DecodedFields_fwd.h>
//#include <Messages/FieldSet_fwd.h>

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

      /// @brief Enable/disable strict checking of conformance to the FAST standard
      ///
      /// If strict is false some conditions such as integer overflow or inefficient encoding
      /// will be ignored.  The default is true -- strict checking is enabled.
      /// @param strict true to enable; false to disable strict checking
      void setStrict(bool strict)
      {
        strict_ = strict;
      }

      /// @brief get the current status of the strict property.
      /// @returns true if strict checking is enabled.
      bool getStrict()const
      {
        return strict_;
      }
      /// @brief Decode the next message.
      /// @param[in] source where to read the incoming message(s).
      /// @param[out] message an empty message into which the decoded fields will be stored.
      /// @return true if a message was successfully decoded.  False normally means end of file.
      bool decodeMessage(
        DataSource & source,
        Messages::DecodedFields & message);

      /// @brief Decode a group field.
      ///
      /// If the application type of the group matches the application type of the
      /// fieldSet parameter then fields in the group will be added to the fieldSet
      /// If they differ then a GroupField will be created and added to the fieldSet,
      /// and the fields in the group will be added to the fieldSet contained in the
      /// new GroupField.
      ///
      /// @param[in] source from which the FAST data comes
      /// @param[in] segment defines the group [a subset of a template]
      /// @param[in] fieldSet to which the decoded fields will be added
      void
      Decoder::decodeGroup(
        DataSource & source,
        SegmentBodyCPtr segment,
        Messages::DecodedFields & fieldSet);

      /// @brief Decode a segment into a fieldSet.
      ///
      /// @param[in] source supplies the FAST encoded data.
      /// @param[in] fieldSet to which the decoded fields will be added
      bool decodeSegment(
        DataSource & source,
        Messages::DecodedFields & fieldSet);

      /// @brief Decode the body of a segment into a fieldSet.
      ///
      /// @param[in] source supplies the FAST encoded data.
      /// @param[in] pmap is used to determine which fields are present
      ///        in the input.
      /// @param[in] segment defines the expected fields [part of a template]
      /// @param[in] fieldSet to which the decoded fields will be added
      bool decodeSegmentBody(
        DataSource & source,
        PresenceMap & pmap,
        SegmentBodyCPtr segment,
        Messages::DecodedFields & fieldSet);

    private:
      /// false makes the decoder more forgiving
      bool strict_;
    };
  }
}
#endif // DECODER_H
