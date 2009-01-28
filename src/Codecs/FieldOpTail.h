// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDOPTAIL_H
#define FIELDOPTAIL_H
#include "FieldOp.h"
namespace QuickFAST{
  namespace Codecs{
    /// @brief Dispatch the &lt;tail> operation to the appropriate method in a FieldInstruction.
    class QuickFAST_Export FieldOpTail : public FieldOp
    {
    public:
      virtual bool usesPresenceMap(bool mandatory)const;
      virtual bool decode(
        const Codecs::FieldInstruction & instruction,
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::FieldSet & fieldSet) const;

      virtual void encode(
        const Codecs::FieldInstruction & instruction,
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

      virtual void setDefaultValue(
        Codecs::FieldInstruction & instruction) const;
    };
  }
}
#endif // FIELDOPTAIL_H
