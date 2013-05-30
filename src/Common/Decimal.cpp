// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "Decimal.h"
#include <Common/Exceptions.h>
#include <Common/WorkingBuffer.h>
#include <boost/algorithm/string/trim.hpp>

using namespace ::QuickFAST;

Decimal::Decimal(
    mantissa_t mantissa,
    exponent_t exponent,
    bool autoNormalize)
: mantissa_(mantissa)
, exponent_(exponent)
, autoNormalize_(autoNormalize)
{
}

Decimal::Decimal(const Decimal & rhs)
: mantissa_(rhs.mantissa_)
, exponent_(rhs.exponent_)
, autoNormalize_(rhs.autoNormalize_)
{
}

Decimal::~Decimal()
{
}

void
Decimal::parse(const std::string & value)
{
  std::string str = value;
  boost::algorithm::trim(str);
  size_t dotPos = str.find(".");
  std::string wholeString = str.substr(0,dotPos);
  std::string fracString;
  if(dotPos != std::string::npos)
  {
    fracString = str.substr(dotPos+1);
  }
  std::string mantissaString = wholeString+fracString;
  exponent_ = -exponent_t(fracString.size());

  bool overflow = false;

  // VC8 stringstream is truncating a large string instead of throwing
  // the exception, so we'll always fall back on the overflow code.
#if defined _MSC_VER && _MSC_VER < 1500
  overflow = true;
#else
  try {
    mantissa_ = boost::lexical_cast<mantissa_t>(mantissaString);
  }
  catch (std::exception &)
  {
    overflow = true;
  }
#endif

  if (overflow && autoNormalize_)
  {
    size_t pos = mantissaString.find_last_not_of ("0");
    exponent_ += exponent_t(mantissaString.length () - pos - 1);
    mantissaString = mantissaString.substr (0, pos + 1);
    mantissa_ = boost::lexical_cast<mantissa_t>(mantissaString);
  }

  if(autoNormalize_)
  {
    normalize();
  }
}

void
Decimal::setAutoNormalize(bool autoNormalize)
{
  autoNormalize_ = autoNormalize;
  if(autoNormalize_)
  {
    normalize();
  }
}

void
Decimal::setMantissa(mantissa_t mantissa)
{
  mantissa_ = mantissa;
}

void
Decimal::setExponent(exponent_t exponent)
{
  exponent_ = exponent;
}



mantissa_t
Decimal::getMantissa() const
{
  return mantissa_;
}

exponent_t
Decimal::getExponent() const
{
  return exponent_;
}

void
Decimal::toString(std::string & value)const
{
#if 0
  value = boost::lexical_cast<std::string>(double(*this));
#elif 1
  std::stringstream str;
  str << double(*this);
  value = str.str();
#else
  WorkingBuffer buffer;
  buffer.clear(true, 20);
  bool negative = false;
  int64 m = mantissa_;
  if(m < 0)
  {
    negative = true;
    m = -m;
  }
  short e = exponent_;
  if(e >= 0)
  {
    // No trailing decimal point
    // buffer.push((unsigned char)'.');
    while(e > 0)
    {
      buffer.push((unsigned char)'0');
      e -= 1;
    }
  }
  bool none = true;
  while(m != 0 || e < 0 || none)
  {
    none = false;
    char c = (m % 10) + '0';
    m /= 10;
    buffer.push((unsigned char)c);
    if(e < 0)
    {
      e += 1;
      if(e == 0)
      {
        buffer.push((unsigned char)'.');
        // insure at least one character to the left of the decimal point
        if(m == 0)
        {
          buffer.push((unsigned char)'0');
        }
      }
    }
  }
  if(negative)
  {
    buffer.push((unsigned char)'-');
  }
  value.assign((const char *)buffer.begin(), buffer.end()-buffer.begin());
#endif
}

Decimal &
Decimal::operator = (const Decimal & rhs)
{
  Decimal temp(rhs);
  swap(temp);
  return *this;
}

void
Decimal::swap(Decimal & rhs)
{
  exponent_t texp = exponent_;
  exponent_ = rhs.exponent_;
  rhs.exponent_ = texp;
  mantissa_t tman = mantissa_;
  mantissa_ = rhs.mantissa_;
  rhs.mantissa_ = tman;
}

void
Decimal::normalize(bool strict /*= true*/)
{
  while(mantissa_ != 0 && mantissa_ % 10 == 0 && exponent_ < 64)
  {
    mantissa_ /= 10;
    exponent_ += 1;
  }
  if(exponent_ > 64)
  {
    if(strict)
    {
      throw OverflowError("[ERR R1]Decimal Exponent overflow.");
    }
    while(exponent_ > 64)
    {
      mantissa_ *= 10;
      exponent_ -= 1;
    }
  }
  if(exponent_ < -64)
  {
    if(strict)
    {
      throw OverflowError("[ERR R1]Decimal Exponent undeflow.");
    }
    while(exponent_ < -64)
    {
      mantissa_ /= 10;
      exponent_ += 1;
    }
  }
}

void
Decimal::denormalize(exponent_t exponent)
{
  while(exponent_ > exponent && mantissa_ < (LLONG_MAX/10))
  {
    exponent_ -= 1;
    mantissa_ *= 10;
    // todo check mantissa overflow
  }
}
void
Decimal::maximizeMantissa()
{
  // this could be considerably faster!
  while(exponent_ > SCHAR_MIN && mantissa_ < (LLONG_MAX/10))
  {
    exponent_ -= 1;
    mantissa_ *= 10;
  }
}

