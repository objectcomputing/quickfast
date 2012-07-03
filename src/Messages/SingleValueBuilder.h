// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef SINGLEVALUEBUILDER_H
#define SINGLEVALUEBUILDER_H
#include <Messages/ValueMessageBuilder.h>
#include <Common/Value.h>

namespace QuickFAST
{
  namespace Messages
  {
    ///@brief a MessageBuilder that creates a "record" containing a single value
    ///
    /// Or more accurately, captures a single value produced by the decoder.
    template<typename DATATYPE>
    class SingleValueBuilder : public ValueMessageBuilder
    {
      public:
        SingleValueBuilder();
        virtual ~SingleValueBuilder();

        ///////////////////////////
        // Implement ValueMessageBuilder
        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const int64 value);
        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const uint64 value);
        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const int32 value);
        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const uint32 value);
        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const int16 value);
        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const uint16 value);
        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const int8 value);
        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const uchar value);
        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const Decimal& value);
        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const unsigned char * value, size_t length);
        virtual ValueMessageBuilder & startMessage(
          const std::string & applicationType,
          const std::string & applicationTypeNamespace,
          size_t size);
        virtual bool endMessage(ValueMessageBuilder & messageBuilder);
        virtual bool ignoreMessage(ValueMessageBuilder & messageBuilder);

        /// @brief check to see if a value was set
        ///
        /// @returns true if a value was set
        bool isSet()const;

        /// @brief retrieve the captured value
        ///
        /// probably a good idea to check isSet() first
        /// @returns the value
        DATATYPE value()const;

        /// @brief access the identity that was used to set the value
        const FieldIdentity & identity()const;

        /// @brief clear flag used by isSet()
        void reset();

      private:
        /////////////////////////////////////////////////
        // Dummy implementations of other virtual methods
        virtual void clear(size_t /*capacity*/){}
        virtual void reserve(size_t /*capacity*/){}
        virtual size_t size() const
        {
          return 1;
        }

        virtual void setApplicationType(const std::string & /*type*/, const std::string & /*ns*/)
        {
        }

        virtual const std::string & getApplicationType() const
        {
          static const std::string name("singlevalue");
          return name;
        }

        virtual const std::string & getApplicationTypeNs() const
        {
          static const std::string result("");
          return result;
        }

        virtual ValueMessageBuilder & startSequence(
          const FieldIdentity & identity,
          const std::string & applicationType,
          const std::string & applicationTypeNamespace,
          size_t fieldCount,
          const FieldIdentity &lengthIdentity,
          size_t length)
        {
          throw UsageError("Coding Error", "SingleValueBuilder does not support start sequence.");
        }

        virtual ValueMessageBuilder & startSequenceEntry(
          const std::string & /*applicationType*/,
          const std::string & /*applicationTypeNamespace*/,
          size_t /*size*/)
        {
          throw UsageError("Coding Error", "SingleValueBuilder does not support start sequence entry.");
        }

        virtual void endSequenceEntry(ValueMessageBuilder & /*entry*/)
        {
          throw UsageError("Coding Error", "SingleValueBuilder does not support end sequence entry.");
        }
        virtual void endSequence(const FieldIdentity &/*identity*/, ValueMessageBuilder & )
        {
          throw UsageError("Coding Error", "SingleValueBuilder does not support end sequence.");
        }

        virtual ValueMessageBuilder & startGroup(
          const FieldIdentity &/*identity*/,
          const std::string & /*applicationType*/,
          const std::string & /*applicationTypeNamespace*/,
          size_t /*size*/)
        {
          throw UsageError("Coding Error", "SingleValueBuilder does not support start group.");
        }

        virtual void endGroup(
          const FieldIdentity &/*identity*/,
          ValueMessageBuilder & /*entry*/)
        {
          throw UsageError("Coding Error", "SingleValueBuilder does not support end group.");
        }

        virtual bool wantLog(unsigned short /*level*/)
        {
          return false;
        }

        virtual bool logMessage(unsigned short /*level*/, const std::string & /*logMessage*/)
        {
          return true;
        }

        virtual bool reportDecodingError(const std::string & /*errorMessage*/)
        {
          return true;
        }

        virtual bool reportCommunicationError(const std::string & /*errorMessage*/)
        {
          return true;
        }

      private:
        Value value_;
        const FieldIdentity * identity_;
    };

    template<typename DATATYPE>
    inline
    SingleValueBuilder<DATATYPE>::SingleValueBuilder()
    : identity_(0)
    {
    }

    template<typename DATATYPE>
    inline
    SingleValueBuilder<DATATYPE>::~SingleValueBuilder()
    {
    }

    template<typename DATATYPE>
    inline void
    SingleValueBuilder<DATATYPE>::addValue(const FieldIdentity & identity, ValueType::Type type, const int64 value)
    {
      identity_ = &identity;
      value_.setValue(value);
    }

    template<typename DATATYPE>
    inline void
    SingleValueBuilder<DATATYPE>::addValue(const FieldIdentity & identity, ValueType::Type type, const uint64 value)
    {
      identity_ = &identity;
      value_.setValue(value);
    }

    template<typename DATATYPE>
    inline void
    SingleValueBuilder<DATATYPE>::addValue(const FieldIdentity & identity, ValueType::Type type, const int32 value)
    {
      identity_ = &identity;
      value_.setValue(value);
    }

    template<typename DATATYPE>
    inline void
    SingleValueBuilder<DATATYPE>::addValue(const FieldIdentity & identity, ValueType::Type type, const uint32 value)
    {
      identity_ = &identity;
      value_.setValue(value);
    }

    template<typename DATATYPE>
    inline void
    SingleValueBuilder<DATATYPE>::addValue(const FieldIdentity & identity, ValueType::Type type, const int16 value)
    {
      identity_ = &identity;
      value_.setValue(value);
    }

    template<typename DATATYPE>
    inline void
    SingleValueBuilder<DATATYPE>::addValue(const FieldIdentity & identity, ValueType::Type type, const uint16 value)
    {
      identity_ = &identity;
      value_.setValue(value);
    }

    template<typename DATATYPE>
    inline void
    SingleValueBuilder<DATATYPE>::addValue(const FieldIdentity & identity, ValueType::Type type, const int8 value)
    {
      identity_ = &identity;
      value_.setValue(value);
    }

    template<typename DATATYPE>
    inline void
    SingleValueBuilder<DATATYPE>::addValue(const FieldIdentity & identity, ValueType::Type type, const uchar value)
    {
      identity_ = &identity;
      value_.setValue(value);
    }

    template<typename DATATYPE>
    inline void
    SingleValueBuilder<DATATYPE>::addValue(const FieldIdentity & identity, ValueType::Type type, const Decimal& value)
    {
      identity_ = &identity;
      value_.setValue(value);
    }

    template<typename DATATYPE>
    inline void
    SingleValueBuilder<DATATYPE>::addValue(const FieldIdentity & identity, ValueType::Type type, const unsigned char * value, size_t length)
    {
      identity_ = &identity;
      value_.setValue(value, length);
    }

    template<typename DATATYPE>
    inline ValueMessageBuilder &
    SingleValueBuilder<DATATYPE>::startMessage(
      const std::string & /*applicationType*/,
      const std::string & /*applicationTypeNamespace*/,
      size_t /*size*/)
    {
      return *this;
    }

    template<typename DATATYPE>
    inline bool
    SingleValueBuilder<DATATYPE>::endMessage(ValueMessageBuilder & messageBuilder)
    {
      return true;
    }

    template<typename DATATYPE>
    inline bool
    SingleValueBuilder<DATATYPE>::ignoreMessage(ValueMessageBuilder & messageBuilder)
    {
      value_.setNull();
      return true;
    }

    template<typename DATATYPE>
    inline bool
    SingleValueBuilder<DATATYPE>::isSet()const
    {
      return value_.isDefined();
    }
    template<typename DATATYPE>
    inline void
    SingleValueBuilder<DATATYPE>::reset()
    {
      value_.setUndefined();
    }


    template<typename DATATYPE>
    inline DATATYPE
    SingleValueBuilder<DATATYPE>::value()const
    {
      DATATYPE result = DATATYPE();
      value_.getValue(result);
      return result;
    }

    template<typename DATATYPE>
    inline const FieldIdentity &
    SingleValueBuilder<DATATYPE>::identity()const
    {
      return *identity_;
    }
  }
}

#endif // SINGLEVALUEBUILDER_H

