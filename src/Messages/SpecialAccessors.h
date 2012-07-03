// Copyright (c) 2009, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef SPECIALACCESSORS_H
#define SPECIALACCESSORS_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

#include <Messages/MessageAccessor.h>
#include <Messages/MessageField.h>
#include <Messages/Sequence.h>

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
      virtual bool isPresent(const FieldIdentity & identity)const;
      virtual bool getUnsignedInteger(const FieldIdentity & identity, ValueType::Type type, uint64 & value)const;
      virtual bool getSignedInteger(const FieldIdentity & identity, ValueType::Type type, int64 & value)const;
      virtual bool getDecimal(const FieldIdentity & identity, ValueType::Type type, Decimal & value)const;
      virtual bool getString(const FieldIdentity & identity, ValueType::Type type, const StringBuffer *& value)const;
      virtual bool getGroup(const FieldIdentity & identity, const MessageAccessor *& group)const;
      virtual bool getSequenceLength(const FieldIdentity & identity, size_t & length)const;
      virtual bool getSequenceEntry(const FieldIdentity & identity, size_t index, const MessageAccessor *& entry)const;

      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;

    private:
      std::string nada_;

    };

    inline bool
    EmptyAccessor::isPresent(const Messages::FieldIdentity & )const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getUnsignedInteger(const FieldIdentity & identity, ValueType::Type type, uint64 & value)const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getSignedInteger(const FieldIdentity & identity, ValueType::Type type, int64 & value)const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getDecimal(const FieldIdentity & identity, ValueType::Type type, Decimal & value)const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getString(const FieldIdentity & identity, ValueType::Type type, const StringBuffer *& value)const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getGroup(const FieldIdentity & identity, const MessageAccessor *& group)const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getSequenceLength(const FieldIdentity & identity, size_t & length)const
    {
      return false;
    }

    inline bool
    EmptyAccessor::getSequenceEntry(const FieldIdentity & identity, size_t index, const MessageAccessor *& entry)const
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
          const Messages::FieldIdentity & identity,
          const Messages::FieldCPtr & field);

      /////////////
      // Implement MessageAccessor
      virtual bool isPresent(const FieldIdentity & identity)const;
      virtual bool getUnsignedInteger(const FieldIdentity & identity, ValueType::Type type, uint64 & value)const;
      virtual bool getSignedInteger(const FieldIdentity & identity, ValueType::Type type, int64 & value)const;
      virtual bool getDecimal(const FieldIdentity & identity, ValueType::Type type, Decimal & value)const;
      virtual bool getString(const FieldIdentity & identity, ValueType::Type type, const StringBuffer *& value)const;
      virtual bool getGroup(const FieldIdentity & identity, const MessageAccessor *& group)const;
      virtual bool getSequenceLength(const FieldIdentity & identity, size_t & length)const;
      virtual bool getSequenceEntry(const FieldIdentity & identity, size_t index, const MessageAccessor *& entry)const;
      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;

    private:
      Messages::MessageField messageField_;
      std::string nada_;
    };

    inline
    SingleFieldAccessor::SingleFieldAccessor(
        const Messages::FieldIdentity & identity,
        const Messages::FieldCPtr & field)
      : messageField_(identity, field)
    {
    }

    inline bool
    SingleFieldAccessor::isPresent(const FieldIdentity & identity)const
    {
      return identity == messageField_.getIdentity();
    }
    inline bool
    SingleFieldAccessor::getUnsignedInteger(const FieldIdentity & identity, ValueType::Type type, uint64 & value)const
    {
      if(messageField_.getField()->isUnsignedInteger())
      {
        value = messageField_.getField()->toUnsignedInteger();
        return true;
      }
      return false;
    }

    inline bool
    SingleFieldAccessor::getSignedInteger(const FieldIdentity & identity, ValueType::Type type, int64 & value)const
    {
      if(messageField_.getField()->isSignedInteger())
      {
        value = messageField_.getField()->toSignedInteger();
        return true;
      }
      return false;
    }

    inline bool
    SingleFieldAccessor::getDecimal(const FieldIdentity & identity, ValueType::Type type, Decimal & value)const
    {
      if(messageField_.getField()->getType() == ValueType::DECIMAL)
      {
        value = messageField_.getField()->toDecimal();
        return true;
      }
      return false;
    }

    inline bool
    SingleFieldAccessor::getString(const FieldIdentity & identity, ValueType::Type type, const StringBuffer *& value)const
    {
      if(messageField_.getField()->isString())
      {
        value = & messageField_.getField()->toString();
        return true;
      }
      return false;
    }

    inline bool
    SingleFieldAccessor::getGroup(const FieldIdentity & identity, const MessageAccessor *& group)const
    {
      if(messageField_.getField()->getType() == ValueType::GROUP)
      {
        const GroupCPtr & groupPtr = messageField_.getField()->toGroup();
        group = groupPtr.get();
        return true;
      }
      return false;
    }

    inline bool
    SingleFieldAccessor::getSequenceLength(const FieldIdentity & identity, size_t & length)const
    {
      if(messageField_.getField()->getType() == ValueType::SEQUENCE)
      {
        const SequenceCPtr & sequence = messageField_.getField()->toSequence();
        length = sequence->size();
        return true;
      }
      return false;
    }

    inline bool
    SingleFieldAccessor::getSequenceEntry(const FieldIdentity & identity, size_t index, const MessageAccessor *& entry)const
    {
      if(messageField_.getField()->getType() == ValueType::SEQUENCE)
      {
        const SequenceCPtr & sequence = messageField_.getField()->toSequence();
        entry = (*sequence)[index].get();
        return true;
      }
      return false;
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
