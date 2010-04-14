// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONMOCK_H
#define FIELDINSTRUCTIONMOCK_H
#include <Codecs/FieldInstruction.h>

namespace QuickFAST{
  namespace Tests{
    /// Mock Field instruction that counts function calls
    class FieldInstructionMock : public Codecs::FieldInstruction
    {
    public:
      FieldInstructionMock()
        : Codecs::FieldInstruction("mock","testing")
        , calledDecodeNop_(0)
        , calledDecodeConstant_(0)
        , calledDecodeDefault_(0)
        , calledDecodeCopy_(0)
        , calledDecodeDelta_(0)
        , calledDecodeIncrement_(0)
        , calledDecodeTail_(0)
        , calledInterpretValue_(0)
        , calledEncodeNop_(0)
        , calledEncodeConstant_(0)
        , calledEncodeDefault_(0)
        , calledEncodeCopy_(0)
        , calledEncodeDelta_(0)
        , calledEncodeIncrement_(0)
        , calledEncodeTail_(0)
      {
      }

      virtual void decodeNop(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & message) const
      {
        ++calledDecodeNop_;
      }

      virtual void decodeConstant(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & message) const
      {
        ++calledDecodeConstant_;
      }

      virtual void decodeDefault(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & message) const
      {
        ++calledDecodeDefault_;
      }

      virtual void decodeCopy(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & message) const
      {
        ++calledDecodeCopy_;
      }

      virtual void decodeDelta(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & message) const
      {
        ++calledDecodeDelta_;
      }

      virtual void decodeIncrement(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & message) const
      {
        ++calledDecodeIncrement_;
      }

      virtual void decodeTail(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & message) const
      {
        ++calledDecodeTail_;
      }

      virtual void encodeNop(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const
      {
        ++calledEncodeNop_;
      }

      virtual void encodeConstant(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const
      {
        ++calledEncodeConstant_;
      }

      virtual void encodeDefault(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const
      {
        ++calledEncodeDefault_;
      }

      virtual void encodeCopy(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const
      {
        ++calledEncodeCopy_;
      }

      virtual void encodeDelta(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const
      {
        ++calledEncodeDelta_;
      }

      virtual void encodeIncrement(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const
      {
        ++calledEncodeIncrement_;
      }

      virtual void encodeTail(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const
      {
        ++calledEncodeTail_;
      }

      virtual void interpretValue(const std::string & value)
      {
        ++calledInterpretValue_;
      }

      size_t readDecodeNop()
      {
        size_t result = calledDecodeNop_;
        calledDecodeNop_ = 0;
        return result;
      }

      size_t readDecodeConstant()
      {
        size_t result = calledDecodeConstant_;
        calledDecodeConstant_ = 0;
        return result;
      }

      size_t readDecodeDefault()
      {
        size_t result = calledDecodeDefault_;
        calledDecodeDefault_ = 0;
        return result;
      }

      size_t readDecodeDelta()
      {
        size_t result = calledDecodeDelta_;
        calledDecodeDelta_ = 0;
        return result;
      }

      size_t readDecodeCopy()
      {
        size_t result = calledDecodeCopy_;
        calledDecodeCopy_ = 0;
        return result;
      }

      size_t readDecodeIncrement()
      {
        size_t result = calledDecodeIncrement_;
        calledDecodeIncrement_ = 0;
        return result;
      }

      size_t readDecodeTail()
      {
        size_t result = calledDecodeTail_;
        calledDecodeTail_ = 0;
        return result;
      }

      size_t readInterpretValue()
      {
        size_t result = calledInterpretValue_;
        calledInterpretValue_ = 0;
        return result;
      }


      size_t readEncodeNop()
      {
        size_t result = calledEncodeNop_;
        calledEncodeNop_ = 0;
        return result;
      }

      size_t readEncodeConstant()
      {
        size_t result = calledEncodeConstant_;
        calledEncodeConstant_ = 0;
        return result;
      }

      size_t readEncodeDefault()
      {
        size_t result = calledEncodeDefault_;
        calledEncodeDefault_ = 0;
        return result;
      }

      size_t readEncodeDelta()
      {
        size_t result = calledEncodeDelta_;
        calledEncodeDelta_ = 0;
        return result;
      }

      size_t readEncodeCopy()
      {
        size_t result = calledEncodeCopy_;
        calledEncodeCopy_ = 0;
        return result;
      }

      size_t readEncodeIncrement()
      {
        size_t result = calledEncodeIncrement_;
        calledEncodeIncrement_ = 0;
        return result;
      }

      size_t readEncodeTail()
      {
        size_t result = calledEncodeTail_;
        calledEncodeTail_ = 0;
        return result;
      }

      virtual ValueType::Type fieldInstructionType()const
      {
        return ValueType::UNDEFINED;
      }

      bool isClear() const
      {
        return 0 ==
          calledDecodeNop_
          + calledDecodeConstant_
          + calledDecodeDefault_
          + calledDecodeCopy_
          + calledDecodeDelta_
          + calledDecodeIncrement_
          + calledDecodeTail_
          + calledInterpretValue_
          + calledEncodeNop_
          + calledEncodeConstant_
          + calledEncodeDefault_
          + calledEncodeCopy_
          + calledEncodeDelta_
          + calledEncodeIncrement_
          + calledEncodeTail_
    ;
      }

    private:
      mutable size_t calledDecodeNop_;
      mutable size_t calledDecodeConstant_;
      mutable size_t calledDecodeDefault_;
      mutable size_t calledDecodeCopy_;
      mutable size_t calledDecodeDelta_;
      mutable size_t calledDecodeIncrement_;
      mutable size_t calledDecodeTail_;
      mutable size_t calledInterpretValue_;

      mutable size_t calledEncodeNop_;
      mutable size_t calledEncodeConstant_;
      mutable size_t calledEncodeDefault_;
      mutable size_t calledEncodeCopy_;
      mutable size_t calledEncodeDelta_;
      mutable size_t calledEncodeIncrement_;
      mutable size_t calledEncodeTail_;
    };

  }
}

#endif /* FIELDINSTRUCTIONMOCK_H */
