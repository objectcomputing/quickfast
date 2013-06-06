// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTION_H
#define FIELDINSTRUCTION_H

#include "FieldInstruction_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>
#include <Common/Constants.h>
#include <Common/WorkingBuffer.h>
#include <Common/Exceptions.h>
#include <Messages/Message_fwd.h>
#include <Messages/ValueMessageBuilder_fwd.h>
#include <Messages/FieldIdentity.h>
#include <Codecs/SchemaElement.h>
#include <Codecs/DataSource.h>
#include <Codecs/Context.h>
#include <Codecs/FieldOp.h>
#include <Codecs/PresenceMap.h>

#include <Common/Profiler.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief Instruction: an element of a Template.
    ///
    /// Specializations: TemplateRef, Group, Sequence,
    /// String(Ascii), String(unicode), ByteVector
    /// UINT64, INT64, UINT32, INT32, Decimal
    ///
    /// Note typeref is not a FieldInstruction, it's a setApplicationType()
    /// call on the containing SegmentBody
    ///
    /// <pre>
    /// Instruction Attribute           method to call
    /// =====================           ==============
    /// name="name" [required]          constructor(name, namespace)
    /// ns="namespace" [optional]
    ///
    /// Presence=mandatory|optional     setPresence();
    /// dictionary=(template|type|global|"user")
    ///                                 setDictionaryName();
    /// key="token"                     setKey();
    /// id="id"                         setId();
    ///
    /// Embedded Operations
    ///  i.e.: constant, default, copy, increment, delta, tail, "nop")
    ///  Value=literal (for constant, default, copy)
    ///                                 setFieldOp(FieldOPXXXX, value);
    ///
    /// </pre>

    class QuickFAST_Export FieldInstruction
      : public SchemaElement
    {
    protected:
      /// @brief Derived class can constuct given a name and namespace
      /// @param name is the localname for this field
      /// @param fieldNamespace qualifies localname
      FieldInstruction(const std::string & name, const std::string & fieldNamespace);

      /// @brief Derived class can also construct an anonymous Field Instruction
      FieldInstruction();
    public:
      /// @brief a typical virtual destructor.
      virtual ~FieldInstruction();

      /// @brief set the id of this field
      /// @param id is the id for this field.
      void setId(const field_id_t & id)
      {
        identity_.setId(id);
      }

      /// @brief set the name of this field instruction after construction
      /// @param name is the localname for this field
      void setName(const std::string & name)
      {
        identity_.setName(name);
      }

      /// @brief set the namespace of this field instruction after construction
      /// @param fieldNamespace qualifies localname
      void setNamespace(const std::string & fieldNamespace)
      {
        identity_.setNamespace(fieldNamespace);
      }

      /// @brief Indicate that the field is mandatory in the application record.
      /// Default if not specified is true.
      /// @param mandatory true for presence="mandatory"; false for presence="optional"
      virtual void setPresence(bool mandatory);

      /// @brief Provide a way to control overflow checking during decoding.
      /// @param allowOverflow is true to disable/false to enable overflow checking (default is false)
      virtual void setIgnoreOverflow(bool allowOverflow);

      /// @brief Set a field operation
      ///
      /// Assigns the appropriate dispatching object to this field instruction.
      void setFieldOp(FieldOpPtr fieldOp);

      /// @brief Get the currently used field operation
      FieldOpCPtr getFieldOp() const;

      virtual void setApplicationType(const std::string & type, const std::string & ns)
      {
        applicationType_ = type;
        applicationTypeNamespace_ = ns;
        if(applicationTypeNamespace_.empty())
        {
          qualifiedApplicationType_ = applicationType_;
        }
        else
        {
          qualifiedApplicationType_ = applicationTypeNamespace_ + "." + applicationType_;
        }
      }

      /// @brief get the Application Type assigned to this field instruction
      /// @returns the type assigned to this Fiel
      const std::string & getApplicationType() const
      {
        return qualifiedApplicationType_;
      }

      /// @brief Retrieve the field's name.
      /// @returns the field name.
      const std::string & getName() const
      {
        return identity_.name();
      }

      /// @brief Retrieve the field's id
      /// @returns the fields id
      const field_id_t &
        getId()const
      {
        return identity_.id();
      }

      /// @brief Retrieve the field's identity.
      /// @returns the field identity.
      const Messages::FieldIdentity & getIdentity()const
      {
        return identity_;
      }

      /// @brief Is the field mandatory in the application record?
      /// @returns true if the field is mandatory.
      bool isMandatory()const
      {
        return mandatory_;
      }

      /// @brief Implement the dictionary= attribute.
      ///
      /// Defines an dictionary to be used for this element.
      /// Not applicable to all field instructions so the default implementation
      /// throws a TemplateDefinition error.
      /// @param name is the dictionary= attribute of the segment-defining element
      virtual void setDictionaryName(const std::string & name);


      /// @brief set the segment body which defines the contents of a sequence or group.
      ///
      /// Does not apply to other field instructions so the default implementation
      /// throws a TemplateDefinition error.
      /// @param segment is a set of field instructions that define the element
      virtual void setSegmentBody(Codecs::SegmentBodyPtr segment);


      /// @brief Access the segment body for this field instruction if any
      /// @param[out] segment is set to point to the segment.
      /// @returns true if the segment exists
      virtual bool getSegmentBody(Codecs::SegmentBodyPtr & segment) const;

      /// @brief Alternative access to the segment body.
      /// @returns the segment body pointer.
      /// @throws templateDefinitionError if segment body is not defined
      /// @todo: Unused -- is this necessary?
      virtual const Codecs::SegmentBodyPtr& getSegmentBody();

      /// @brief How many fields will this field instruction produce
      ///
      /// Includes merged groups and templateRefs. If not merged, then the answer is 1
      /// @brief parent SegmentBody is needed to check for merge
      virtual size_t fieldCount(const SegmentBody & parent)const;

      /// @brief Assign a dictionary index to this field and any subfields.
      /// @param indexer assigns the index.
      /// @param dictionaryName is the parent's dictionary name (inherited unless overridden)
      /// @param typeName is the application type for this instruction
      /// @param typeNamespace is the namespace to qualify the application type.
      virtual void indexDictionaries(
        DictionaryIndexer & indexer,
        const std::string & dictionaryName,
        const std::string & typeName,
        const std::string & typeNamespace);

      /// @brief Decode the field from a data source.
      ///
      /// @param[in] source supplies the data
      /// @param[in] pmap identifies fields present in the stream
      /// @param decoder provides context for decoding
      /// @param[out] builder receives the decoded fields
      void decode(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder)const
      {
        fieldOp_->decode(*this, source, pmap, decoder, builder);
      }

      /// @brief Decode the field from a data source.
      ///
      /// @param destination receives the encoded data
      /// @param pmap is set to indicate which fields are explicitly represented in the encoded data stream
      /// @param encoder provides the context for encoding
      /// @param accessor supplies the data to be encoded
      void encode(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor)const
      {
        return fieldOp_->encode(*this, destination, pmap, encoder, accessor);
      }

      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueNop();
      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueConstant();
      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueDefault();
      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueCopy();
      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueDelta();
      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueIncrement();
      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueTail();

      ///////////////////
      // Decoding support

      /// @brief Decode when no operation is specified.
      /// @see decode()
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] builder
      virtual void decodeNop(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const = 0;

      /// @brief Decode when &lt;constant> operation is specified.
      /// @see decode()
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] builder
      virtual void decodeConstant(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      /// @brief Decode when &lt;default> operation is specified.
      /// @see decode()
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] builder
      virtual void decodeDefault(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      /// @brief Decode when &lt;copy> operation is specified.
      /// @see decode()
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] builder
      virtual void decodeCopy(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      /// @brief Decode when &lt;copy> operation is specified using specific pmap bit.
      ///
      /// Special support for Arca's FAST-like-ish encoding scheme.
      /// @see decode()
      /// @param[in] source for the FAST data
      /// @param[in] pmapValue prefetched to  indicate field presence
      /// @param[in] decoder driving this process
      /// @param[out] builder
      virtual void decodeCopy(
        Codecs::DataSource & source,
        bool pmapValue,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      /// @brief Decode when &lt;delta> operation is specified.
      /// @see decode()
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] builder
      virtual void decodeDelta(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      /// @brief Decode when &lt;increment> operation is specified.
      /// @see decode()
      /// Not pure because not all types support increment operator.
      /// Default implementation throws bad operation
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] builder
      virtual void decodeIncrement(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      /// @brief Decode when &lt;increment> operation is specified.
      /// @see decode()
      /// Not pure because not all types support increment operator.
      /// Default implementation throws bad operation
      /// @param[in] source for the FAST data
      /// @param[in] pmapValue prefetched to  indicate field presence
      /// @param[in] decoder driving this process
      /// @param[out] builder
      virtual void decodeIncrement(
        Codecs::DataSource & source,
        bool pmapValue,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      /// @brief Decode when &lt;tail> operation is specified.
      /// @see decode()
      /// Not pure because not all types support tail operator.
      /// Default implementation throws bad operation
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] builder
      virtual void decodeTail(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      ///////////////////
      // Encoding support

      /// @brief Encode when no operation is specified.
      /// @see encode()
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param accessor
      virtual void encodeNop(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const = 0;

      /// @brief Encode when &lt;constant> operation is specified.
      /// @see encode()
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param accessor
      virtual void encodeConstant(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      /// @brief Encode when &lt;default> operation is specified.
      /// @see encode()
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param accessor
      virtual void encodeDefault(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      /// @brief Encode when &lt;copy> operation is specified.
      /// @see encode()
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param accessor
      virtual void encodeCopy(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      /// @brief Encode when &lt;delta> operation is specified.
      /// @see encode()
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param accessor
      virtual void encodeDelta(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      /// @brief Encode when &lt;increment> operation is specified.
      /// @see encode()
      /// Default implementation throws bad operation
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param accessor
      virtual void encodeIncrement(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      /// @brief Encode when &lt;tail> operation is specified.
      /// @see encode()
      /// Default implementation throws bad operation
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param accessor
      virtual void encodeTail(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      /////////////////////////
      // Basic Decoding methods
      // Developer Note:   These are here to
      // support all derived fielddefs.
      // They are static to make them more generally useful (testable)


      /// @brief Encode a string that's nullable, but not null.
      /// @param destination to which the string will be written.
      /// @param value to be written.
      static void encodeNullableAscii(DataDestination & destination, const StringBuffer & value);

      /// @brief Encode a string.
      /// @param destination to which the string will be written.
      /// @param value to be written.
      static void encodeAscii(DataDestination & destination, const StringBuffer & value);

      /// @brief Helper routine to encode a blob represented as a string; into a destination
      /// @param destination to which the data will be written
      /// @param value to be written to the destination
      static void encodeBlobData(DataDestination & destination, const StringBuffer & value);

      /// @brief Encode signed integer
      ///
      /// Works with all signed integer types.  Variable byte length encoding
      /// assures the most compact result.
      static void encodeSignedInteger(DataDestination & destination, WorkingBuffer & buffer, int64 value);

      /// @brief Encode signed integer
      ///
      /// Works with all signed integer types.  Variable byte length encoding
      /// assures the most compact result.
      /// @param destination receives encoded results
      /// @param buffer provides working space
      /// @param value is the data to be encoded
      static void encodeUnsignedInteger(DataDestination & destination, WorkingBuffer & buffer, uint64 value);

      /// @brief basic decoding for signed integer types.
      ///
      /// Ignores presence map, field operators, etc.  All it does
      /// is decode an integer from the data source./
      /// Used to decode Int32 and Int64 fields.
      /// Also used for exponent and mantissa of Decimal fields
      /// @param[in] source supplies the data
      /// @param context in which the decoding occurs.
      /// @param[out] value returns the result
      /// @param[in] name of this field to be used in error messages
      /// @param[in] allowOversize ignores one bit of overflow to cope
      ///            with out-of-range deltas (see section 6.3.7.1 of the Fast Specification v1x1)
      /// @param[in] ignoreOverflow ignores overflows completely to cope with funky ARCA encoding.
      /// @returns true if successful; false if EOF
      /// @throws OverflowError if the decoded value doesn't fit the supplied type
      template<typename IntType>
      static void decodeSignedInteger(
        Codecs::DataSource & source,
        Codecs::Context & context,
        IntType & value,
        const std::string & name,
        bool allowOversize = false,
        bool ignoreOverflow = false);

      /// @brief basic decoding for signed integer types.
      ///
      /// Ignores presence map, field operators, etc.  All it does
      /// is decode an integer from the data source.
      /// Used to decode UInt32 and UInt64 fields.
      /// Also used for length portion of BitVector and Utf8 fields
      /// @param[in] source supplies the data
      /// @param context in which the decoding occurs.
      /// @param[out] value returns the result
      /// @param[in] name of this field to be used in error messages
      /// @param[in] ignoreOverflow ignores overflows completely to cope with funky ARCA encoding.
      /// @returns true if successful; false if EOF
      /// @throws OverflowError if the decoded value doesn't fit the supplied type
      template<typename UnsignedIntType>
      static void decodeUnsignedInteger(
        Codecs::DataSource & source,
        Codecs::Context & context,
        UnsignedIntType & value,
        const std::string & name,
        bool ignoreOverflow = false);

      /// @brief Check nullable signed or unsigned integer field for null value
      ///
      /// Fixes value to reverse the effect of null encoding.
      /// @param value the value as returned by decodeSignedInteger() or decodeUnsignedInteger()
      /// @returns true if this is a null field
      template<typename IntType>
      static bool checkNullInteger(IntType & value);

      /// @brief Basic decoding for ASCII strings
      /// @param[in] source supplies the data
      /// @param buffer to receive the string
      static bool decodeAscii(
        Codecs::DataSource & source,
        WorkingBuffer & buffer);

      /// @brief Check for zero preamble to encode empty string
      ///
      /// If the string is nullable use checkNullAscii() first.
      /// @param buffer as returned by decodeAscii()
      static bool checkEmptyAscii(WorkingBuffer & buffer);

      /// @brief Check for zero preamble to encode null string
      ///
      /// Must be called before checkEmptyAscii() for nullable strings.
      /// @param buffer as returned by decodeAscii()
      static bool checkNullAscii(WorkingBuffer & buffer);

      /// @brief Helper method to find the longest match at the beginning of two strings
      /// @param previous one of the strings
      /// @param value the other string
      /// @returns a count of bytes that match exactly at the beginning of the strings
      static size_t longestMatchingPrefix(
        const std::string & previous,
        const std::string & value);

      /// @brief Helper method to find the longest match at the end of two strings
      /// @param previous one of the strings
      /// @param value the other string
      /// @returns a count of bytes that match exactly at the end of the strings
      static size_t longestMatchingSuffix(
        const std::string & previous,
        const std::string & value);


      /// @brief Basic decoding for ByteVectors and Utf8 strings
      ///
      /// Assumes byte vector of approriate length is present in input stream.
      /// @param[in] decoder for which this decoding is being done
      /// @param[in] source supplies the data
      /// @param[in] name of this field to be used in error messages
      /// @param[out] buffer receives the result
      /// @param[in] length expected
      /// @throws EncodingError if not enough data is available
      static void decodeByteVector(
        Codecs::Context & decoder,
        Codecs::DataSource & source,
        const std::string & name,
        WorkingBuffer & buffer,
        size_t length);

      /// @brief do final processing of this field instruction after parsing entire template set.
      virtual void finalize(Codecs::TemplateRegistry & registry);

      /// @brief Help preserve sanity during finalize.  Identify "include" instructions.
      virtual bool isPossiblyRecursive() const;

      /// @brief how many presence map bits are used by this field instruction?
      size_t getPresenceMapBitsUsed() const
      {
        return presenceMapBitsUsed_;
      }

      /// @brief Write the fieldInstruction in human readable form.
      ///
      /// @param output is the stream to which the display will be written
      /// @param indent is the number of spaces to appear at the beginning of each line.
      virtual void display(std::ostream & output, size_t indent = 0) const;

      /// @brief identify this field instructoin
      ///
      /// a touch of manual RTTI
      virtual ValueType::Type fieldInstructionType()const = 0;

      /// @brief display the body of compound field instructions (Groups and Sequences)
      virtual void displayBody(std::ostream & output, size_t indent)const;

    private:
      /// @brief Interpret initial or default value attribute.
      /// @param value for this instruction as specified in XML
      virtual void interpretValue(const std::string & value) = 0;

    private:
      FieldInstruction(const FieldInstruction &);
      FieldInstruction & operator = (const FieldInstruction &);

    protected:
      /// Identifies this field
      Messages::FieldIdentity identity_;
      /// Application type associated with this instruction
      std::string applicationType_;
      /// Namespace in which the application type is defined.
      std::string applicationTypeNamespace_;
      /// Cached applicationTypeNamespace.applicationType
      std::string qualifiedApplicationType_;
      /// Pointer to field operator dispatcher.
      FieldOpPtr fieldOp_;

      /// Presence map bits used by this instruction/opererator combination
      /// only valid after finalize has been called
      size_t presenceMapBitsUsed_;

      /// True if the field described by this instruction MUST appear in the application message.
      bool mandatory_;
      /// True if overflows in the integer field should be ignored (settable via XML)
      bool ignoreOverflow_;
    };

    ///////////////////////////////
    // Template method definitions

    template<typename IntType>
    void
    FieldInstruction::decodeSignedInteger(
      Codecs::DataSource & source,
      Codecs::Context & context,
      IntType & value,
      const std::string & name,
      bool oversize,
      bool ignoreOverflow)
    {
      PROFILE_POINT("decodeSignedInteger");
      uchar byte = 0;
      if(!source.getByte(byte))
      {
        context.reportFatal("[ERR U03]", "Unexpected end of data decoding signedinteger", name);
      }

      value = 0;
      // extend the sign bit
      if((byte & signBit) != 0)
      {
        value = IntType(-1);
      }
      // Assume an 8 bit byte;
      // Check the seven data bits and the sign bit to make sure no significant
      // information is lost.
      size_t shift = sizeof(IntType) * byteSize - (dataShift + 1);
      IntType overflowMask(IntType(-1) << shift);
      IntType overflowCheck(value << shift);
      // if we're allowing oversize values ignore a sign bit overflow
      if(oversize)
      {
        overflowMask <<= 1;
        overflowCheck <<= 1;
      }
      while((byte & stopBit) == 0)
      {
        if(!ignoreOverflow && (value & overflowMask) != overflowCheck)
        {
          context.reportError("[ERR D2]", "Integer Field overflow (signed).", name);
        }
        value <<= dataShift;
        value |= byte;
        if(!source.getByte(byte))
        {
          context.reportFatal("[ERR D2]", "Unexpected EOF in signed integer field.", name);
        }
      }
      // include the last byte (the one with the stop bit)
      if(!ignoreOverflow && (value & overflowMask) != overflowCheck)
      {
        context.reportError("[ERR D2]", "Signed Integer Field overflow.", name);
      }
      value <<= dataShift;
      value |= (byte & dataBits);
    }

    template<typename IntType>
    bool
    FieldInstruction::checkNullInteger(IntType & value)
    {
      bool isNull = (value == 0);
      if(value > 0)
      {
        value -= 1;
      }
      return isNull;
    }

#define INTEGER_SPECIALIZATION_NEEDS_DEBUGGING
#ifdef INTEGER_SPECIALIZATION
#define SIGNEDINTEGERSPECIALIZATION_NEEDS_DEBUGGING
#ifdef SIGNEDINTEGERSPECIALIZATION
    template<>
    inline
    void
    FieldInstruction::decodeSignedInteger(
      Codecs::DataSource & source,
      Codecs::Context & context,
      int32 & value,
      const std::string & name,
      bool oversize,
      bool ignoreOverflow)
    {
      const uchar * buffer;
      if(source.hasContiguous((32+6)/7, buffer))
      {
        int32 result = 0;
        const uchar * start = buffer;
        PROFILE_POINT("decodeSignedInteger");
        uchar byte = *buffer++; // byte 0
        // extend the sign bit
        if((byte & signBit) != 0)
        {
          result = int32(-1);
        }
        if(0 == (byte & stopBit))
        {
          result <<= dataShift;
          result |= byte;
          byte = *buffer++; // byte 1
          if(0 == (byte & stopBit))
          {
            result <<= dataShift;
            result |= byte;
            byte = *buffer++; // byte 2
            if(0 == (byte & stopBit))
            {
              result <<= dataShift;
              result |= byte;
              byte = *buffer++; // byte 3
              if(0 == (byte & stopBit))
              {
                result <<= dataShift;
                result |= byte;
                byte = *buffer++; // byte 4
                if(0 == (byte & stopBit))
                {
                  result <<= dataShift;
                  result |= byte;
                  byte = *buffer++; // byte 5
                  if(0 == (byte & stopBit))
                  {
                    if(!ignoreOverflow)
                    {
                      context.reportFatal("[ERR D2]", "Overflow in signed 32 bit integer field.", name);
                    }
                    value = uint32(-1); // return implausable value
                    source.skipContiguous(buffer - start);
                    unsigned char trash = 0;
                    while(0 == (trash & stopBit))
                    {
                      if(!source.getByte(trash))
                      {
                        context.reportFatal("[ERR D2]", "Unexpected EOF in signed 32 bit integer field.", name);
                      }
                    } // overflow
                    return;
                  } // byte 5
                } // byte4
              } // byte 3
            } // byte 2
          } // byte 1
        } // byte 0
        result <<= dataShift;
        result |= (byte & dataBits);
        value = result;
        source.skipContiguous(buffer - start);
        return;
      }
      else // contiguous data not available
      {
        PROFILE_POINT("decodeSignedInteger");
        uchar byte = 0;
        if(!source.getByte(byte))
        {
          context.reportFatal("[ERR U03]", "Unexpected end of data decoding signedinteger", name);
        }

        int32 result = 0;
        // extend the sign bit
        if((byte & signBit) != 0)
        {
          result = int32(-1);
        }
        // Assume an 8 bit byte;
        // Check the seven data bits and the sign bit to make sure no significant
        // information is lost.
        size_t shift = sizeof(int32) * byteSize - (dataShift + 1);
        int32 overflowMask(int32(-1) << shift);
        int32 overflowCheck(result << shift);
        // if we're allowing oversize results ignore a sign bit overflow
        if(oversize)
        {
          overflowMask <<= 1;
          overflowCheck <<= 1;
        }
        while((byte & stopBit) == 0)
        {
          if(!ignoreOverflow && (result & overflowMask) != overflowCheck)
          {
            context.reportError("[ERR D2]", "Integer Field overflow (signed).", name);
          }
          result <<= dataShift;
          result |= byte;
          if(!source.getByte(byte))
          {
            context.reportFatal("[ERR D2]", "Unexpected EOF in signed integer field.", name);
          }
        }
        // include the last byte (the one with the stop bit)
        if(!ignoreOverflow && (result & overflowMask) != overflowCheck)
        {
          context.reportError("[ERR D2]", "Signed Integer Field overflow.", name);
        }
        result <<= dataShift;
        result |= (byte & dataBits);
      }
    }
#endif // SIGNEDINTEGERSPECIALIZATION_NEEDS_DEBUGGING

    // Specialize for unsigned 32 bit inteeger
    template<>
    inline
    void
    FieldInstruction::decodeUnsignedInteger(
      Codecs::DataSource & source,
      Codecs::Context & context,
      uint32 & value,
      const std::string & name,
      bool ignoreOverflow)
    {
      uint32 result = 0;
      const uchar *buffer = 0;
      if(source.hasContiguous((32+6)/7, buffer))
      {
        const uchar * start = buffer;
        uchar byte = *buffer++;   //byte 0
        if(0 == (byte & stopBit))
        {
          result = byte;
          byte = *buffer++;       // byte 1
          if(0 == (byte & stopBit))
          {
            result <<= dataShift;
            result |= byte;
            byte = *buffer++;     // byte 2
            if(0 == (byte & stopBit))
            {
              result <<= dataShift;
              result |= byte;
              byte = *buffer++;   // byte 3
              if(0 == (byte & stopBit))
              {
                result <<= dataShift;
                result |= byte;
                byte = *buffer++;   // byte 4
                if(0 == (byte & stopBit))
                {
                  result <<= dataShift;
                  result |= byte;
                  byte = *buffer++;     // byte 5
                  if(0 == (byte & stopBit))
                  {
                    if(!ignoreOverflow)
                    {
                      context.reportFatal("[ERR D2]", "Overflow in unsigned 32 bit integer field.", name);
                    }
                    value = uint32(-1); // return implausable value
                    source.skipContiguous(buffer - start);
                    unsigned char trash = 0;
                    while(0 == (trash & stopBit))
                    {
                      if(!source.getByte(trash))
                      {
                        context.reportFatal("[ERR D2]", "Unexpected EOF in integer field.", name);
                      }
                    } // overflow
                    return;
                  } // byte 5
                } // byte4
              } // byte3
            } // byte2
          } // byte1
        } // byte0
        result <<= dataShift;
        result |= (byte & dataBits);
        source.skipContiguous(buffer - start);
        value = result;
      }
      else
      {
        uchar byte = 0;
        if(!source.getByte(byte)) // byte 0
        {
          context.reportFatal("[ERR D2]", "Unexpected EOF in integer field.", name);
        }
        if(0 == (byte & stopBit))
        {
          result <<= dataShift;
          result |= byte;
          if(!source.getByte(byte)) // byte 1
          {
            context.reportFatal("[ERR D2]", "Unexpected EOF in integer field.", name);
          }
          if(0 == (byte & stopBit))
          {
            result <<= dataShift;
            result |= byte;
            if(!source.getByte(byte)) // byte 2
            {
              context.reportFatal("[ERR D2]", "Unexpected EOF in integer field.", name);
            }
            if(0 == (byte & stopBit))
            {
              result <<= dataShift;
              result |= byte;
              if(!source.getByte(byte)) // byte 3
              {
                context.reportFatal("[ERR D2]", "Unexpected EOF in integer field.", name);
              }
              if(0 == (byte & stopBit))
              {
                result <<= dataShift;
                result |= byte;
                if(!source.getByte(byte)) // byte 4
                {
                  context.reportFatal("[ERR D2]", "Unexpected EOF in integer field.", name);
                }
                if(0 == (byte & stopBit))
                {
                  result <<= dataShift;
                  result |= byte;
                  if(!source.getByte(byte)) // byte 5
                  {
                    context.reportFatal("[ERR D2]", "Unexpected EOF in integer field.", name);
                  }
                  if(0 == (byte & stopBit)) // overflow
                  {
                    if(!ignoreOverflow)
                    {
                      context.reportFatal("[ERR D2]", "Overflow in unsigned 32 bit integer field.", name);
                    }
                    unsigned char trash = 0;
                    while(0 == (trash & stopBit))
                    {
                      if(!source.getByte(trash))
                      {
                        context.reportFatal("[ERR D2]", "Unexpected EOF in integer field.", name);
                      }
                    } // overflow
                  } // byte 5
                } // byte 4
              } // byte 3
            } // byte 2
          } // byte 1
        } // byte 0
        result <<= dataShift;
        result |= (byte & dataBits);
        value = result;
      }
    }
#endif // INTEGER_SPECIALIZATION

    template<typename UnsignedIntType>
    void
    FieldInstruction::decodeUnsignedInteger(
      Codecs::DataSource & source,
      Codecs::Context & context,
      UnsignedIntType & value,
      const std::string & name,
      bool ignoreOverflow)
    {
      PROFILE_POINT("decodeUnsignedInteger");
      uchar byte = 0;
      if(!source.getByte(byte))
      {
        context.reportFatal("[ERR U03]", "Unexpected end of data decoding unsigned integer", name);
      }

      value = 0;

      // Assume an 8 bit byte;
      // Check the seven data bitsbit to make sure no significant
      // information is lost.
      unsigned short shift = ((sizeof(UnsignedIntType) * byteSize) / dataShift) * dataShift;
      UnsignedIntType overflowMask(UnsignedIntType(-1) << shift);
      UnsignedIntType overflowCheck(value << shift);

      while((byte & stopBit) == 0)
      {
        if(!ignoreOverflow && (value & overflowMask) != overflowCheck)
        {
          context.reportError("[ERR D2]", "Unsigned Integer Field overflow...", name);
        }
        value <<= dataShift;
        value |= byte;
        if(!source.getByte(byte))
        {
          context.reportFatal("[ERR U03]", "End of file without stop bit decoding unsigned integer.", name);
        }
      }
      if(!ignoreOverflow && (value & overflowMask) != overflowCheck)
      {
        context.reportError("[ERR D2]", "Unsigned Integer Field overflow..", name);
      }
      value <<= dataShift;
      value |= (byte & dataBits);
    }
  }
}
#endif // FIELDINSTRUCTION_H
