// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef TEMPLATE_H
#define TEMPLATE_H
#include "Template_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>
#include <Codecs/SegmentBody.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief The internal representation of a FAST template.
    ///
    /// Defines the fields that will be sent for a message when this
    /// template is used.
    class QuickFAST_Export Template : public SegmentBody
    {
    public:
      /// @brief Construct an empty template.
      Template()
        : SegmentBody(1) // init pmap bit count to one for the templateID
        , templateId_(0)
      {
      }

      /// @brief Destructor.
      virtual ~Template()
      {
      }

      /// @brief Set the templates identity
      /// @param id is the identifier for this template.
      void setId(template_id_t id)
      {
        templateId_ = id;
      }

      /// @brief Set the template name.
      /// Name is not as important as ID, but it is more human-friendly.
      /// @param name the name assigned to this template
      void setTemplateName(const std::string & name)
      {
        templateName_ = name;
      }

      /// @brief Set the template namespace.
      /// The namespace in which name is defined.
      /// @param templateNamespace the name assigned to this template
      void setTemplateNamespace(const std::string & templateNamespace)
      {
        templateNamespace_ = templateNamespace;
      }

      /// @brief Set the namespace for enclosed elements.
      /// The namespace for elements within this template
      /// @param templateNamespace for enclosed elements
      void setNamespace(const std::string & templateNamespace)
      {
        namespace_ = templateNamespace;
      }

      /// @brief Retrieve the template id
      /// @returns the template id.
      template_id_t getId()const
      {
        return templateId_;
      }

      /// @brief Retrieve the template's name
      /// @returns the name of this template.
      const std::string & getTemplateName()const
      {
        return templateName_;
      }

      /// @brief Retrieve the namespace to qualify the template name
      const std::string & getTemplateNamespace()const
      {
        return namespace_;
      }

      /// @brief Retrieve the namespace to qualify names of enclosed elements.
      const std::string & getNamespace()const
      {
        return namespace_;
      }

    private:
      uint32 templateId_;
      std::string templateName_;
      std::string templateNamespace_;
      std::string namespace_;
    };
  }
}
#endif // TEMPLATE_H
