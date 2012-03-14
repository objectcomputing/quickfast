// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef VALUE_H
#define VALUE_H
#include "Value_fwd.h"
#include <Common/Types.h>
#include <Common/Exceptions.h>
#include <Common/Decimal.h>
#include <Common/StringBuffer.h>
namespace QuickFAST{

  /// @brief A container for several different types of values
  ///
  class Value
  {
  public:
    /// @brief Enumerate the types of values (orable)
    enum ValueClass
    {
      SIGNEDINTEGER = 2,
      UNSIGNEDINTEGER = 4,
      DECIMAL = 8,
      STRING = 16,
      COMPOUND = 32,  // No data is stored for compound value types
      EMPTY = 64,
      UNDEFINED = 1
    };

    Value()
      : class_(UNDEFINED)
      , cachedString_(false)
      , unsignedInteger_(0)
      , signedInteger_(0)
      , exponent_(0)
    {
    }

    /// @brief a typical destructor.
    ~Value()
    {
    }

    /// @brief Set Compound type
    void setCompound()
    {
      class_ = COMPOUND;
      cachedString_ = false;
    }

    /// @brief reset the value class
    /// @param undefined defaults to UNDEFINED but can be used to set other classes for testing
    ///
    void setUndefined(ValueClass undefined = UNDEFINED)
    {
      class_ = undefined;
      cachedString_ = false;
    }

    /// @brief set the value to NULL
    void setNull()
    {
      class_ = EMPTY;
      cachedString_ = false;
    }

    /// @brief check for NULL value
    bool isNull() const
    {
      return ((class_ & EMPTY) == EMPTY);
    }

    /// @brief set the value to undefined
    void erase()
    {
      class_ = UNDEFINED;
      cachedString_ = false;
      string_.erase();
      signedInteger_ = 0;
      unsignedInteger_ = 0;
      exponent_ = 0;
    }

    /// @brief assign a value
    /// @param value is the value to be assigned
    void setValue(const int64 value)
    {
      class_ = SIGNEDINTEGER;
      cachedString_ = false;
      signedInteger_ = value;
    }

    /// @brief assign a value
    /// @param value is the value to be assigned
    void setValue(const uint64 value)
    {
      class_ = UNSIGNEDINTEGER;
      cachedString_ = false;
      unsignedInteger_ = value;
    }

    /// @brief assign a value
    /// @param value is the value to be assigned
    void setValue(const int32 value)
    {
      class_ = SIGNEDINTEGER;
      cachedString_ = false;
      signedInteger_ = value;
    }

    /// @brief assign a value
    /// @param value is the value to be assigned
    void setValue(const uint32 value)
    {
      class_ = UNSIGNEDINTEGER;
      cachedString_ = false;
      unsignedInteger_ = value;
    }

    /// @brief assign a value
    /// @param value is the value to be assigned
    void setValue(const int16 value)
    {
      class_ = SIGNEDINTEGER;
      cachedString_ = false;
      signedInteger_ = value;
    }

    /// @brief assign a value
    /// @param value is the value to be assigned
    void setValue(const uint16 value)
    {
      class_ = UNSIGNEDINTEGER;
      cachedString_ = false;
      unsignedInteger_ = value;
    }

    /// @brief assign a value
    /// @param value is the value to be assigned
    void setValue(const int8 value)
    {
      class_ = SIGNEDINTEGER;
      cachedString_ = false;
      signedInteger_ = value;
    }

    /// @brief assign a value
    /// @param value is the value to be assigned
    void setValue(const uchar value)
    {
      class_ = UNSIGNEDINTEGER;
      cachedString_ = false;
      unsignedInteger_ = value;
    }

    /// @brief assign a value
    /// @param value is the value to be assigned
    void setValue(const Decimal& value)
    {
      class_ = DECIMAL;
      cachedString_ = false;
      exponent_ = value.getExponent();
      signedInteger_ = value.getMantissa();
    }

    /// @brief assign a value
    /// @param value is the value to be assigned
    /// @param length is the lenght of the string pointed to by value.
    void setValue(const unsigned char * value, size_t length)
    {
      class_ = STRING;
      cachedString_ = true;
      string_.assign(value, length);
    }

