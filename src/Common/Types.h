// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef TYPES_H
#define TYPES_H

#include <Common/QuickFAST_Export.h>
#include <Common/Decimal_fwd.h>

typedef std::streamsize streamsize_t;

namespace QuickFAST{
  /// @brief Unsigned character. AKA: a byte
  typedef unsigned char uchar;
  /// @brief Signed 8 bit integer. AKA: char on some, but not all platforms.
  typedef boost::int8_t int8;
  /// @brief 16 bit signed integer
  typedef boost::int16_t int16;
  /// @brief 16 bit unsigned integer
  typedef boost::uint16_t uint16;
  /// @brief 32 bit signed integer
  typedef boost::int32_t int32;
  /// @brief 32 bit unsigned integer
  typedef boost::uint32_t uint32;
  /// @brief 64 bit signed integer
  typedef boost::int64_t int64;
  /// @brief 64 bit unsigned integer
  typedef boost::uint64_t uint64;
  /// @brief the mantissa portion of a Decimal
  typedef int64 mantissa_t;
  /// @brief the exponent portion of a Decimal
  typedef int8 exponent_t;

  /// @brief an alias for Decimal
  typedef Decimal decimal_t;
  /// @brief the type used to store a template Id.
  typedef uint32 template_id_t;
  /// @brief type used to store a field Id.
  typedef std::string field_id_t;

  /// @brief type used for packet sequence numbers during arbitrage
  typedef uint32 sequence_t;

  /// @brief a "namespace" for value types and the method to interpret them.
  class QuickFAST_Export ValueType
  {
  public:
    /// @brief Identify the data type
    ///
    /// Most of these values correspond directly to the types of
    /// fields defined in
    /// the FAST standard.
    enum Type
    {
      INT8,     // not FAST Standard
      UINT8,    // not FAST Standard
      INT16,    // not FAST Standard
      UINT16,   // not FAST Standard
      INT32,
      UINT32,
      INT64,
      UINT64,
      DECIMAL,
      EXPONENT,
      MANTISSA,
      ASCII,
      UTF8,
      BYTEVECTOR,
      BITMAP,   // not FAST Standard (and not properly supported, yet)
      SEQUENCE,
      LENGTH, // Synonym for INT32 when used as a length field
      GROUP,
      TEMPLATEREF,
      TYPEREF,
      UNDEFINED
    };

    /// @brief translate a ValueType::Type to a string
    /// @param type is the type to be interpreted.
    /// @return is the type as a string that matches the XML element name for the corresponding field instruction.
    static const std::string & typeName(Type type);
  };
}
#endif // TYPES_H
