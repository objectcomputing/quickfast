// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDOPNOP_H
#define FIELDOPNOP_H
#include <Codecs/FieldOp.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief Dispatch to the appropriate method in a FieldInstruction when no operator is specified.
    class /*QuickFAST_Export */ FieldOpNop
      : public FieldOp
    {
    public:
      virtual bool usesPresenceMap(bool mandatory)const;
      virtual bool usesDictionary() const;

      virtual void decode(
        const Codecs::FieldInstruction & instruction,
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & fieldSet) const;

      virtual void encode(
        const Codecs::FieldInstruction & instruction,
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const;
      virtual void setDefaultValue(
        Codecs::FieldInstruction & instruction) const;

      virtual OpType opType()const;
    };

    inline
    bool
    FieldOpNop::usesPresenceMap(bool /*mandatory*/)const
    {
      return false;
    }
    inline
    bool
    FieldOpNop::usesDictionary() const
    {
      return false;
    }

    inline
    FieldOp::OpType
    FieldOpNop::opType()const
    {
      return NOP;
    }

    inline
    void
    FieldOpNop::decode(
      const Codecs::FieldInstruction & instruction,
      Codecs::DataSource & source,
      Codecs::PresenceMap & pmap,
      Codecs::Decoder & decoder,
      Messages::ValueMessageBuilder & fieldSet) const
    {
      instruction.decodeNop(source, pmap, decoder, fieldSet);
    }

    inline
    void
    FieldOpNop::encode(
      const Codecs::FieldInstruction & instruction,
      Codecs::DataDestination & destination,
      Codecs::PresenceMap & pmap,
      Codecs::Encoder & encoder,
      const Messages::MessageAccessor & fieldSet) const
    {
      return instruction.encodeNop(destination, pmap, encoder, fieldSet);
    }

    inline
    void
    FieldOpNop::setDefaultValue(
      Codecs::FieldInstruction & /*instruction*/) const
    {
    }

  }
}
#endif // FIELDOPNOP_H