bool
Decimal::operator<(const Decimal & rhs) const
{
  if(exponent_ == rhs.exponent_)
  {
    return mantissa_ < rhs.mantissa_;
  }

  if(rhs.exponent_ < exponent_)
  {
    Decimal temp(*this);
    temp.denormalize(rhs.exponent_);
    return temp.mantissa_ < rhs.mantissa_;
  }
  else
  {
    Decimal temp(rhs);
    temp.denormalize(exponent_);
    return mantissa_ < temp.mantissa_;
  }
//  // at this point comparison is iffy, converting to double doesn't
//  // hurt
//  return (double) *this < (double)rhs;

}

bool
Decimal::operator==(const Decimal & rhs) const
{
  if(exponent_ == rhs.exponent_)
  {
    return mantissa_ == rhs.mantissa_;
  }

  if(rhs.exponent_ < exponent_)
  {
    Decimal temp(*this);
    temp.denormalize(rhs.exponent_);
    return temp.mantissa_ == rhs.mantissa_;
  }
  else
  {
    Decimal temp(rhs);
    temp.denormalize(exponent_);
    return mantissa_== temp.mantissa_;
  }
}

Decimal::operator double()const
{
  // faster than the pow function or repeated multiply/divides
  static double powerTable[] =
  {
      1.0e-63, 1.0e-62, 1.0e-61, 1.0e-60, 1.0e-59, 1.0e-58, 1.0e-57, 1.0e-56,
      1.0e-55, 1.0e-54, 1.0e-53, 1.0e-52, 1.0e-51, 1.0e-50, 1.0e-49, 1.0e-48,
      1.0e-47, 1.0e-46, 1.0e-45, 1.0e-44, 1.0e-43, 1.0e-42, 1.0e-41, 1.0e-40,
      1.0e-39, 1.0e-38, 1.0e-37, 1.0e-36, 1.0e-35, 1.0e-34, 1.0e-33, 1.0e-32,
      1.0e-31, 1.0e-30, 1.0e-29, 1.0e-28, 1.0e-27, 1.0e-26, 1.0e-25, 1.0e-24,
      1.0e-23, 1.0e-22, 1.0e-21, 1.0e-20, 1.0e-19, 1.0e-18, 1.0e-17, 1.0e-16,
      1.0e-15, 1.0e-14, 1.0e-13, 1.0e-12, 1.0e-11, 1.0e-10, 1.0e-9 , 1.0e-8 ,
      1.0e-7 , 1.0e-6 , 1.0e-5 , 1.0e-4 , 1.0e-3 , 1.0e-2 , 1.0e-1 , 1.0e0  ,
      1.0e1  , 1.0e2  , 1.0e3  , 1.0e4  , 1.0e5  , 1.0e6  , 1.0e7  , 1.0e8  ,
      1.0e9  , 1.0e10 , 1.0e11 , 1.0e12 , 1.0e13 , 1.0e14 , 1.0e15 , 1.0e16 ,
      1.0e17 , 1.0e18 , 1.0e19 , 1.0e20 , 1.0e21 , 1.0e22 , 1.0e23 , 1.0e24 ,
      1.0e25 , 1.0e26 , 1.0e27 , 1.0e28 , 1.0e29 , 1.0e30 , 1.0e31 , 1.0e32 ,
      1.0e33 , 1.0e34 , 1.0e35 , 1.0e36 , 1.0e37 , 1.0e38 , 1.0e39 , 1.0e40 ,
      1.0e41 , 1.0e42 , 1.0e43 , 1.0e44 , 1.0e45 , 1.0e46 , 1.0e47 , 1.0e48 ,
      1.0e49 , 1.0e50 , 1.0e51 , 1.0e52 , 1.0e53 , 1.0e54 , 1.0e55 , 1.0e56 ,
      1.0e57 , 1.0e58 , 1.0e59 , 1.0e60 , 1.0e61 , 1.0e62 , 1.0e63
  };
  if(exponent_ >= -63 && exponent_ <= 63)
  {
    return (double)mantissa_ * powerTable[exponent_ + 63];
  }
  // note it is a FAST reportable error if abs(exponent) > 63, but for now we'll just fake it here.
  return double(double(mantissa_) * pow(10.0L, exponent_));
}

Decimal &
Decimal::operator+=(const Decimal & rhs)
{
  if(rhs.exponent_ < exponent_)
  {
    Decimal temp(*this);
    temp.denormalize(rhs.exponent_);
    temp.mantissa_ += rhs.mantissa_;
    temp.normalize();
    swap(temp);
  }
  else
  {
    Decimal temp(rhs);
    temp.denormalize(exponent_);
    temp.mantissa_ += mantissa_;
    temp.normalize();
    swap(temp);
  }
  return *this;
}


Decimal &
Decimal::operator-=(const Decimal & rhs)
{
  if(rhs.exponent_ < exponent_)
  {
    Decimal temp(*this);
    temp.denormalize(rhs.exponent_);
    temp.mantissa_ -= rhs.mantissa_;
    temp.normalize();
    swap(temp);
  }
  else
  {
    Decimal temp(rhs);
    temp.denormalize(exponent_);
    temp.mantissa_ = mantissa_ - temp.mantissa_;
    temp.normalize();
    swap(temp);
  }
  return *this;
}

Decimal &
Decimal::operator*=(const Decimal & rhs)
{
  Decimal temp(*this);
  temp.exponent_ += rhs.exponent_;
  temp.mantissa_ *= rhs.mantissa_;
  temp.normalize();
  swap(temp);
  return *this;
}

Decimal&
Decimal::operator/=(const Decimal & rhs)
{
  Decimal temp(*this);
  temp.maximizeMantissa();
  temp.exponent_ -= rhs.exponent_;
  temp.mantissa_ /= rhs.mantissa_;
  temp.normalize(false);
  swap(temp);
  return *this;
}
