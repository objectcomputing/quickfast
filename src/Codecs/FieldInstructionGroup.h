// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONGROUP_H
#define FIELDINSTRUCTIONGROUP_H
#include <Codecs/FieldInstruction.h>
#include <Codecs/SegmentBody_fwd.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief Instruction to encode/decode a group.
    ///
    /// Groups are constructed by capturing field definitions
    /// into a SegmentBody then adding that segment to the Group
    /// via the setSegmentBody() method.
    ///
    /// Groups guide decoding by implementing decodeNop.  It uses
    /// the segment to decode fields into the currently active Message Builder.
    ///
    /// An attempt to use any other instruction with a Group
    /// will lead to a TemplateDefinitionError exception being thrown.
    class QuickFAST_Export FieldInstructionGroup : public FieldInstruction
    {
    public:
      /// @brief construct with a name and a namespace
      /// @param name is the local name
      /// @param fieldNamespace is the namespace to qualify this name
      FieldInstructionGroup(
        const std::string & name,
        const std::string & fieldNamespace);

      /// @brief construct anonomous field instruction
      FieldInstructionGroup();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionGroup();

      /// @brief define the segment body of this group
      void setSegmentBody(Codecs::SegmentBodyPtr segmentBody)
      {
        segmentBody_ = segmentBody;
      }

      /// @brief how many fields are contained in this group?
      /// @param parent allows checking to see if group will be merged into parent segment
      virtual size_t fieldCount(const SegmentBody & parent)const;

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

      virtual bool getSegmentBody(Codecs::SegmentBodyPtr & segment) const
      {
        segment = segmentBody_;
        return bool(segmentBody_);
      }
      virtual ValueType::Type fieldInstructionType()const;
      virtual void displayBody(std::ostream & output, size_t indent)const;

    private:
      void interpretValue(const std::string & value);
    private:
      Codecs::SegmentBodyPtr segmentBody_;
    };
  }
}
#endif // FIELDINSTRUCTIONGROUP_H
