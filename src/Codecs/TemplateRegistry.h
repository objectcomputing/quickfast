// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif

#ifndef TEMPLATEREGISTRY_H
#define TEMPLATEREGISTRY_H

#include "TemplateRegistry_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>
#include <Codecs/SchemaElement.h>
#include <Codecs/Template_fwd.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief A registry of known templates.
    ///
    /// The TemplateRegistry is a collection of Template objects. It is
    /// searchable by template ID or template name.  It is also possible
    /// to iterate through the templates using the stl-like begin() and end()
    /// methods.
    ///
    /// Normally the Template Registry will be initialized by reading an
    /// XML templates file. This is done by a QuickFAST::Util::XMLTemplateParser object.
    class QuickFAST_Export TemplateRegistry : public SchemaElement
    {
    public:
      /// @brief A map from template id to the template definition
      typedef std::map<template_id_t, TemplateCPtr> TemplateIdMap;
      /// @brief A map from template name to the template definition
      typedef std::map<std::string, TemplateCPtr> TemplateNameMap;
      /// @brief Iterator for a TemplateIdMap
      typedef TemplateIdMap::iterator iterator;
      /// @brief Iterator for a TemplateIdMap
      typedef TemplateIdMap::const_iterator const_iterator;

      /// @brief Construct an empty registry.
      TemplateRegistry();

      /// @brief special constructor for test/debug.
      /// DO NOT USE IN PRODUCTION CODE
      /// @param pmapBits how many pmap bits are needed by the largest template
      /// @param fieldCount how many fields are defined by the largest template
      /// @param dictionarySize how many slots are needed in the dictionary
      TemplateRegistry(
        size_t pmapBits,
        size_t fieldCount,
        size_t dictionarySize);

      /// @brief Virtual destructor.
      virtual ~TemplateRegistry()
      {
      }

      /// @brief Add a definition to the registry
      /// @param value smart pointer to the template to be added
      virtual void addTemplate(TemplatePtr value);

      /// @brief do any final processing after parsing is complete.
      virtual void finalize();

      /// @brief How many templates are defined?
      /// @return the count of known templates.
      size_t size()const;

      /// @brief What is the largest number of PresenceMap bits needed?
      /// @returns the number of bits needed in the largest presence map for these templates
      size_t presenceMapBits()const
      {
        return presenceMapBits_;
      }

      /// @brief How many entries are needed in the dictionaries associated with this registry
      /// @brief returns an count of dictionary indexes used.
      size_t dictionarySize() const
      {
        return dictionarySize_;
      }

      /// @brief Returns the maximum number of fields that will be produced by any template in the registry.
      ///
      /// Does not include "nested" fields -- unmerged groups and sequences count as one each.
      /// @returns the number of fields.
      size_t maxFieldCount()const
      {
        return maxFieldCount_;
      }

      /// @brief Use Template ID to find a template.
      /// @param[in] templateId the desired template
      /// @param[out] valueFound is the result of the search if return is true
      /// @returns true if the template was found.
      bool getTemplate(uint32 templateId, TemplateCPtr & valueFound)const;

      /// @brief Find a template by name.
      /// @param[in] name the desired template
      /// @param[in] templateNamespace in which name is defined.
      /// @param[out] valueFound is the result of the search if return is true
      /// @returns true if the template was found.
      bool findNamedTemplate(
        const std::string & name,
        const std::string & templateNamespace,
        TemplateCPtr & valueFound)const;

      /// @brief Find a template by name (mutable version)
      /// @param[in] name the desired template
      /// @param[in] templateNamespace in which name is defined.
      /// @param[out] valueFound is the result of the search if return is true
      /// @returns true if the template was found.
      bool findNamedTemplate(
        const std::string & name,
        const std::string & templateNamespace,
        TemplatePtr & valueFound);

      /// @brief Support constant iteration over known templates.
      /// @returns a pointer to the first template in the set
      const_iterator begin()const
      {
        return templates_.begin();
      }

      /// @brief Support constant iteration over known templates.
      /// @returns a pointer to the end of the set
      const_iterator end() const
      {
        return templates_.end();
      }

      /// @brief Support iteration over known templates.
      /// @returns a pointer to the first template in the set
      iterator begin()
      {
        return templates_.begin();
      }

      /// @brief Support iteration over known templates.
      /// @returns a pointer to the end of the set
      iterator end()
      {
        return templates_.end();
      }

      /// @brief set the name for this registry
      void setName(const std::string & name)
      {
        name_ = name;
      }

      /// @brief set the namespace to qualify the registry name
      void setNamespace(const std::string & nameSpace)
      {
        namespace_ = nameSpace;
      }

      /// @brief set the namespace for templates within this registry
      void setTemplateNamespace(const std::string & nameSpace)
      {
        templateNamespace_ = nameSpace;
      }

      /// @brief set the dictionary for templates within this registry
      void setDictionaryName(const std::string & dictionaryName)
      {
        dictionaryName_ = dictionaryName;
      }

      /// @brief get the name for this registry
      const std::string & getName() const
      {
        return name_;
      }

      /// @brief get the namespace to qualify the registry name
      const std::string & getNamespace() const
      {
        return namespace_;
      }

      /// @brief get the namespace for templates within this registry
      const std::string & getTemplateNamespace() const
      {
        return templateNamespace_;
      }

      /// @brief get the dictionary for templates within this registry
      const std::string & getDictionaryName() const
      {
        return dictionaryName_;
      }

      /// @brief Write the contents of the registry in human readable form.
      ///
      /// @param output is the stream to which the display will be written
      /// @param indent is the number of spaces to appear at the beginning of each line.
      void display(std::ostream & output, size_t indent = 0) const;

    private:
      // forbid copy constructor
      TemplateRegistry(const TemplateRegistry &);
      // forbid assignment
      TemplateRegistry & operator =(const TemplateRegistry &);

    private:
      TemplateIdMap templates_;

      TemplateNameMap namedTemplates_;

      typedef std::vector<TemplatePtr> MutableTemplates;
      MutableTemplates mutableTemplates_;
      size_t presenceMapBits_;
      size_t dictionarySize_;
      size_t maxFieldCount_;
      std::string name_;
      std::string namespace_;
      std::string templateNamespace_;
      std::string dictionaryName_;
    };
  }
}
#endif // TEMPLATEREGISTRY_H
