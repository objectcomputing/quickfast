// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#include <Common/Decimal.h>

namespace QuickFAST
{
  namespace DotNet
  {
    /// @brief A .NET representation of a FAST encoded Decimal field
    public value struct DNDecimal
    {
      /// @brief the mantissa
      __int64  Mantissa;
      /// @brief the exponent (base 10)
      signed char Exponent;

      /// @brief constuct from separate mantissa and exponent
      DNDecimal(__int64 mantissa, signed char exponent):
        Mantissa(mantissa),
        Exponent(exponent)
      {
      }

      /// @brief Express the value as a double
      double ToDouble()
      {
        return (double)Mantissa * System::Math::Pow(10, Exponent);
      }

      /// @brief Express the value as a string
      virtual System::String^ ToString() override
      {
        return System::String::Format("{0}E{1}", this->Mantissa, this->Exponent);
      }

    internal:
      DNDecimal(const QuickFAST::Decimal& decimal):
        Mantissa(decimal.getMantissa()),
        Exponent(decimal.getExponent())
      {
      }
    };
  }
}