    /// @brief assign a value from a null terminated C-style string
    ///
    /// @param value is the value to be assigned
    void setValue(const char * value)
    {
      setValue(reinterpret_cast<const unsigned char*>(value), std::strlen(value));
    }

    /// @brief assign a value
    /// @param value is the value to be assigned
    void setValue(const std::string& value)
    {
      setValue(reinterpret_cast<const unsigned char*>(value.c_str()), value.length());
    }

    /// @brief check for class and value equality
    bool operator == (const Value & rhs) const
    {
      if(((class_ | rhs.class_) & UNDEFINED) == UNDEFINED)
      {
        return false;
      }
      if(class_ != rhs.class_)
      {
        return false;
      }
      if(unsignedInteger_ != rhs.unsignedInteger_ ||
        signedInteger_ != rhs.signedInteger_ ||
        exponent_ != rhs.exponent_)
      {
        return false;
      }
      if((class_ & STRING) == STRING)
      {
        return string_ == rhs.string_;
      }
      return true;
    }

    /// @brief inequality operator
    bool operator != (const Value & rhs)const
    {
      return ! (*this == rhs);
    }

    /// @brief display the value as a string.  Low performance
    const StringBuffer & displayString() const
    {
      if(isDefined() && !cachedString_)
      {
        valueToStringBuffer();
      }
      return string_;
    }

    /// @brief Does this field have a value?
    /// @return true if the field has a value.
    bool isDefined() const
    {
      return (class_ & UNDEFINED) == 0;
    }

    /// @brief Is this field a kind of string (Ascii, Utf8, or ByteVector)?
    bool isString()const
    {
      return (class_ & (UNDEFINED | STRING)) == STRING;
    }

    /// @brief true if signed integer
    bool isSignedInteger() const
    {
      return (class_ & (UNDEFINED | SIGNEDINTEGER)) == SIGNEDINTEGER;
    }

    /// @brief true if unsigned integer
    bool isUnsignedInteger() const
    {
      return (class_ & (UNDEFINED | UNSIGNEDINTEGER)) == UNSIGNEDINTEGER;
    }

    /// @brief true if numberic (integer, unsigned integer or decimal)
    bool isNumeric() const
    {
      return (class_ & (UNDEFINED | STRING | COMPOUND)) == 0;
    }

    /// @brief true if compound value
    bool isCompound() const
    {
      return (class_ & (UNDEFINED | COMPOUND)) == COMPOUND;
    }

    /// @brief get the value
    /// @param value receives the data
    bool getValue(int64 & value) const
    {
      if(class_ == SIGNEDINTEGER)
      {
        value = signedInteger_;
        return true;
      }
      return false;
    }

    /// @brief get the value
    /// @param value receives the data
    bool getValue(uint64 & value) const
    {
      if(class_ == UNSIGNEDINTEGER)
      {
        value = unsignedInteger_;
        return true;
      }
      return false;
    }

    /// @brief get the value
    /// @param value receives the data
    bool getValue(int32 & value) const
    {
      if(class_ == SIGNEDINTEGER)
      {
        value = static_cast<int32>(signedInteger_);
        return true;
      }
      return false;
    }

    /// @brief get the value
    /// @param value receives the data
    bool getValue(uint32 & value) const
    {
      if(class_ == UNSIGNEDINTEGER)
      {
        value = static_cast<uint32>(unsignedInteger_);
        return true;
      }
      return false;
    }

    /// @brief get the value
    /// @param value receives the data
    bool getValue(int16 & value) const
    {
      if(class_ == SIGNEDINTEGER)
      {
        value = static_cast<int16>(signedInteger_);
        return true;
      }
      return false;
    }

    /// @brief get the value
    /// @param value receives the data
    bool getValue(uint16 & value) const
    {
      if(class_ == UNSIGNEDINTEGER)
      {
        value = static_cast<uint16>(unsignedInteger_);
        return true;
      }
      return false;
    }

