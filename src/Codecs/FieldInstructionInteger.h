// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTIONINTEGER_H
#define FIELDINSTRUCTIONINTEGER_H
#include <Codecs/FieldInstruction.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>
#include <Codecs/DataSource.h>
#include <Codecs/DataDestination.h>
#include <Messages/ValueMessageBuilder.h>
#include <Messages/MessageAccessor.h>
#include <Messages/Field.h>

#include <Common/Profiler.h>

using namespace QuickFAST;
using namespace QuickFAST::Codecs;

namespace QuickFAST{
  namespace Codecs{

    /// @brief A basic implementation for all integral types.
    ///
    /// Used for &lt;int32> &lt;uint32> &lt;int64> &lt;uint64> fields.
    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    class FieldInstructionInteger : public FieldInstruction
    {
    public:
      /// @brief construct with a name and a namespace
      /// @param name is the local name
      /// @param fieldNamespace is the namespace to qualify this name
      FieldInstructionInteger(
        const std::string & name,
        const std::string & fieldNamespace);

      /// @brief construct anonomous field instruction
      FieldInstructionInteger();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionInteger();

      virtual void interpretValue(const std::string & value);

      /// @brief direct access for debugging
      void setInitialValue(INTEGER_TYPE initialValue)
      {
        typedValue_ = initialValue;
        typedValueIsDefined_ = true;
      }

      virtual void setDefaultValueIncrement()
      {
        typedValue_ = INTEGER_TYPE(1);
        typedValueIsDefined_ = true;
      }

      virtual bool decodeNop(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & fieldSet) const;

      virtual bool decodeConstant(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & fieldSet) const;

      virtual bool decodeDefault(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & fieldSet) const;

      virtual bool decodeCopy(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & fieldSet) const;

      virtual bool decodeCopy(
        Codecs::DataSource & source,
        bool pmapValue,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & fieldSet) const;

      virtual bool decodeDelta(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & fieldSet) const;

      virtual bool decodeIncrement(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & fieldSet) const;

      virtual bool decodeIncrement(
        Codecs::DataSource & source,
        bool pmapValue,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & fieldSet) const;

      virtual void encodeNop(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const;

      virtual void encodeConstant(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const;

      virtual void encodeDefault(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const;

      virtual void encodeCopy(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const;

      virtual void encodeDelta(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const;

      virtual void encodeIncrement(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const;

    private:
      FieldInstructionInteger(const FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED> &);
      FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED> & operator=(const FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED> &);

    private:
      INTEGER_TYPE typedValue_;
      bool typedValueIsDefined_;
    };

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    FieldInstructionInteger(
          const std::string & name,
          const std::string & fieldNamespace)
      : FieldInstruction(name, fieldNamespace)
      , typedValue_(INTEGER_TYPE(0))
      , typedValueIsDefined_(false)
    {
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    FieldInstructionInteger()
      : typedValue_(INTEGER_TYPE(0))
      , typedValueIsDefined_(false)
    {
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    ~FieldInstructionInteger()
    {
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    void
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    interpretValue(const std::string & value)
    {
      if(value.empty())
      {
        typedValue_ = INTEGER_TYPE(0);
        typedValueIsDefined_ = false;
      }
      else
      {
        if(SIGNED)
        {
          int64 v = boost::lexical_cast<int64>(value);
          typedValue_ = INTEGER_TYPE(v);
          if(v != typedValue_)
          {
            throw QuickFAST::TemplateDefinitionError("[ERR D1] Initial value oveflow");
          }
          typedValueIsDefined_ = true;
        }
        else
        {
          uint64 v = boost::lexical_cast<uint64>(value);
          typedValue_ = INTEGER_TYPE(v);
          if(v != typedValue_)
          {
            throw QuickFAST::TemplateDefinitionError("[ERR D1] Initial value oveflow");
          }
          typedValueIsDefined_ = true;
        }
      }
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    bool
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    decodeNop(
      Codecs::DataSource & source,
      Codecs::PresenceMap & /*pmap*/,
      Codecs::Decoder & decoder,
      Messages::ValueMessageBuilder & fieldSet) const
    {
      PROFILE_POINT("int::decodeNop");

      // note NOP never uses pmap.  It uses a null value instead for optional fields
      // so it's always safe to do the basic decode.
      INTEGER_TYPE value = 0;
      if(SIGNED) // expect compile-time optimization here
      {
        decodeSignedInteger(source, decoder, value, identity_->name());
      }
      else
      {
        decodeUnsignedInteger(source, decoder, value, identity_->name());
      }
      if(isMandatory())
      {
        fieldSet.addValue(
          identity_,
          VALUE_TYPE,
          value);
      }
      else
      {
        // not mandatory means it's nullable
        if(!checkNullInteger(value))
        {
          fieldSet.addValue(
            identity_,
            VALUE_TYPE,
            value);
        }
      }
      return true;
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    bool
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    decodeConstant(
      Codecs::DataSource & /*source*/,
      Codecs::PresenceMap & pmap,
      Codecs::Decoder & /*decoder*/,
      Messages::ValueMessageBuilder & fieldSet) const
    {
      PROFILE_POINT("int::decodeConstant");
      if(!isMandatory() && !pmap.checkNextField())
      {
        // nothing to say
      }
      else
      {
        fieldSet.addValue(
          identity_,
          VALUE_TYPE,
          typedValue_);
      }
      return true;
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    bool
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    decodeCopy(
      Codecs::DataSource & source,
      Codecs::PresenceMap & pmap,
      Codecs::Decoder & decoder,
      Messages::ValueMessageBuilder & fieldSet) const
    {
      return decodeCopy(source, pmap.checkNextField(), decoder, fieldSet);
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    bool
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    decodeCopy(
        Codecs::DataSource & source,
        bool pmapValue,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & fieldSet) const
    {
      PROFILE_POINT("int::decodeCopy");
      if(pmapValue)
      {
        INTEGER_TYPE value = typedValue_;
        // present in stream
        if(SIGNED) // expect compile-time optimization here
        {
          decodeSignedInteger(source, decoder, value, identity_->name());
        }
        else
        {
          decodeUnsignedInteger(source, decoder, value, identity_->name());
        }

        if(isMandatory())
        {
          fieldSet.addValue(
            identity_,
            VALUE_TYPE,
            value);
          fieldOp_->setDictionaryValue(decoder, value);
        }
        else
        {
          // not mandatory means it's nullable
          if(checkNullInteger(value))
          {
            fieldOp_->setDictionaryValueNull(decoder);
          }
          else
          {
            fieldSet.addValue(
              identity_,
              VALUE_TYPE,
              value);
            fieldOp_->setDictionaryValue(decoder, value);
          }
        }

      }
      else // pmap says not present, use copy
      {
        INTEGER_TYPE previousValue = 0;
        if(fieldOp_->getDictionaryValue(decoder, previousValue))
        {
          fieldSet.addValue(
            identity_,
            VALUE_TYPE,
            previousValue);
        }
        else
        {
          // value not found in dictionary
          // not a problem..  use initial value if it's available
          if(fieldOp_->hasValue())
          {
            fieldSet.addValue(
              identity_,
              VALUE_TYPE,
              typedValue_);
            fieldOp_->setDictionaryValue(decoder, typedValue_);
          }
          else
          {
            if(isMandatory())
            {
              decoder.reportError(
                "[ERR D5]",
                "Copy operator missing mandatory integer field/no initial value",
                *identity_);
              fieldSet.addValue(
                identity_,
                VALUE_TYPE,
                INTEGER_TYPE(0));
              fieldOp_->setDictionaryValue(decoder, INTEGER_TYPE(0));
            }
          }
        }
      }
      return true;
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    bool
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    decodeDefault(
      Codecs::DataSource & source,
      Codecs::PresenceMap & pmap,
      Codecs::Decoder & decoder,
      Messages::ValueMessageBuilder & fieldSet) const
    {
      PROFILE_POINT("int::decodeDefault");
      if(pmap.checkNextField())
      {
        PROFILE_POINT("int::decodeDefault:present");
        INTEGER_TYPE value = 0;
        decodeSignedInteger(source, decoder, value, identity_->name());
        if(isMandatory())
        {
          fieldSet.addValue(
            identity_,
            VALUE_TYPE,
            value);
        }
        else
        {
          if(!checkNullInteger(value))
          {
            PROFILE_POINT("int::decodeDefault:,addexplicit");
            fieldSet.addValue(
              identity_,
              VALUE_TYPE,
              value);
          }
        }
      }
      else // field not in stream
      {
        PROFILE_POINT("int::decodeDefault:absent");
        if(fieldOp_->hasValue())
        {
          PROFILE_POINT("int::decodeDefault:adddefault");
          fieldSet.addValue(
            identity_,
            VALUE_TYPE,
            typedValue_);
        }
        else if(isMandatory())
        {
          decoder.reportError("[ERR D5]", "Mandatory default operator with no value.", *identity_);
        }
      }
      return true;
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    bool
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    decodeDelta(
      Codecs::DataSource & source,
      Codecs::PresenceMap & /*pmap*/,
      Codecs::Decoder & decoder,
      Messages::ValueMessageBuilder & fieldSet) const
    {
      PROFILE_POINT("int::decodeDelta");
      int64 delta;
      decodeSignedInteger(source, decoder, delta, identity_->name(), true);
      if(!isMandatory())
      {
        if(checkNullInteger(delta))
        {
          return true; // nothing in Message; no change to saved value
        }
      }
      INTEGER_TYPE value = typedValue_;
      Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(decoder, value);
      if(previousStatus == Context::UNDEFINED_VALUE)
      {
        if(fieldOp_->hasValue()) // initial value in field op?
        {
          value = typedValue_;
          fieldOp_->setDictionaryValue(decoder, value);
        }
      }
      // Apply delta
      value = INTEGER_TYPE(value + delta);
      // Save the results
      fieldSet.addValue(
        identity_,
        VALUE_TYPE,
        value);
      fieldOp_->setDictionaryValue(decoder, value);

      return true;
    }


    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    bool
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    decodeIncrement(
      Codecs::DataSource & source,
      Codecs::PresenceMap & pmap,
      Codecs::Decoder & decoder,
      Messages::ValueMessageBuilder & fieldSet) const
    {
      return decodeIncrement(source, pmap.checkNextField(), decoder, fieldSet);
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    bool
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    decodeIncrement(
        Codecs::DataSource & source,
        bool pmapValue,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & fieldSet) const
    {
      PROFILE_POINT("int::decodeIncrement");
      if(pmapValue)
      {
        //PROFILE_POINT("int::decodeIncrement::present");
        INTEGER_TYPE value = 0;
        if(SIGNED) // expect compile-time optimization here
        {
          decodeSignedInteger(source, decoder, value, identity_->name());
        }
        else
        {
          decodeUnsignedInteger(source, decoder, value, identity_->name());
        }
        if(isMandatory())
        {
          fieldSet.addValue(
            identity_,
            VALUE_TYPE,
            value);
          fieldOp_->setDictionaryValue(decoder, value);
        }
        else
        {
          //PROFILE_POINT("int::decodeIncrement::optional");
          // not mandatory means it's nullable
          if(!checkNullInteger(value))
          {
            fieldSet.addValue(
              identity_,
              VALUE_TYPE,
              value);
          fieldOp_->setDictionaryValue(decoder, value);
          }
        }
      }
      else
      {
        //PROFILE_POINT("int::decodeIncrement::absent");
        INTEGER_TYPE value = typedValue_;
        Context::DictionaryStatus previousStatus = fieldOp_->getDictionaryValue(decoder, value);
        if(previousStatus == Context::OK_VALUE)
        {
          value += 1;
        }
        else
        {
          if(fieldOp_->hasValue())
          {
            value = typedValue_;
          }
          else
          {
            if(isMandatory())
            {
              decoder.reportError("[ERR D5]", "Missing initial value for Increment operator", *identity_);
              value = 0;
            }
            else
            {
              // missing value for optional field.  We're done
              return true;
            }
          }
        }
        fieldSet.addValue(
          identity_,
          VALUE_TYPE,
          value);
        fieldOp_->setDictionaryValue(decoder, value);
      }
      return true;
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    void
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    encodeNop(
      Codecs::DataDestination & destination,
      Codecs::PresenceMap & /*pmap*/,
      Codecs::Encoder & encoder,
      const Messages::MessageAccessor & fieldSet) const
    {
      // get the value from the application data
      Messages::FieldCPtr field;
      if(fieldSet.getField(identity_->name(), field))
      {
        INTEGER_TYPE value;
        field->getValue(value);
        if(!isMandatory())
        {
          // gcc produces bogus warning on the following line see GCC Bugzilla Bug 11856
          // if(!SIGNED || value >= 0)
          if(!SIGNED || value > 0 || value == 0)
          {
            ++value;
          }
        }
        if(SIGNED)
        {
          encodeSignedInteger(destination, encoder.getWorkingBuffer(), value);
        }
        else
        {
          encodeUnsignedInteger(destination, encoder.getWorkingBuffer(), value);
        }
      }
      else // not defined in fieldset
      {
        if(isMandatory())
        {
          encoder.reportError("[ERR U01]", "Missing mandatory field.", *identity_);
          if(SIGNED)
          {
            encodeSignedInteger(destination, encoder.getWorkingBuffer(), 0);
          }
          else
          {
            encodeUnsignedInteger(destination, encoder.getWorkingBuffer(), 0);
          }
        }
        else
        {
          destination.putByte(nullInteger);
        }
      }
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    void
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    encodeConstant(
      Codecs::DataDestination & /*destination*/,
      Codecs::PresenceMap & pmap,
      Codecs::Encoder & encoder,
      const Messages::MessageAccessor & fieldSet) const
    {
      // get the value from the application data
      Messages::FieldCPtr field;
      if(fieldSet.getField(identity_->name(), field))
      {
        INTEGER_TYPE value;
        field->getValue(value);
        if(value != typedValue_)
        {
          encoder.reportError("[ERR U02]", "Constant value does not match application data.", *identity_);
        }

        if(!isMandatory())
        {
          pmap.setNextField(true);
        }
      }
      else // not defined in fieldset
      {
        if(isMandatory())
        {
          encoder.reportError("[ERR U01]", "Missing mandatory field.", *identity_);
        }
        pmap.setNextField(false);
      }
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    void
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    encodeDefault(
      Codecs::DataDestination & destination,
      Codecs::PresenceMap & pmap,
      Codecs::Encoder & encoder,
      const Messages::MessageAccessor & fieldSet) const
    {
      // get the value from the application data
      Messages::FieldCPtr field;
      if(fieldSet.getField(identity_->name(), field))
      {
        INTEGER_TYPE value;
        field->getValue(value);
        if(typedValueIsDefined_ && value == typedValue_)
        {
          pmap.setNextField(false); // not in stream. use default
        }
        else
        {
          pmap.setNextField(true); // != default.  Send value
          if(!isMandatory())
          {
            // gcc produces bogus warning on the following line see GCC Bugzilla Bug 11856
            // if(!SIGNED || value >= 0)
            if(!SIGNED || value > 0 || value == 0)
            {
              ++value;
            }
          }
          if(SIGNED)
          {
            encodeSignedInteger(destination, encoder.getWorkingBuffer(), value);
          }
          else
          {
            encodeUnsignedInteger(destination, encoder.getWorkingBuffer(), value);
          }
        }
      }
      else // not defined in fieldset
      {
        if(isMandatory())
        {
          encoder.reportError("[ERR U01]", "Missing mandatory field.", *identity_);
        }
        if(fieldOp_->hasValue())
        {
          pmap.setNextField(true);
          destination.putByte(nullInteger);
        }
        else
        {
          pmap.setNextField(false);
        }
      }
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    void
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    encodeCopy(
      Codecs::DataDestination & destination,
      Codecs::PresenceMap & pmap,
      Codecs::Encoder & encoder,
      const Messages::MessageAccessor & fieldSet) const
    {
      // declare a couple of variables...
      bool previousIsKnown = false;
      bool previousNotNull = false;
      INTEGER_TYPE previousValue = 0;

      // ... then initialize them from the dictionary
#if 0
      Messages::FieldCPtr previousField;
      if(fieldOp_->findDictionaryField(encoder, previousField))
      {
        if(!previousField->isType(typedValue_))
        {
          encoder.reportError("[ERR D4]", "Previous value type mismatch.", *identity_);
        }
        previousIsKnown = true;
        previousNotNull = previousField->isDefined();
        if(previousNotNull)
        {
          previousField->getValue(previousValue);
        }
      }
#else
      if(fieldOp_->getDictionaryValue(encoder, previousValue))
      {
        previousIsKnown = true;
        previousNotNull = true;
        /// @TOOD: distinguish unknown from explicitly null?
      }
#endif
      if(!previousIsKnown && fieldOp_->hasValue())
      {
        previousIsKnown = true;
        previousNotNull = true;
        previousValue = typedValue_;
#if 0
        fieldOp_->setDictionaryValue(encoder, VALUE_TYPE::create(previousValue));
#else
        fieldOp_->setDictionaryValue(encoder, typedValue_);
#endif
      }

      // get the value from the application data
      Messages::FieldCPtr field;
      if(fieldSet.getField(identity_->name(), field))
      {
        INTEGER_TYPE value;
        field->getValue(value);

        if(previousNotNull && previousValue == value)
        {
          pmap.setNextField(false); // not in stream, use copy
        }
        else
        {
          INTEGER_TYPE nullableValue = value;
          if(!isMandatory())
          {
            // gcc produces bogus warning on the following line see GCC Bugzilla Bug 11856
            // if(!SIGNED || nullableValue >= 0)
            if(!SIGNED || nullableValue > 0 || nullableValue == 0)
            {
              ++nullableValue;
            }
          }
          pmap.setNextField(true);// value in stream
          if(SIGNED)
          {
            encodeSignedInteger(destination, encoder.getWorkingBuffer(), nullableValue);
          }
          else
          {
            encodeUnsignedInteger(destination, encoder.getWorkingBuffer(), nullableValue);
          }
#if 0
          field = VALUE_TYPE::create(value);
          fieldOp_->setDictionaryValue(encoder, field);
#else
          fieldOp_->setDictionaryValue(encoder, value);
#endif

        }
      }
      else // not defined in fieldset
      {
        if(isMandatory())
        {
          encoder.reportError("[ERR U01]", "Missing mandatory field.", *identity_);
          // if reportEror returns we're being lax about the rules.
          // Let the copy happen
          pmap.setNextField(false);
        }
        else
        {
          // Missing optional field.  If we have a previous, non-null value
          // we need to explicitly null it out.  Otherwise just don't send it.
          if(previousIsKnown && previousNotNull)
          {
            pmap.setNextField(true);// value in stream
            destination.putByte(nullInteger);
#if 0
            field = VALUE_TYPE::createNull();
            fieldOp_->setDictionaryValue(encoder, field);
#else
            fieldOp_->setDictionaryValueNull(encoder);
#endif

          }
          else
          {
            pmap.setNextField(false);
          }
        }
      }
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    void
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    encodeDelta(
      Codecs::DataDestination & destination,
      Codecs::PresenceMap & /*pmap*/,
      Codecs::Encoder & encoder,
      const Messages::MessageAccessor & fieldSet) const
    {
      // declare a couple of variables...
      bool previousIsKnown = false;
      bool previousNotNull = true;
      INTEGER_TYPE previousValue = 0;

      // ... then initialize them from the dictionary
#if 0
      Messages::FieldCPtr previousField;
      if(fieldOp_->findDictionaryField(encoder, previousField))
      {
        if(!previousField->isType(typedValue_))
        {
          encoder.reportError("[ERR D4]", "Previous value type mismatch.", *identity_);
        }
        else
        {
          previousIsKnown = true;
          previousNotNull = previousField->isDefined();
          if(previousNotNull)
          {
            previousField->getValue(previousValue);
          }
        }
      }
#else
      if(fieldOp_->getDictionaryValue(encoder, previousValue))
      {
        previousIsKnown = true;
        previousNotNull = true;
        /// @TOOD: distinguish unknown from explicitly null?
      }
#endif
      if(!previousIsKnown && fieldOp_->hasValue())
      {
        previousIsKnown = true;
        previousNotNull;
        previousValue = typedValue_;
#if 0
        fieldOp_->setDictionaryValue(encoder, VALUE_TYPE::create(previousValue));
#else
        fieldOp_->setDictionaryValue(encoder, typedValue_);
#endif

      }

      // get the value from the application data
      Messages::FieldCPtr field;
      if(fieldSet.getField(identity_->name(), field))
      {
        INTEGER_TYPE value;
        field->getValue(value);
        int64 deltaValue = int64(value) - int64(previousValue);
        if(!isMandatory())
        {
          if(deltaValue >= 0)
          {
            deltaValue += 1;
          }
        }
        encodeSignedInteger(destination, encoder.getWorkingBuffer(), deltaValue);
        if(!previousIsKnown  || value != previousValue)
        {
#if 0
          field = VALUE_TYPE::create(value);
          fieldOp_->setDictionaryValue(encoder, field);
#else
          fieldOp_->setDictionaryValue(encoder, value);
#endif

        }

      }
      else // not defined in fieldset
      {
        if(isMandatory())
        {
          encoder.reportError("[ERR U01]", "Missing mandatory field.", *identity_);
          encodeSignedInteger(destination, encoder.getWorkingBuffer(), 0);
        }
        else
        {
          destination.putByte(nullInteger);
        }
      }
    }

    template<typename INTEGER_TYPE, ValueType::Type VALUE_TYPE, bool SIGNED>
    void
    FieldInstructionInteger<INTEGER_TYPE, VALUE_TYPE, SIGNED>::
    encodeIncrement(
      Codecs::DataDestination & destination,
      Codecs::PresenceMap & pmap,
      Codecs::Encoder & encoder,
      const Messages::MessageAccessor & fieldSet) const
    {
      // declare a couple of variables...
      bool previousIsKnown = false;
      bool previousNotNull = false;
      INTEGER_TYPE previousValue = 0;

      // ... then initialize them from the dictionary
#if 0
      Messages::FieldCPtr previousField;
      if(fieldOp_->findDictionaryField(encoder, previousField))
      {
        if(!previousField->isType(typedValue_))
        {
          encoder.reportError("[ERR D4]", "Previous value type mismatch.", *identity_);
        }
        else
        {
          previousIsKnown = true;
          previousNotNull = previousField->isDefined();
          if(previousNotNull)
          {
            previousField->getValue(previousValue);
          }
        }
      }
#else
      if(fieldOp_->getDictionaryValue(encoder, previousValue))
      {
        previousIsKnown = true;
        previousNotNull = true;
        /// @TOOD: distinguish unknown from explicitly null?
      }
#endif

      if(!previousIsKnown && fieldOp_->hasValue())
      {
        previousIsKnown = true;
        previousNotNull = true;
        // pretend the previous value was the value attribute - 1 so that
        // the increment will produce cause the initial value to be sent.
        previousValue = typedValue_ - 1;
#if 0
        fieldOp_->setDictionaryValue(encoder, VALUE_TYPE::create(previousValue));
#else
        fieldOp_->setDictionaryValue(encoder, previousValue);
#endif

      }

      // get the value from the application data
      Messages::FieldCPtr field;
      if(fieldSet.getField(identity_->name(), field))
      {
        INTEGER_TYPE value;
        field->getValue(value);
        INTEGER_TYPE nullableValue = value;
        if(previousValue + 1 == value)
        {
          pmap.setNextField(false);
        }
        else
        {
          if(!isMandatory())
          {
            // gcc produces bogus warning on the following line see GCC Bugzilla Bug 11856
            // if(!SIGNED || value >= 0)
            if(!SIGNED || nullableValue > 0 || nullableValue == 0)
            {
              ++nullableValue;
            }
          }
          pmap.setNextField(true);
          if(SIGNED)
          {
            encodeSignedInteger(destination, encoder.getWorkingBuffer(), nullableValue);
          }
          else
          {
            encodeUnsignedInteger(destination, encoder.getWorkingBuffer(), nullableValue);
          }
        }
#if 0
        field = VALUE_TYPE::create(value);
        fieldOp_->setDictionaryValue(encoder, field);
#else
        fieldOp_->setDictionaryValue(encoder, value);
#endif

      }
      else // not defined in fieldset
      {
        if(isMandatory())
        {
          encoder.reportError("[ERR U01]", "Missing mandatory field.", *identity_);
          // if reportError returns we're being lax about the encoding rules.
          // just let it increment
          pmap.setNextField(false);
        }
        else
        {
          // Missing optional field.  If we have a previous, non-null value
          // we need to explicitly null it out.  Otherwise just don't send it.
          if (previousIsKnown && previousNotNull)
          {
            pmap.setNextField(true);// value in stream
            destination.putByte(nullInteger);
#if 0
            field = VALUE_TYPE::createNull();
            fieldOp_->setDictionaryValue(encoder, field);
#else
            fieldOp_->setDictionaryValueNull(encoder);
#endif
          }
          else
          {
            pmap.setNextField(false);
          }
        }
      }
    }
  }
}
#endif // FIELDINSTRUCTIONINTEGER_H
