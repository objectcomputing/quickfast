// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef SINGLEVALUEBUILDER_H
#define SINGLEVALUEBUILDER_H
#include <Messages/MessageBuilder.h>

namespace QuickFAST
{
  namespace Messages
  {
    template<typename DATATYPE>
    class SingleValueBuilder : public Messages::MessageBuilder
    {
      public:
        SingleValueBuilder()
          : set_(false)
          , value_(0)
        {
        }

        virtual ~SingleValueBuilder()
        {
        }

        MessageBuilder & startMessage(
          const std::string & applicationType,
          const std::string & applicationTypeNamespace,
          size_t size)
        {
          return *this;
        }

        bool endMessage(MessageBuilder &)
        {
          return true;
        }

        virtual void addField(
          const Messages::FieldIdentityCPtr & identity,
          const Messages::FieldCPtr & value)
        {
          value->getValue(value_);
          set_ = true;
        }

        bool isSet()const
        {
          return set_;
        }
        DATATYPE value()const
        {
          return value_;
        }


      private:
        /////////////////////////////////////////////////
        // Dummy implementations of other virtual methods
        virtual void clear(size_t capacity){}
        virtual void reserve(size_t capacity){}
        virtual size_t size() const
        {
          return 1;
        }

        virtual bool getIdentity(const std::string &name, Messages::FieldIdentityCPtr & identity) const
        {
          return false;
        }

        virtual void setApplicationType(const std::string & type, const std::string & ns)
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

        virtual MessageBuilder & startSequence(
          Messages::FieldIdentityCPtr identity,
          const std::string & applicationType,
          const std::string & applicationTypeNamespace,
          size_t size)
        {
          throw QuickFAST::UsageError("Single Value", "Illegal Sequence.");
        }

        virtual QuickFAST::Messages::MessageBuilder & startSequenceEntry(
          const std::string & applicationType,
          const std::string & applicationTypeNamespace,size_t size)
        {
          throw QuickFAST::UsageError("Single Value", "Illegal Sequence.");
        }

        virtual void endSequenceEntry(MessageBuilder & entry)
        {
          throw QuickFAST::UsageError("Single Value", "Illegal Sequence.");
        }
        virtual void endSequence( Messages::FieldIdentityCPtr identity, MessageBuilder & )
        {
          throw QuickFAST::UsageError("Single Value", "Illegal Sequence.");
        }

        virtual QuickFAST::Messages::MessageBuilder & startGroup(
          Messages::FieldIdentityCPtr identity,
          const std::string & applicationType,
          const std::string & applicationTypeNamespace,size_t size)
        {
          throw QuickFAST::UsageError("Single Value", "Illegal Group.");
        }
        virtual void endGroup(
          Messages::FieldIdentityCPtr identity,
          MessageBuilder & entry)
        {
          throw QuickFAST::UsageError("Single Value", "Illegal Group.");
        }

        virtual const FieldSet & getFieldSet() const
        {
          throw QuickFAST::UsageError("Single Value", "Illegal Sequence or Group.");
        }

        virtual bool wantLog(unsigned short level)
        {
          return false;
        }

        virtual bool logMessage(unsigned short level, const std::string & logMessage)
        {
          return true;
        }

        virtual bool reportDecodingError(const std::string & errorMessage)
        {
          return true;
        }

        virtual bool reportCommunicationError(const std::string & errorMessage)
        {
          return true;
        }

      private:
        bool set_;
        DATATYPE value_;
    };
  }
}

#endif // SINGLEVALUEBUILDER_H

