// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONSEQUENCE_H
#define FIELDINSTRUCTIONSEQUENCE_H
#include <Codecs/FieldInstruction.h>
#include <Codecs/SegmentBody_fwd.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief Instruction to encode/decode a sequence.
    ///
    /// Sequences are constructed by capturing field definitions
    /// into a SegmentBody then adding that segment to the Sequence
    /// via the setSegmentBody() method.
    ///
    /// Sequences guide decoding by implementing decodeNop.  It uses
    /// the segment to decode fields into the currently active ValueMessageBuilder.
    ///
    /// An attempt to use any other instruction with a Sequence
    /// will lead to a TemplateDefinitionError exception being thrown.

    class QuickFAST_Export FieldInstructionSequence : public FieldInstruction
    {
    public:
      /// @brief construct with a name and a namespace
      /// @param name is the local name
      /// @param fieldNamespace is the namespace to qualify this name
      FieldInstructionSequence(
        const std::string & name,
        const std::string & fieldNamespace);

      /// @brief construct anonomous field instruction
      FieldInstructionSequence();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionSequence();

      void setSegmentBody(Codecs::SegmentBodyPtr segment)
      {
        segment_ = segment;
      }

      virtual bool getSegmentBody(Codecs::SegmentBodyPtr & segment) const
      {
        segment = segment_;
        return bool(segment);
      }

      virtual void decodeNop(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void encodeNop(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void indexDictionaries(
        DictionaryIndexer & indexer,
        const std::string & dictionaryName,
        const std::string & typeName,
        const std::string & typeNamespace);

      virtual void finalize(TemplateRegistry & templateRegistry);

      virtual ValueType::Type fieldInstructionType()const;
      virtual void displayBody(std::ostream & output, size_t indent)const;

    private:
      void interpretValue(const std::string & value);
    private:
      Codecs::SegmentBodyPtr segment_;
    };
  }
}
#endif // FIELDINSTRUCTIONSEQUENCE_H
