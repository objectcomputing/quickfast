// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#pragma unmanaged
#include <Messages/ValueMessageBuilder.h>

namespace QuickFAST
{
  namespace DotNet
  {
    class ImplMessageDeliverer;

    /// @brief base class for DotNet Builder implementations
    ///
    /// Handles Appplication type data; forwarding requests to Deliverer, and provides
    /// default, throwing implementations of other virtual methods.
    class ImplBuilderBase
      : public Messages::ValueMessageBuilder
    {
    public:
      /// @brief Construct given a deliverer to receive the results
      ImplBuilderBase(ImplMessageDeliverer & deliverer);
      virtual ~ImplBuilderBase();

      /// @brief set the name and namespace for this buildr
      void setApplicationType(const std::string & applicationType, const std::string & applicationTypeNamespace = "");

      ///@brief set the least interesting log message level
      /// setting this here avoids unnecessary conversions to System.String
      void setLogLimit(unsigned short limit);

      //////////////////////////
      // Implement ValueMessageBuilder
      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;
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
      virtual Messages::ValueMessageBuilder & startSequenceEntry(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) ;
      virtual void endSequenceEntry(Messages::ValueMessageBuilder & entry);
      virtual Messages::ValueMessageBuilder & startGroup(
        const Messages::FieldIdentity & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) ;
      virtual void endGroup(
        const Messages::FieldIdentity & identity,
        Messages::ValueMessageBuilder & groupBuilder);

      //////////////////////////
      // Implement Logger
      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);

    protected:
      /// @brief the name= attribute from the template.
      std::string applicationType_;
      /// @brief the ns= attribute from the template
      std::string applicationTypeNs_;

      /// @brief what's the minimum interesting log message
      unsigned short logLimit_;
      /// @brief Where to deliver notifications
      ImplMessageDeliverer & deliverer_;
    };
  }
}
#pragma managed
