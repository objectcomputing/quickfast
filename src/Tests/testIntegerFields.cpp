// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>

#include <Messages/FieldInt32.h>
#include <Messages/FieldInt64.h>
#include <Messages/FieldUInt32.h>
#include <Messages/FieldUInt64.h>
#include <Common/Exceptions.h>

using namespace ::QuickFAST;

BOOST_AUTO_TEST_CASE(testFieldInt32)
{
  Messages::FieldCPtr field = Messages::FieldInt32::createNull();
  BOOST_CHECK(!field->isDefined());
  BOOST_CHECK(!field->isType(ValueType::ASCII));
  BOOST_CHECK(!field->isType(ValueType::BYTEVECTOR));
  BOOST_CHECK(!field->isType(ValueType::DECIMAL));
  BOOST_CHECK( field->isType(ValueType::INT32));
  BOOST_CHECK(!field->isType(ValueType::INT64));
  BOOST_CHECK(!field->isType(ValueType::UINT32));
  BOOST_CHECK(!field->isType(ValueType::UINT64));
  BOOST_CHECK(!field->isType(ValueType::UTF8));
  BOOST_CHECK(!field->isType(ValueType::SEQUENCE));

  field = Messages::FieldInt32::create(23);
  BOOST_CHECK(field->isDefined());
  BOOST_CHECK(!field->isType(ValueType::ASCII));
  BOOST_CHECK(!field->isType(ValueType::BYTEVECTOR));
  BOOST_CHECK(!field->isType(ValueType::DECIMAL));
  BOOST_CHECK( field->isType(ValueType::INT32));
  BOOST_CHECK(!field->isType(ValueType::INT64));
  BOOST_CHECK(!field->isType(ValueType::UINT32));
  BOOST_CHECK(!field->isType(ValueType::UINT64));
  BOOST_CHECK(!field->isType(ValueType::UTF8));
  BOOST_CHECK(!field->isType(ValueType::SEQUENCE));

  BOOST_CHECK_EQUAL(23, field->toInt32());

  BOOST_CHECK_THROW(field->toAscii(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUInt32(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toInt64(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUInt64(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toDecimal(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUtf8(),UnsupportedConversion);
}


BOOST_AUTO_TEST_CASE(testFieldInt64)
{
  Messages::FieldCPtr field = Messages::FieldInt64::createNull();
  BOOST_CHECK(!field->isDefined());
  BOOST_CHECK(!field->isType(ValueType::ASCII));
  BOOST_CHECK(!field->isType(ValueType::BYTEVECTOR));
  BOOST_CHECK(!field->isType(ValueType::DECIMAL));
  BOOST_CHECK(!field->isType(ValueType::INT32));
  BOOST_CHECK( field->isType(ValueType::INT64));
  BOOST_CHECK(!field->isType(ValueType::UINT32));
  BOOST_CHECK(!field->isType(ValueType::UINT64));
  BOOST_CHECK(!field->isType(ValueType::UTF8));
  BOOST_CHECK(!field->isType(ValueType::SEQUENCE));

  field = Messages::FieldInt64::create(4294967295L);
  BOOST_CHECK(field->isDefined());
  BOOST_CHECK(!field->isType(ValueType::ASCII));
  BOOST_CHECK(!field->isType(ValueType::BYTEVECTOR));
  BOOST_CHECK(!field->isType(ValueType::DECIMAL));
  BOOST_CHECK(!field->isType(ValueType::INT32));
  BOOST_CHECK( field->isType(ValueType::INT64));
  BOOST_CHECK(!field->isType(ValueType::UINT32));
  BOOST_CHECK(!field->isType(ValueType::UINT64));
  BOOST_CHECK(!field->isType(ValueType::UTF8));
  BOOST_CHECK(!field->isType(ValueType::SEQUENCE));

  int64 value = field->toInt64();
  BOOST_CHECK(value > 0);

  BOOST_CHECK_EQUAL(4294967295L, value);

  BOOST_CHECK_THROW(field->toAscii(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toInt32(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUInt32(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUInt64(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toDecimal(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUtf8(),UnsupportedConversion);

  int64 posValue = 0x7FFFFFFFFFFFFFFFLL;
  int64 negValue = 0x8000000000000000LL;
  field = Messages::FieldInt64::create(posValue);
  BOOST_CHECK(field->isDefined());
  BOOST_CHECK_EQUAL(posValue, field->toInt64());
  BOOST_CHECK(field->toInt64() > 0);

  field = Messages::FieldInt64::create(negValue);
  BOOST_CHECK_EQUAL(negValue, field->toInt64());
  BOOST_CHECK(field->toInt64() < 0);

}

BOOST_AUTO_TEST_CASE(testFieldUInt32)
{
  Messages::FieldCPtr field = Messages::FieldUInt32::createNull();
  BOOST_CHECK(!field->isDefined());
  BOOST_CHECK(!field->isType(ValueType::ASCII));
  BOOST_CHECK(!field->isType(ValueType::BYTEVECTOR));
  BOOST_CHECK(!field->isType(ValueType::DECIMAL));
  BOOST_CHECK(!field->isType(ValueType::INT32));
  BOOST_CHECK(!field->isType(ValueType::INT64));
  BOOST_CHECK( field->isType(ValueType::UINT32));
  BOOST_CHECK(!field->isType(ValueType::UINT64));
  BOOST_CHECK(!field->isType(ValueType::UTF8));
  BOOST_CHECK(!field->isType(ValueType::SEQUENCE));

  field = Messages::FieldUInt32::create(23);
  BOOST_CHECK(field->isDefined());
  BOOST_CHECK(!field->isType(ValueType::ASCII));
  BOOST_CHECK(!field->isType(ValueType::BYTEVECTOR));
  BOOST_CHECK(!field->isType(ValueType::DECIMAL));
  BOOST_CHECK(!field->isType(ValueType::INT32));
  BOOST_CHECK(!field->isType(ValueType::INT64));
  BOOST_CHECK( field->isType(ValueType::UINT32));
  BOOST_CHECK(!field->isType(ValueType::UINT64));
  BOOST_CHECK(!field->isType(ValueType::UTF8));
  BOOST_CHECK(!field->isType(ValueType::SEQUENCE));

  BOOST_CHECK_EQUAL(23, field->toUInt32());

  BOOST_CHECK_THROW(field->toAscii(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toInt32(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toInt64(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUInt64(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toDecimal(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUtf8(),UnsupportedConversion);

  field = Messages::FieldUInt32::create((uint32)-999999);
  BOOST_CHECK(field->isDefined());
  BOOST_CHECK_EQUAL((uint32)-999999, field->toUInt32());
  BOOST_CHECK(field->toUInt32() > 0);
}

BOOST_AUTO_TEST_CASE(testFieldUInt64)
{
  Messages::FieldCPtr field = Messages::FieldUInt64::createNull();
  BOOST_CHECK(!field->isDefined());
  BOOST_CHECK(!field->isType(ValueType::ASCII));
  BOOST_CHECK(!field->isType(ValueType::BYTEVECTOR));
  BOOST_CHECK(!field->isType(ValueType::DECIMAL));
  BOOST_CHECK(!field->isType(ValueType::INT32));
  BOOST_CHECK(!field->isType(ValueType::INT64));
  BOOST_CHECK(!field->isType(ValueType::UINT32));
  BOOST_CHECK( field->isType(ValueType::UINT64));
  BOOST_CHECK(!field->isType(ValueType::UTF8));
  BOOST_CHECK(!field->isType(ValueType::SEQUENCE));

  field = Messages::FieldUInt64::create(4294967295L);
  BOOST_CHECK(field->isDefined());
  BOOST_CHECK(!field->isType(ValueType::ASCII));
  BOOST_CHECK(!field->isType(ValueType::BYTEVECTOR));
  BOOST_CHECK(!field->isType(ValueType::DECIMAL));
  BOOST_CHECK(!field->isType(ValueType::INT32));
  BOOST_CHECK(!field->isType(ValueType::INT64));
  BOOST_CHECK(!field->isType(ValueType::UINT32));
  BOOST_CHECK( field->isType(ValueType::UINT64));
  BOOST_CHECK(!field->isType(ValueType::UTF8));
  BOOST_CHECK(!field->isType(ValueType::SEQUENCE));

  BOOST_CHECK_EQUAL(4294967295L, field->toUInt64());

  BOOST_CHECK_THROW(field->toAscii(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toInt32(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUInt32(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toInt64(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toDecimal(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUtf8(),UnsupportedConversion);

  uint64 posValue = 0x7FFFFFFFFFFFFFFFLL;
  uint64 negValue = (uint64)0x8000000000000000LL;
  field = Messages::FieldUInt64::create(posValue);
  BOOST_CHECK(field->isDefined());
  BOOST_CHECK_EQUAL(posValue, field->toUInt64());
  BOOST_CHECK(field->toUInt64() > 0);

  field = Messages::FieldUInt64::create(negValue);
  BOOST_CHECK_EQUAL(negValue, field->toUInt64());
  BOOST_CHECK(field->toUInt64() > 0);

}
