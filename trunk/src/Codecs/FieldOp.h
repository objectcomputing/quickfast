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
#include <Messages/FieldSet_fwd.h>
#include <Codecs/FieldInstruction_fwd.h>
#include <Codecs/SchemaElement.h>
#include <Codecs/DataSource_fwd.h>
#include <Codecs/DataDestination_fwd.h>
#include <Codecs/Decoder_fwd.h>
#include <Codecs/Encoder_fwd.h>
#include <Codecs/PresenceMap.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief Base class for field operation dispatching.
    ///
    /// Supports applying an encoding/decoding operation to a particular field type.
    /// This is a double-dispatch problem, so the virtual methods defined for this class
    /// call the appropriate virtual methods in the targeted FieldInstruction.
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
        Messages::FieldSet & fieldSet) const = 0;

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
        const Messages::FieldSet & fieldSet) const = 0;

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

      /// @brief get the key= attribute
      /// @returns the key specified (empty if key= was not used.)
      const std::string & getKey()const
      {
        return key_;
      }

      /// @brief Implement the key= attribute
      /// @param key is the value of the attribute.
      void setKey(const std::string & key)
      {
        key_ = key;
      }

      /// @brief Get the namespace for the key
      /// @returns the namespace (if any)
      const std::string & getKeyNamespace()const
      {
        return keyNamespace_;
      }

      /// @brief implement the nsKey= attribute
      /// @param keyNamespace is the value specified for nsKey
      void setKeyNamespace(const std::string & keyNamespace)
      {
        keyNamespace_ = keyNamespace;
      }

      /// @brief get the name of the dictionary to use for this operation
      /// @returns the dictionary name or an empty string if none
      const std::string & getDictionaryName()const
      {
        return dictionaryName_;
      }

      /// @brief Implement the dictionary= attribute
      /// @param name is the name of the dictionary to use.
      void setDictionaryName(const std::string & name)
      {
        dictionaryName_ = name;
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

    };
  }
}
#endif // FIELDOP_H
