// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDIDENTITY_H
#define FIELDIDENTITY_H
#include "FieldIdentity_fwd.h"
#include <Common/Types.h>

namespace QuickFAST{
  namespace Messages{
    /// @brief Identify a field within a FieldSet
    ///
    /// Keeping the field's identity separate from it's type and value allows
    /// immutable fields to be shared among different field containers (dictionaries & field sets, &tc.)
    class QuickFAST_Export FieldIdentity
    {
    public:
      /// @brief Construct the FieldIdentity
      /// @param name the localname for the field
      /// @param fieldNamespace the namespace in which the localname is defined
      /// @param id the optional field ID for this field.
      explicit FieldIdentity(
        const std::string & name,
        const std::string & fieldNamespace = "",
        const field_id_t & id = "");

      /// @brief Construct an anonomous FieldIdentity
      FieldIdentity();
      ~FieldIdentity();
    public:
      /// @brief set name after construction
      /// @param name the localname for the field
      void setName(const std::string & name)
      {
        localName_ = name;
        qualifyName();
      }

      /// @brief Set Namespace after construction
      /// @param fieldNamespace the namespace in which the localname is defined
      void setNamespace(const std::string & fieldNamespace)
      {
        fieldNamespace_ = fieldNamespace;
        qualifyName();
      }

      /// @brief Copy construct the FieldIdentity
      /// @param rhs is the FieldIdentity from which to copy
      FieldIdentity(const FieldIdentity & rhs)
        : localName_(rhs.localName_)
        , fieldNamespace_(rhs.fieldNamespace_)
        , fullName_(rhs.fullName_)
        , id_(rhs.id_)
      {
      }

      /// @brief Set the fields ID (not terribly useful)
      /// @param id to be stored
      void setId(const field_id_t & id)
      {
        id_ = id;
      }

      /// @brief get the fully qualified name of the field.
      /// @returns the name qualified by the namespace (from the cached value)
      const std::string & name()const
      {
        return fullName_;
      }

      /// @brief get the localname of a field
      /// @returns the unqualified name
      const std::string & getLocalName()const
      {
        return localName_;
      }

      /// @brief get the namespace of a field
      /// @returns the namespace
      const std::string & getNamespace()const
      {
        return fieldNamespace_;
      }

      /// @brief get the field id
      /// @returns the id assigned to this field.
      const field_id_t & id()const
      {
        return id_;
      }

      ///@brief Debug: Display the identity on the given output stream.
      /// @param output is where to write the human-readable representation of the identity.
      void display(std::ostream & output)const;

      /// @brief Compare identities
      ///
      /// Equality means names, namespaces, and possibly ids are equal.
      /// ids are considered only if both are specified.
      /// @param rhs is the identity to be compared to this.
      bool operator == (const FieldIdentity & rhs) const
      {
        return(
          (fieldNamespace_ == rhs.fieldNamespace_) &&
          (fullName_ == rhs.fullName_) &&
          (id_.empty() || rhs.id_.empty() || id_ == rhs.id_));
      }

      /// @brief Compare identities
      /// @param rhs is the identity to be compared to this.
      bool operator != (const FieldIdentity & rhs) const
      {
        return ! this->operator==(rhs);
      }

    private:
      void qualifyName()
      {
        if(fieldNamespace_.empty())
        {
          fullName_ = localName_;
        }
        else
        {
          fullName_ = fieldNamespace_ + "::" + localName_;
        }
      }
    private:
      std::string localName_;
      std::string fieldNamespace_;
      std::string fullName_; // cached for performance
      field_id_t id_;
    };
  }
}
#endif // FIELDIDENTITY_H