    /// @brief get the value
    /// @param value receives the data
    bool getValue(int8 & value) const
    {
      if(class_ == SIGNEDINTEGER)
      {
        value = static_cast<int8>(signedInteger_);
        return true;
      }
      return false;
    }

    /// @brief get the value
    /// @param value receives the data
    bool getValue(uchar & value) const
    {
      if(class_ == UNSIGNEDINTEGER)
      {
        value = static_cast<uchar>(unsignedInteger_);
        return true;
      }
      return false;
    }

    /// @brief get the value
    /// @param value receives the data
    bool getValue(Decimal & value) const
    {
      if(class_ == DECIMAL)
      {
        value = Decimal(signedInteger_, exponent_);
        return true;
      }
      return false;

    }

    /// @brief get the value
    /// @param value is set to point to the string
    /// @param length is the length of the string.
    bool getValue(const unsigned char *& value, size_t &length) const
    {
      if(class_ == STRING)
      {
        value = string_.data();
        length = string_.size();
        return true;
      }
      return false;
    }

    /// @brief get the value
    /// @param value is set to point to the string
    bool getValue(const char *& value) const
    {
      if(class_ == STRING)
      {
        value = reinterpret_cast<const char *>(string_.c_str());
        return true;
      }
      return false;
    }

    /// @brief get the value
    /// @param value receives the data
    bool getValue(std::string& value) const
    {
      if(class_ == STRING)
      {
        value = static_cast<std::string>(string_);
        return true;
      }
      return false;
    }

    /// @brief get the value
    /// @returns the value
    uint64 getUnsignedInteger()const
    {
      if(class_ != UNSIGNEDINTEGER)
      {
        UnsupportedConversion ex("Value is not unsigned integer.");
        throw ex;
      }
      return unsignedInteger_;
    }

    /// @brief get the value
    /// @returns the value
    int64 getSignedInteger()const
    {
      if(class_ != SIGNEDINTEGER)
      {
        UnsupportedConversion ex("Value is not signed integer.");
        throw ex;
      }
      return signedInteger_;
    }

    /// @brief get the value
    /// @returns the value
    int64 getMantissa()const
    {
      if(class_ != DECIMAL)
      {
        UnsupportedConversion ex("Value is not decimal.");
        throw ex;
      }
      return signedInteger_;
    }

    /// @brief get the value
    /// @returns the value
    exponent_t getExponent()const
    {
      if(class_ != DECIMAL)
      {
        UnsupportedConversion ex("Value is not decimal.");
        throw ex;
      }
      return exponent_;
    }

    /// @brief get the value
    /// @returns the value
    Decimal getDecimal()const
    {
      if(class_ != DECIMAL)
      {
        UnsupportedConversion ex("Value is not decimal.");
        throw ex;
      }
      return Decimal(signedInteger_, exponent_);
    }

  private:
    /// @brief Produce a cached "human readable" representation of the value
    void valueToStringBuffer()const
    {
      if(cachedString_ || (class_ == STRING))
      {
        return;
      }
      std::stringstream buffer;
      if(class_ == SIGNEDINTEGER)
      {
        buffer << signedInteger_;
      }
      else if(class_ == UNSIGNEDINTEGER)
      {
        buffer << unsignedInteger_;
      }
      else if(class_ == DECIMAL)
      {
        Decimal d(signedInteger_, exponent_);

        buffer << static_cast<double>(d);
      }
      else if(class_ == EMPTY)
      {
        buffer << "[null]";
      }
      string_ = buffer.str();
    }


  private:
    /// What class of information is this Value
    /// Bits are ORed.
    ValueClass class_;

    /// @brief true if string representation of the value has been cached
    mutable bool cachedString_;

    ///////////////////////////////////////////////////
    // Value contents

    ///@brief Data for any of the unsigned integral types.
    unsigned long long unsignedInteger_;

    ///@brief Data for any of the signed integral types. Also Decimal mantissa.
    signed long long signedInteger_;

    ///@brief Exponent for Decimal types (mantissa is in signedInteger_)
    exponent_t exponent_;

    ///@brief Buffer containing string value. Owned by this object
    mutable StringBuffer string_;
  };
}

#endif // VALUE_H
