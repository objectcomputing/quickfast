// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#pragma unmanaged
#include <DotNet/ImplBuilderBase.h>
#include <Messages/FieldIdentity_fwd.h>

namespace QuickFAST
{
  namespace DotNet
  {
    class ImplFieldSet;
    class ImplSequenceBuilder;

    /// @brief implementation class for field set builder
    class ImplFieldSetBuilder
      : public ImplBuilderBase
    {
    public:
      /// @brief construct given a deliverer to receive the results
      ImplFieldSetBuilder(ImplMessageDeliverer & deliverer);
      ~ImplFieldSetBuilder();
      /// @brief End this set, return the implementation (losing ownership)
      ImplFieldSet * endSet();

      /// @brief Start a new set
      void reset(size_t size);

      //////////////////////////
      // Implement ValueMessageBuilder
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int64 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint64 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int32 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint32 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int16 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint16 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int8 value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uchar value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const Decimal& value);
      virtual void addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const unsigned char * value, size_t length);
      virtual Messages::ValueMessageBuilder & startMessage(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual bool endMessage(Messages::ValueMessageBuilder & messageBuilder);
      virtual bool ignoreMessage(Messages::ValueMessageBuilder & messageBuilder);
      virtual Messages::ValueMessageBuilder & startSequence(
        const Messages::FieldIdentity & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t fieldCount,
        const Messages::FieldIdentity & lengthIdentity,
        size_t length);
      virtual void endSequence(
        const Messages::FieldIdentity & identity,
        Messages::ValueMessageBuilder & sequenceBuilder);
      virtual Messages::ValueMessageBuilder & startGroup(
        const Messages::FieldIdentity & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual void endGroup(
        const Messages::FieldIdentity & identity,
        Messages::ValueMessageBuilder & groupBuilder);
    private:
      ImplFieldSet * fieldSet_;
      boost::scoped_ptr<ImplSequenceBuilder> sequenceBuilder_;
      boost::scoped_ptr<ImplFieldSetBuilder> groupBuilder_;
    };
  }
}
#pragma managed
