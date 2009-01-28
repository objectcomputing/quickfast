// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef TYPES_H
#define TYPES_H

namespace QuickFAST{
  /// @brief Unsigned character. AKA: a byte
  typedef unsigned char uchar;
  /// @brief 32 bit signed integer
  typedef signed long int32;
  /// @brief 32 bit unsigned integer
  typedef unsigned long uint32;
  /// @brief 64 bit signed integer
  typedef signed long long int64;
  /// @brief 64 bit unsigned integer
  typedef unsigned long long uint64;
  /// @brief the mantissa portion of a Decimal
  typedef signed long mantissa_t;
  /// @brief the exponent portion of a Decimal
  typedef signed char exponent_t;
  class Decimal;
  /// @brief an alias for Decimal
  typedef Decimal decimal_t;
  /// @brief the type used to store a template Id.
  typedef uint32 template_id_t;
  /// @brief type type used to store a field Id.
  typedef std::string field_id_t;
}
#endif // TYPES_H
