// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONASCII_H
#define FIELDINSTRUCTIONASCII_H
#include "FieldInstruction.h"
namespace QuickFAST{
  namespace Codecs{
    /// @brief Implement &lt;string charset="ascii"> field instruction.
    class QuickFAST_Export FieldInstructionAscii
      : public FieldInstruction
    {
    public:
      /// @brief construct with a name and a namespace
      /// @param name is the local name
      /// @param fieldNamespace is the namespace to qualify this name
      FieldInstructionAscii(
        const std::string & name,
        const std::string & fieldNamespace);

      /// @brief construct anonomous field instruction
      FieldInstructionAscii();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionAscii();

      virtual bool decodeNop(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const;

      virtual bool decodeConstant(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const;

      virtual bool decodeDefault(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const;

      virtual bool decodeCopy(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const;

      virtual bool decodeDelta(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const;

      virtual bool decodeTail(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const;

      virtual void encodeNop(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

      virtual void encodeConstant(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

      virtual void encodeDefault(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

      virtual void encodeCopy(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

      virtual void encodeDelta(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

      virtual void encodeTail(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

    private:
     virtual bool decodeFromSource(
        Codecs::DataSource & source,
        bool mandatory,
        WorkingBuffer & buffer,
        Messages::FieldCPtr & field) const;

      void interpretValue(const std::string & value);
    private:
      Messages::FieldCPtr initialValue_;

    };
  }
}
#endif // FIELDINSTRUCTIONASCII_H
