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
      virtual bool pickTemplate(template_id_t & tid);
      virtual bool isPresent(const FieldIdentity & identity)const;
      virtual bool getField(const FieldIdentity & identity, FieldCPtr & value) const;
      virtual bool getUnsignedInteger(const FieldIdentity & identity, ValueType::Type type,  uint64 & value)const;
      virtual bool getSignedInteger(const FieldIdentity & identity, ValueType::Type type, int64 & value)const;
      virtual bool getDecimal(const FieldIdentity & identity,ValueType::Type type, Decimal & value)const;
      virtual bool getString(const FieldIdentity & identity,ValueType::Type type, const StringBuffer *& value)const;
      virtual bool getIdentity(const std::string &name, FieldIdentityCPtr & identity) const;
      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;

    private:
      std::string nada_;

    };

    inline bool
    EmptyAccessor::pickTemplate(template_id_t & tid)
    {
      return false;
    }

    inline bool
    EmptyAccessor::isPresent(const FieldIdentity & /*identity*/)const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getField(const FieldIdentity & /*identity*/, Messages::FieldCPtr & /*value*/) const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getIdentity(const std::string &/*name*/, Messages::FieldIdentityCPtr & /*identity*/) const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getUnsignedInteger(const FieldIdentity & identity, ValueType::Type type,  uint64 & value)const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getSignedInteger(const FieldIdentity & identity, ValueType::Type type, int64 & value)const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getDecimal(const FieldIdentity & identity,ValueType::Type type, Decimal & value)const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getString(const FieldIdentity & identity,ValueType::Type type, const StringBuffer *& value)const
    {
      return false;
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
          const Messages::FieldCPtr & field,
          template_id_t tid = 0);

      /////////////
      // Implement MessageAccessor
      virtual bool pickTemplate(template_id_t & tid);
      virtual bool isPresent(const FieldIdentity & identity)const;
      virtual bool getField(const FieldIdentity & identity, FieldCPtr & value) const;
      virtual bool getUnsignedInteger(const FieldIdentity & identity, ValueType::Type type,  uint64 & value)const;
      virtual bool getSignedInteger(const FieldIdentity & identity, ValueType::Type type, int64 & value)const;
      virtual bool getDecimal(const FieldIdentity & identity,ValueType::Type type, Decimal & value)const;
      virtual bool getString(const FieldIdentity & identity,ValueType::Type type, const StringBuffer *& value)const;
      virtual bool getIdentity(const std::string &name, FieldIdentityCPtr & identity) const;
      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;

    private:
      Messages::MessageField messageField_;
      std::string nada_;
      template_id_t tid_;
    };

    inline
    SingleFieldAccessor::SingleFieldAccessor(
        Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & field,
        template_id_t tid)
      : messageField_(identity, field)
      , tid_(tid)
    {
    }

    inline
    bool
    SingleFieldAccessor::pickTemplate(template_id_t & tid)
    {
      tid = tid_;
      return true;
    }

    inline bool
    SingleFieldAccessor::isPresent(const FieldIdentity & /*identity*/)const
    {
      // we could check
      return true;
    }

    inline bool
    SingleFieldAccessor::getField(const FieldIdentity & /*identity*/, Messages::FieldCPtr & value) const
    {
      value = messageField_.getField();
      return true;
    }

    inline bool
    SingleFieldAccessor::getUnsignedInteger(const FieldIdentity & identity, ValueType::Type type,  uint64 & value)const
    {
      bool result = false;
      if(messageField_.getField()->isUnsignedInteger())
      {
        value = messageField_.getField()->toUnsignedInteger();
        result = true;
      }
      return result;
    }

    inline bool
    SingleFieldAccessor::getSignedInteger(const FieldIdentity & identity, ValueType::Type type, int64 & value)const
    {
      bool result = false;
      if(messageField_.getField()->isSignedInteger())
      {
        value = messageField_.getField()->toSignedInteger();
        result = true;
      }
      return result;
    }

    inline bool
    SingleFieldAccessor::getDecimal(const FieldIdentity & identity,ValueType::Type type, Decimal & value)const
    {
      bool result = false;
      if(messageField_.getField()->isType(ValueType::DECIMAL))
      {
        value = messageField_.getField()->toDecimal();
        result = true;
      }
      return result;
    }

    inline bool
    SingleFieldAccessor::getString(const FieldIdentity & identity,ValueType::Type type, const StringBuffer *& value)const
    {
      bool result = false;
      if(messageField_.getField()->isString())
      {
        value = &messageField_.getField()->toString();
        result = true;
      }
      return result;
    }

    inline bool
    SingleFieldAccessor::getIdentity(const std::string &/*name*/, Messages::FieldIdentityCPtr & identity) const
    {
      identity = messageField_.getIdentity();
      return true;
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
