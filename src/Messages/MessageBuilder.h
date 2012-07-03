// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H
#include "MessageBuilder_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Messages/ValueMessageBuilder.h>
#include <Messages/MessageField.h>
//#include <Common/Logger.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief Interface to support building a message during decoding.
    class QuickFAST_Export MessageBuilder : public ValueMessageBuilder
    {
    public:

      MessageBuilder();

      virtual ~MessageBuilder(){};

      /// @brief Write verbose information to ostream
      /// @param vout is the stream to which to write.  0 disables verbosity.
      void setVerbose(std::ostream * vout)
      {
        vout_ = vout;
      }

      /// @brief Add a field to the set.
      ///
      /// The FieldCPtr is copied, not the actual Field object.
      /// @param identity identifies this field
      /// @param value is the value to be assigned.
      virtual void addField(const FieldIdentity & identity, const FieldCPtr & value) = 0;

      //////////////////////////////////////////
      // Partially Implement ValueMessageBuilder
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

    private:
      std::ostream * vout_;
    };
  }
}
#endif // MESSAGEBUILDER_H
