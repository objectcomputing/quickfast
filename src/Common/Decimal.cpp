// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "Decimal.h"
#include <Common/Exceptions.h>
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

  try {
    mantissa_ = boost::lexical_cast<mantissa_t>(mantissaString);
  }
  catch (std::exception &)
  {
    overflow = true;
  }

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
  value = boost::lexical_cast<std::string>(double(*this));
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
Decimal::normalize()
{
  while(mantissa_ != 0 && mantissa_ % 10 == 0 && exponent_ < 64)
  {
    mantissa_ /= 10;
    exponent_ += 1;
  }
  if(exponent_ > 64)
  {
    throw OverflowError("[ERR R1]Decimal Exponent overflow.");
  }
  if(exponent_ < -64)
  {
    throw OverflowError("[ERR R1]Decimal Exponent undeflow.");
  }
}

void
Decimal::denormalize(exponent_t exponent)
{
  while(exponent_ > exponent)
  {
    exponent_ -= 1;
    mantissa_ *= 10;
    // todo check mantissa overflow
  }
}

bool
Decimal::operator<(const Decimal & rhs) const
{
  if(exponent_ == rhs.exponent_)
  {
    return mantissa_ < rhs.mantissa_;
  }
  // at this point comparison is iffy, converting to double doesn't
  // hurt
  return (double) *this == (double)rhs;

}

bool
Decimal::operator==(const Decimal & rhs) const
{
  if(exponent_ == rhs.exponent_)
  {
    return mantissa_ == rhs.mantissa_;
  }
  // at this point equality is iffy, converting to double doesn't
  // hurt
  return (double) *this == (double)rhs;
}

Decimal::operator double()const
{
    return double(mantissa_) * pow(10.0L, exponent_);
}

Decimal &
Decimal::operator+=(const Decimal & rhs)
{
  if(rhs.exponent_ > exponent_)
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
  if(rhs.exponent_ > exponent_)
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
    temp.mantissa_ += mantissa_;
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
  temp.exponent_ -= rhs.exponent_;
  temp.mantissa_ /= rhs.mantissa_;
  temp.normalize();
  swap(temp);
  return *this;
}
