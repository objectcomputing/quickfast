// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
# pragma once

#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>

namespace QuickFAST{
  /// @brief A  BitMap data type.  Not in the FAST standard.
  ///
  /// BitMaps are used in ARCA to represent packed data fields.
  ///
  class QuickFAST_Export BitMap
  {
  public:
    /// @brief Construct a BitMap, defaulting to 0.0 autonormalized
    BitMap(const uchar * buffer, size_t length);
    /// @brief Copy construct a BitMap
    BitMap(const BitMap & rhs);
    /// @brief Construct an empty bitmap
    BitMap();

    /// @brief Destruct a BitMap
    ~BitMap();

    /// @brief Convert the value to a string
    void toString(std::string & value)const;

    /// @brief Assignment
    BitMap & operator=(const BitMap & rhs);

    /// @brief Nothrow; no allocate; constant time swap values.
    void swap(BitMap & rhs);

  private:
    std::string value_;
  };
}
