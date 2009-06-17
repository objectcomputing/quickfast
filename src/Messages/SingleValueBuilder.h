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

        virtual QuickFAST::Messages::MessageBuilder * createdNestedFields(size_t size)const
        {
          return 0;
        }
      private:
        bool set_;
        DATATYPE value_;
    };
  }
}

#endif // SINGLEVALUEBUILDER_H

