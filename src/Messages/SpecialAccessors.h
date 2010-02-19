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
      virtual const Messages::MessageField & operator[](size_t index)const;
      virtual bool isPresent(const std::string & name)const;
      virtual bool getField(const std::string &name, Messages::FieldCPtr & value) const;
      virtual bool getIdentity(const std::string &name, Messages::FieldIdentityCPtr & identity) const;
      virtual const_iterator begin() const;
      virtual const_iterator end() const;
      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;

    private:
      std::string nada_;

    };

    inline const Messages::MessageField &
    EmptyAccessor::operator[](size_t /*index*/)const
    {
      throw EncodingError("Indexing empty message.");
    }

    inline bool
    EmptyAccessor::isPresent(const std::string & /*name*/)const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getField(const std::string &/*name*/, Messages::FieldCPtr & /*value*/) const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getIdentity(const std::string &/*name*/, Messages::FieldIdentityCPtr & /*identity*/) const
    {
      return false;
    }

    inline EmptyAccessor::const_iterator
    EmptyAccessor::begin() const
    {
      return 0;
    }

    inline EmptyAccessor::const_iterator
    EmptyAccessor::end() const
    {
      return 0;
    }

    inline const std::string &
    EmptyAccessor::getApplicationType()const
    {
      return nada_;
    }

    inline const std::string &
    EmptyAccessor::getApplicationTypeNs()const
    {
      return nada_;
    }


    ///@brief A MessageAccessor for a "record" containing a single field
    class SingleFieldAccessor : public Messages::MessageAccessor
    {
    public:
      /// @brief construct with the identity and value of the field to be contained in the "record"
      SingleFieldAccessor(
          Messages::FieldIdentityCPtr & identity,
          const Messages::FieldCPtr & field);

      /////////////
      // Implement MessageAccessor
      virtual const Messages::MessageField & operator[](size_t index)const;
      virtual bool isPresent(const std::string & name)const;
      virtual bool getField(const std::string &name, Messages::FieldCPtr & value) const;
      virtual bool getIdentity(const std::string &name, Messages::FieldIdentityCPtr & identity) const;
      virtual const_iterator begin() const;
      virtual const_iterator end() const;
      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;

    private:
      Messages::MessageField messageField_;
      std::string nada_;
    };

    inline
    SingleFieldAccessor::SingleFieldAccessor(
        Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & field)
      : messageField_(identity, field)
    {
    }

    inline const Messages::MessageField &
    SingleFieldAccessor::operator[](size_t /*index*/)const
    {
      return messageField_;
    }

    inline bool
    SingleFieldAccessor::isPresent(const std::string & /*name*/)const
    {
      // we could check
      return true;
    }

    inline bool
    SingleFieldAccessor::getField(const std::string &/*name*/, Messages::FieldCPtr & value) const
    {
      value = messageField_.getField();
      return true;
    }

    inline bool
    SingleFieldAccessor::getIdentity(const std::string &/*name*/, Messages::FieldIdentityCPtr & identity) const
    {
      identity = messageField_.getIdentity();
      return true;
    }

    inline SingleFieldAccessor::const_iterator
    SingleFieldAccessor::begin() const
    {
      return &messageField_;
    }

    inline SingleFieldAccessor::const_iterator
    SingleFieldAccessor::end() const
    {
      return (&messageField_) + 1;
    }

    inline const std::string &
    SingleFieldAccessor::getApplicationType()const
    {
      return nada_;
    }

    inline const std::string &
    SingleFieldAccessor::getApplicationTypeNs()const
    {
      return nada_;
    }

  }
}
#endif // SPECIALACCESSORS_H
