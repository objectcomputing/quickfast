// Copyright (c) 2009, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef SEQUENTIALSINGLEVALUEBUILDER_H
#define SEQUENTIALSINGLEVALUEBUILDER_H
#include <Messages/ValueMessageBuilder.h>
#include <Messages/FieldIdentity.h>

#include <Common/Value.h>

namespace QuickFAST
{
  namespace Messages
  {
    ///@brief a MessageBuilder that creates a "record" containing a single value
    ///
    /// Or more accurately, captures a single value produced by the decoder.
    template<typename DATATYPE>
    class SequentialSingleValueBuilder : public ValueMessageBuilder
    {
      public:
        SequentialSingleValueBuilder()
        : error_(false)
        , logged_(false)
        , gapDetected_(false)
        , messageCount_(0)
        {
        }

        virtual ~SequentialSingleValueBuilder()
        {
        }

        ///////////////////////////
        // Implement ValueMessageBuilder
        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const int64 value)
        {
          identities_.push_back(identity);
          Value v;
          v.setValue(value);
          values_.push_back(v);
        }

        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const uint64 value)
        {
          identities_.push_back(identity);
          Value v;
          v.setValue(value);
          values_.push_back(v);
        }

        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const int32 value)
        {
          identities_.push_back(identity);
          Value v;
          v.setValue(value);
          values_.push_back(v);
        }

        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const uint32 value)
        {
          identities_.push_back(identity);
          Value v;
          v.setValue(value);
          values_.push_back(v);
        }

        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const int16 value)
        {
          identities_.push_back(identity);
          Value v;
          v.setValue(value);
          values_.push_back(v);
        }

        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const uint16 value)
        {
          identities_.push_back(identity);
          Value v;
          v.setValue(value);
          values_.push_back(v);
        }

        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const int8 value)
        {
          identities_.push_back(identity);
          Value v;
          v.setValue(value);
          values_.push_back(v);
        }

        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const uchar value)
        {
          identities_.push_back(identity);
          Value v;
          v.setValue(value);
          values_.push_back(v);
        }

        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const Decimal& value)
        {
          identities_.push_back(identity);
          Value v;
          v.setValue(value);
          values_.push_back(v);
        }

        virtual void addValue(const FieldIdentity & identity, ValueType::Type type, const unsigned char * value, size_t length)
        {
          identities_.push_back(identity);
          Value v;
          v.setValue(value, length);
          values_.push_back(v);
        }

        virtual ValueMessageBuilder & startMessage(
          const std::string & /*applicationType*/,
          const std::string & /*applicationTypeNamespace*/,
          size_t /*size*/)
        {
          messageCount_ += 1;
          return *this;
        }

        virtual bool endMessage(ValueMessageBuilder & messageBuilder)
        {
          return true;
        }

        virtual bool ignoreMessage(ValueMessageBuilder & messageBuilder)
        {
          return true;
        }

        virtual void reportGap(sequence_t startGap, sequence_t endGap)
        {
          gapDetected_ = true;
        }


        /// @brief How many values have been collected
        ///
        /// @returns the number of values collected
        size_t valueCount()const
        {
          return values_.size();
        }

        /// @brief How many messages have been decoded
        ///
        /// @returns the number of messages
        size_t valueMessages()const
        {
          return messageCount_;
        }

        /// @brief retrieve a captured value
        ///
        /// probably a good idea to check valueCount() first
        /// @param index selects the desired value
        /// @returns the value
        DATATYPE value(size_t index)const
        {
          DATATYPE result = DATATYPE();
          values_[index].getValue(result);
          return result;
        }

        /// @brief access the identity that was used to set the indexth value
        const FieldIdentity & identity(size_t index)const
        {
          return identities_[index];
        }

        /// @ brief was an error detected?
        bool hasError()const
        {
          return error_;
        }

        /// @ brief was a log message written?
        bool hasLog()const
        {
          return logged_;
        }

        /// @brief was a gap detected?
        bool hasGap()const
        {
          return gapDetected_;
        }

        /// @brief Get the message from the last log or error.
        const std::string & message()
        {
          return message_;
        }

        /// @brief reset everything and prepare to accept new data.
        void reset()
        {
          error_ = false;
          logged_ = false;
          gapDetected_ = false;
          message_.clear();
          messageCount_ = 0;
          values_.clear();
          identities_.clear();
        }
      private:
        /////////////////////////////////////////////////
        // Dummy implementations of other virtual methods
        virtual void clear(size_t /*capacity*/){}
        virtual void reserve(size_t /*capacity*/){}
        virtual size_t size() const
        {
          return 1;
        }

        //virtual bool getIdentity(const std::string &/*name*/, const FieldIdentity & identity) const
        //{
        //  if(identities_.empty())
        //  {
        //    return false;
        //  }
        //  identity = identities_[0];
        //  return true;
        //}

        virtual void setApplicationType(const std::string & /*type*/, const std::string & /*ns*/)
        {
        }

        virtual const std::string & getApplicationType() const
        {
          static const std::string name("sequentialsinglevalue");
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
          throw UsageError("Coding Error", "SequentialSingleValueBuilder does not support start sequence.");
        }

        virtual ValueMessageBuilder & startSequenceEntry(
          const std::string & /*applicationType*/,
          const std::string & /*applicationTypeNamespace*/,
          size_t /*size*/)
        {
          throw UsageError("Coding Error", "SequentialSingleValueBuilder does not support start sequence entry.");
        }

        virtual void endSequenceEntry(ValueMessageBuilder & /*entry*/)
        {
          throw UsageError("Coding Error", "SequentialSingleValueBuilder does not support end sequence entry.");
        }
        virtual void endSequence(const FieldIdentity &/*identity*/, ValueMessageBuilder & )
        {
          throw UsageError("Coding Error", "SequentialSingleValueBuilder does not support end sequence.");
        }

        virtual ValueMessageBuilder & startGroup(
          const FieldIdentity &/*identity*/,
          const std::string & /*applicationType*/,
          const std::string & /*applicationTypeNamespace*/,
          size_t /*size*/)
        {
          throw UsageError("Coding Error", "SequentialSingleValueBuilder does not support start group.");
        }

        virtual void endGroup(
          const FieldIdentity &/*identity*/,
          ValueMessageBuilder & /*entry*/)
        {
          throw UsageError("Coding Error", "SequentialSingleValueBuilder does not support end group.");
        }

        virtual bool wantLog(unsigned short /*level*/)
        {
          return true;
        }

        virtual bool logMessage(unsigned short /*level*/, const std::string & logMessage)
        {
          message_ = logMessage;
//          std::cout << "LOG: " << message_ << std::endl;
          logged_ = true;
          return true;
        }

        virtual bool reportDecodingError(const std::string & errorMessage)
        {
          message_ = errorMessage;
//          std::cout << "ERR: " << message_ << std::endl;
          error_ = true;
          return true;
        }

        virtual bool reportCommunicationError(const std::string & errorMessage)
        {
          message_ = errorMessage;
//          std::cout << "COM: " << message_ << std::endl;
          error_ = true;
          return true;
        }

      private:
        bool error_;
        bool logged_;
        bool gapDetected_;
        std::string message_;
        size_t messageCount_;

        typedef std::vector<Value> ValueVector;
        ValueVector values_;

        typedef std::vector<FieldIdentity> IdentityVector;
        mutable IdentityVector identities_;
    };
  }
}

#endif // SEQUENTIALSINGLEVALUEBUILDER_H

