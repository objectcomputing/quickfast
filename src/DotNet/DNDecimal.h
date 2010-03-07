// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#include <Common/Decimal.h>

namespace QuickFAST
{
  namespace DotNet
  {
    public value struct DNDecimal
    {
      __int64  Mantissa;
      signed char Exponent;

      DNDecimal(__int64 mantissa, signed char exponent):
        Mantissa(mantissa),
        Exponent(exponent)
      {
      }

      double ToDouble()
      {
        return (double)Mantissa * System::Math::Pow(10, Exponent);
      }

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
