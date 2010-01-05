// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDOP_H
#define FIELDOP_H
#include "FieldOp_fwd.h"
#include <Common/QuickFAST_Export.h>

#include <Messages/Field_fwd.h>
#include <Messages/MessageAccessor.h>
#include <Messages/ValueMessageBuilder_fwd.h>
#include <Codecs/Context.h>
#include <Codecs/FieldInstruction_fwd.h>
#include <Codecs/SchemaElement.h>
#include <Codecs/DataSource_fwd.h>
#include <Codecs/DataDestination_fwd.h>
#include <Codecs/Decoder_fwd.h>
#include <Codecs/Encoder_fwd.h>
#include <Codecs/PresenceMap.h>
#include <Codecs/DictionaryIndexer_fwd.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief Base class for field operation dispatching.
    ///
    /// Supports applying an encoding/decoding operation to a particular field type.
    /// This is a double-dispatch problem, so the virtual methods defined for this class
    /// call the appropriate virtual methods in the targeted FieldInstruction.
    ///
    /// @todo: suppress dictionary index for operations that don't use a dictionary
    class QuickFAST_Export FieldOp
      : public SchemaElement
    {
    public:
      /// basic constructor
      FieldOp();

      /// @brief Does this operation use the presence map under the given presence= setting?
      /// @param mandatory is true if presence="mandatory" was specified for this field instruction.
      /// @returns true if a presence map bit is used.
      virtual bool usesPresenceMap(bool mandatory)const = 0;

      /// @brief Does this operation use a dictionary entry?
      /// @returns true if a dictionary entry might be needed by this operation.
      virtual bool usesDictionary() const = 0;

      /// @brief Dispatch decode to the correct operator
      /// @param instruction defines the type of data being decoded
      /// @param source supplies the stream of FAST encoded data
      /// @param pmap determines whether the field is represented in the stream
      /// @param decoder defines the context in which decoding is happening
      /// @param fieldSet receives the decoded field.
      virtual bool decode(
        const Codecs::FieldInstruction & instruction,
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & fieldSet) const = 0;

      /// @brief Encode a field to a data destination.
      ///
      /// @param instruction being processed.
      /// @param destination to which the encoded data is written
      /// @param pmap is updated to indicate whether the data exists in the encoded stream
      /// @param encoder defines the context in which the encoding is taking place
      /// @param fieldSet contains the field to be encoded (if any)
      virtual void encode(
        const Codecs::FieldInstruction & instruction,
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const = 0;

      /// @brief Set the appropriate default value in the FieldInstruction
      /// @param instruction is the instruction that will ultimately process the value.
      virtual void setDefaultValue(
        Codecs::FieldInstruction & instruction) const = 0;

      /// @brief Was the value= attribute used?
      /// @returns true if it was
      bool hasValue()const
      {
        return valueIsDefined_;
      }

      /// @brief Get the value= attribute
      /// @returns the value specified as a string
      const std::string & getValue()const
      {
        return value_;
      }

      /// @brief Implement the value= attribute
      /// @param value from the value= attribute.
      void setValue(const std::string & value)
      {
        value_ = value;
        valueIsDefined_ = true;
      }

      /// @brief Set the pmap bit to be used for this field
      ///
      /// This is not a part of the FAST spec. It is here to support
      /// exchanges that do not conform to the spec (ArcaBook for one)
      void setPMapBit(size_t pmapBit)
      {
        pmapBit_ = pmapBit;
        pmapBitValid_ = true;
      }

      /// @brief Implement the key= attribute
      /// @param key is the value of the attribute.
      void setKey(const std::string & key)
      {
        key_ = key;
      }

      /// @brief implement the nsKey= attribute
      /// @param keyNamespace is the value specified for nsKey
      void setKeyNamespace(const std::string & keyNamespace)
      {
        keyNamespace_ = keyNamespace;
      }


      /// @brief Store the current value in the dictionary entry assigned to this field.
      /// @param context contains the dictionaries for this encode/decode
      /// @param value is the entry to store in the dictionary
#if 0
      void setDictionaryValue(
        Context & context,
        const Messages::FieldCPtr & value);

      /// @brief Retrieve the current value for this field from the dictionary.
      /// @param context contains the dictionaries for this encode/decode
      /// @param value will point to the entry that was found
      /// @returns true if a valid entry was found.
      bool findDictionaryField(
        Context & context,
        Messages::FieldCPtr & value);

#endif
      /// @brief Implement the dictionary= attribute
      /// @param name is the name of the dictionary to use.
      void setDictionaryName(const std::string & name)
      {
        dictionaryName_ = name;
      }

      /// @brief Assign a dictionary entry to the field associated with this operation.
      /// @param indexer assigns the index.
      /// @param dictionaryName is the parent's dictionary name (inherited unless overridden)
      /// @param typeName is the application type for this instruction
      /// @param typeNamespace is the namespace to qualify the application type.
      /// @param fieldName is the name of this field.
      /// @param fieldNamespace qualifies fieldName
      void indexDictionaries(
        DictionaryIndexer & indexer,
        const std::string & dictionaryName,
        const std::string & typeName,
        const std::string & typeNamespace,
        const std::string & fieldName,
        const std::string & fieldNamespace);

      void setDictionaryValueUndefined(Context & context)
      {
        context.setDictionaryValueUndefined(dictionaryIndex_);
      }

      void setDictionaryValueNull(Context & context)
      {
        context.setDictionaryValueNull(dictionaryIndex_);
      }

      void setDictionaryValue(Context & context, const int64 value)
      {
        context.setDictionaryValue(dictionaryIndex_, value);
      }

      void setDictionaryValue(Context & context, const uint64 value)
      {
        context.setDictionaryValue(dictionaryIndex_, value);
      }

      void setDictionaryValue(Context & context, const int32 value)
      {
        context.setDictionaryValue(dictionaryIndex_, value);
      }

      void setDictionaryValue(Context & context, const uint32 value)
      {
        context.setDictionaryValue(dictionaryIndex_, value);
      }

      void setDictionaryValue(Context & context, const int16 value)
      {
        context.setDictionaryValue(dictionaryIndex_, value);
      }

      void setDictionaryValue(Context & context, const uint16 value)
      {
        context.setDictionaryValue(dictionaryIndex_, value);
      }

      void setDictionaryValue(Context & context, const int8 value)
      {
        context.setDictionaryValue(dictionaryIndex_, value);
      }

      void setDictionaryValue(Context & context, const uchar value)
      {
        context.setDictionaryValue(dictionaryIndex_, value);
      }

      void setDictionaryValue(Context & context, const Decimal& value)
      {
        context.setDictionaryValue(dictionaryIndex_, value);
      }

      void setDictionaryValue(Context & context, const unsigned char * value, size_t length)
      {
        context.setDictionaryValue(dictionaryIndex_, value, length);
      }

      void setDictionaryValue(Context & context, const char * value)
      {
        context.setDictionaryValue(dictionaryIndex_, value);
      }

      void setDictionaryValue(Context & context, const std::string& value)
      {
        context.setDictionaryValue(dictionaryIndex_, value);
      }

      template<typename VALUE_TYPE>
      Context::DictionaryStatus getDictionaryValue(Context & context, VALUE_TYPE & value)
      {
        return context.getDictionaryValue(dictionaryIndex_, value);
      }
#if 0
      Context::DictionaryStatus getDictionaryValue(Context & context, uint64 & value)
      {
        return context.getDictionaryValue(dictionaryIndex_, value);
      }

      Context::DictionaryStatus getDictionaryValue(Context & context, int32 & value)
      {
        return context.getDictionaryValue(dictionaryIndex_, value);
      }

      Context::DictionaryStatus getDictionaryValue(Context & context, uint32 & value)
      {
        return context.getDictionaryValue(dictionaryIndex_, value);
      }

      Context::DictionaryStatus getDictionaryValue(Context & context, int16 & value)
      {
        return context.getDictionaryValue(dictionaryIndex_, value);
      }

      Context::DictionaryStatus  getDictionaryValue(Context & context, uint16 & value)
      {
        return context.getDictionaryValue(dictionaryIndex_, value);
      }

      Context::DictionaryStatus  getDictionaryValue(Context & context, int8 & value)
      {
        return context.getDictionaryValue(dictionaryIndex_, value);
      }

      Context::DictionaryStatus  getDictionaryValue(Context & context, uchar & value)
      {
        return context.getDictionaryValue(dictionaryIndex_, value);
      }

      Context::DictionaryStatus  getDictionaryValue(Context & context, Decimal & value)
      {
        return context.getDictionaryValue(dictionaryIndex_, value);
      }

      Context::DictionaryStatus  getDictionaryValue(Context & context, const char *& value)
      {
        return context.getDictionaryValue(dictionaryIndex_, value);
      }

      Context::DictionaryStatus  getDictionaryValue(Context & context, std::string& value)
      {
        return context.getDictionaryValue(dictionaryIndex_, value);
      }
#endif

      Context::DictionaryStatus  getDictionaryValue(Context & context, const unsigned char *& value, size_t &length)
      {
        return context.getDictionaryValue(dictionaryIndex_, value, length);
      }

    protected:
            /// Value specified by the operator associated with this instruction [as a string]
      std::string value_;
      /// True if a value was provided
      bool valueIsDefined_;
      /// Dictionary key associated with this operation
      std::string key_;
      /// Namespace within which key is defined
      std::string keyNamespace_;

      /// Dictionary name to be used for this operation
      std::string dictionaryName_;

      /// An index to gain quick access to the dictionary entry for this op
      size_t dictionaryIndex_;
      /// true if dictionaryIndex_ is valid;
      bool dictionaryIndexValid_;

      /// For non-conforming implmentations that assign specific pmap bits....
      size_t pmapBit_;
      /// Indicate that pmapBit_ is to be used
      bool pmapBitValid_;

    };
  }
}
#endif // FIELDOP_H
