// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#pragma message(__FILE__ "(6) : DataDestinationString is obsolete and will be removed from QuickFAST soon. Please use DataDestination::toString() instead.")
#endif
#ifndef DATADESTINATIONSTRING_H
#define DATADESTINATIONSTRING_H
#include <Codecs/DataDestination.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief A data destination that collects the output into a string.
    /// @deprecated  Use DataDestination and the toString() method
    class DataDestinationString : public DataDestination
    {
    public:
      /// @brief access the data written to the destination as a string
    /// @deprecated  Use DataDestination and the toString() method
      const std::string & getValue()const
      {
        toString(value_);
        return value_;
      }
    private:
      mutable std::string value_;
    };
  }
}
#endif // DATADESTINATIONSTRING_H
