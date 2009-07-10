// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONTEMPLATEREF_H
#define FIELDINSTRUCTIONTEMPLATEREF_H
#include <Codecs/FieldInstruction.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief Implement &lt;templateRef> field instruction.
    class QuickFAST_Export FieldInstructionTemplateRef : public FieldInstruction
    {
    public:
      /// @brief construct with a name and a namespace
      /// @param name is the local name
      /// @param fieldNamespace is the namespace to qualify this name
      FieldInstructionTemplateRef(
        const std::string & name,
        const std::string & fieldNamespace);

      /// @brief construct anonomous field instruction
      FieldInstructionTemplateRef();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionTemplateRef();

      /// @brief set name
      void setTemplateName(const std::string & templateName)
      {
        templateName_ = templateName;
      }

      /// @brief set namespace
      void setTemplateNamespace(const std::string & templateNamespace)
      {
        templateNamespace_ = templateNamespace;
      }

      virtual size_t fieldCount(const SegmentBody & parent)const;

      virtual bool decodeNop(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::MessageBuilder & fieldSet) const;

      virtual void encodeNop(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const;

    private:
      void interpretValue(const std::string & value);

    private:
      std::string templateName_;
      std::string templateNamespace_;
    };
  }
}
#endif // FIELDINSTRUCTIONTEMPLATEREF_H
