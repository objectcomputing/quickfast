// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef SPECIALACCESSORS_H
#define SPECIALACCESSORS_H
#include <Messages/MessageAccessor.h>
#include <Messages/MessageField.h>

namespace QuickFAST
{
  namespace Messages
  {
    /// @brief A MessageAccessor for an empty record
    ///
    class EmptyAccessor : public Messages::MessageAccessor
    {
      /////////////
      // Implement MessageAccessor
      virtual const Messages::MessageField & operator[](size_t /*index*/)const
      {
        throw EncodingError("Indexing empty message.");
      }

      virtual bool isPresent(const std::string & /*name*/)const
      {
        return false;
      }

      virtual bool getField(const std::string &/*name*/, Messages::FieldCPtr & /*value*/) const
      {
        return false;
      }

      virtual bool getIdentity(const std::string &/*name*/, Messages::FieldIdentityCPtr & /*identity*/) const
      {
        return false;
      }

      virtual const_iterator begin() const
      {
        return 0;
      }

      virtual const_iterator end() const
      {
        return 0;
      }

      virtual const std::string & getApplicationType()const
      {
        return nada_;
      }
      virtual const std::string & getApplicationTypeNs()const
      {
        return nada_;
      }

    private:
      std::string nada_;

    };

    ///@brief A MessageAccessor for a "record" containing a single field
    class SingleFieldAccessor : public Messages::MessageAccessor
    {
    public:
      /// @brief construct with the identity and value of the field to be contained in the "record"
      SingleFieldAccessor(
          Messages::FieldIdentityCPtr & identity,
          const Messages::FieldCPtr & field)
        : messageField_(identity, field)
      {
      }

      /////////////
      // Implement MessageAccessor
      virtual const Messages::MessageField & operator[](size_t /*index*/)const
      {
        return messageField_;
      }

      virtual bool isPresent(const std::string & /*name*/)const
      {
        // we could check
        return true;
      }

      virtual bool getField(const std::string &/*name*/, Messages::FieldCPtr & value) const
      {
        value = messageField_.getField();
        return true;
      }

      virtual bool getIdentity(const std::string &/*name*/, Messages::FieldIdentityCPtr & identity) const
      {
        identity = messageField_.getIdentity();
        return true;
      }

      virtual const_iterator begin() const
      {
        return &messageField_;
      }

      virtual const_iterator end() const
      {
        return (&messageField_) + 1;
      }

      virtual const std::string & getApplicationType()const
      {
        return nada_;
      }
      virtual const std::string & getApplicationTypeNs()const
      {
        return nada_;
      }

    private:
      Messages::MessageField messageField_;
      std::string nada_;
    };
  }
}
#endif // SPECIALACCESSORS_H
