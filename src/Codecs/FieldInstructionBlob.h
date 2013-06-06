// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONBLOB_H
#define FIELDINSTRUCTIONBLOB_H
#include <Codecs/FieldInstruction.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief A basic implementation for ByteVector and Utf8 field Instructions.
    ///
    /// Implements &lt;byteVector> and &lt;string charset="unicode">
    ///
    class QuickFAST_Export FieldInstructionBlob
      : public FieldInstruction
    {
    public:
      /// @brief construct with a name and a namespace
      /// @param type the actual type of this field instruction (UTF8 or BYTEVECTOR)
      /// @param name is the local name
      /// @param fieldNamespace is the namespace to qualify this name
      FieldInstructionBlob(
        ValueType::Type type,
        const std::string & name,
        const std::string & fieldNamespace);

      /// @brief construct anonomous field instruction
      FieldInstructionBlob();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionBlob();

      virtual void decodeNop(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void decodeConstant(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void decodeDefault(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void decodeCopy(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void decodeDelta(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void decodeTail(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void encodeNop(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void encodeConstant(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void encodeDefault(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void encodeCopy(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void encodeDelta(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void encodeTail(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual ValueType::Type fieldInstructionType()const;

      virtual void addLengthInstruction(FieldInstructionPtr & field);

    protected:
      /// @brief create a populated field of the appropriate type
      /// @param buffer points to the data
      /// @param length gives the length of the data
      /// @returns a pointer to the heap-allocated field
      virtual Messages::FieldCPtr createField(const uchar * buffer, size_t length)const = 0;
      /// @brief create a populated field of the appropriate type
      /// @param value contains the data for the field
      /// @returns a pointer to the heap-allocated field
      virtual Messages::FieldCPtr createField(const std::string & value)const = 0;
      /// @brief create an empty field of the appropriate type
      /// @returns a pointer to the heap-allocated field
      virtual Messages::FieldCPtr createEmptyField()const = 0;

    protected:
      /// @brief interpret value set by the field operator at initialization time
      void interpretValue(const std::string & value);

      /// @brief helper routine to decode the blob data
      bool
      decodeBlobFromSource(
        Codecs::DataSource & source,
        Codecs::Context & context,
        bool mandatory,
        WorkingBuffer & buffer) const;

      /// @brief helper routine to encode a nullable, but not null value
      void encodeNullableBlob(
        Codecs::DataDestination & destination,
        Codecs::Context & context,
        WorkingBuffer & buffer,
        const StringBuffer & value) const;

      /// @brief helper routine to encode a non-nullable value
      void encodeBlob(
        Codecs::DataDestination & destination,
        WorkingBuffer & buffer,
        const StringBuffer & value) const;
    protected:
      /// @brief the actual data type (UTF8, BITVECTOR)
      ValueType::Type type_;
      /// @brief a field of the appropriate type containing the intial value specified with the field Op
      Messages::FieldCPtr initialValue_;
    };
  }
}
#endif // FIELDINSTRUCTIONBLOB_H
