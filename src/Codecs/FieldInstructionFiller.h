// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONFILLER_H
#define FIELDINSTRUCTIONFILLER_H
#include "FieldInstruction.h"
#include <Codecs/SegmentBody_fwd.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief Instruction to act as a placeholder in an ARCA template
    ///
    /// ARCA uses the presence map in nonstandard (actually pre-standard) ways.
    /// Presence map bits are assigned to fields that do not actually appear in
    /// the data stream or the application type.  The &less;filler> tag tells
    /// QuickFAST to consume these bits.
    class QuickFAST_Export FieldInstructionFiller : public FieldInstruction
    {
    public:
      /// @brief construct with a name and a namespace
      /// @param name is the local name
      /// @param fieldNamespace is the namespace to qualify this name
      FieldInstructionFiller(
        const std::string & name,
        const std::string & fieldNamespace);

      /// @brief construct anonomous field instruction
      FieldInstructionFiller();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionFiller();

      virtual size_t presenceMapBitsRequired()const;
      virtual size_t fieldCount(const SegmentBody & parent)const;

      virtual bool decodeNop(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const;

      virtual void encodeNop(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

      virtual void indexDictionaries(
        DictionaryIndexer & indexer,
        const std::string & dictionaryName,
        const std::string & typeName,
        const std::string & typeNamespace);

    private:
      virtual void interpretValue(const std::string & ignored);
    };
  }
}
#endif // FIELDINSTRUCTIONFILLER_H
