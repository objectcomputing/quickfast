// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>

#include <Messages/FieldAscii.h>
#include <Messages/FieldUtf8.h>
#include <Messages/FieldByteVector.h>
#include <Common/Exceptions.h>

using namespace ::QuickFAST;

BOOST_AUTO_TEST_CASE(testFieldAscii)
{
  Messages::FieldCPtr field = Messages::FieldAscii::createNull();
  BOOST_CHECK(!field->isDefined());
  BOOST_CHECK( field->isType(Messages::Field::ASCII));
  BOOST_CHECK(!field->isType(Messages::Field::BYTEVECTOR));
  BOOST_CHECK(!field->isType(Messages::Field::DECIMAL));
  BOOST_CHECK(!field->isType(Messages::Field::INT32));
  BOOST_CHECK(!field->isType(Messages::Field::INT64));
  BOOST_CHECK(!field->isType(Messages::Field::UINT32));
  BOOST_CHECK(!field->isType(Messages::Field::UINT64));
  BOOST_CHECK(!field->isType(Messages::Field::UTF8));
  BOOST_CHECK(!field->isType(Messages::Field::SEQUENCE));

  field = Messages::FieldAscii::create("Hello, World!");
  BOOST_CHECK( field->isDefined());
  BOOST_CHECK( field->isType(Messages::Field::ASCII));
  BOOST_CHECK(!field->isType(Messages::Field::BYTEVECTOR));
  BOOST_CHECK(!field->isType(Messages::Field::DECIMAL));
  BOOST_CHECK(!field->isType(Messages::Field::INT32));
  BOOST_CHECK(!field->isType(Messages::Field::INT64));
  BOOST_CHECK(!field->isType(Messages::Field::UINT32));
  BOOST_CHECK(!field->isType(Messages::Field::UINT64));
  BOOST_CHECK(!field->isType(Messages::Field::UTF8));
  BOOST_CHECK(!field->isType(Messages::Field::SEQUENCE));

  BOOST_CHECK_EQUAL(field->toAscii(), "Hello, World!");

  BOOST_CHECK_THROW(field->toInt32(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUInt32(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toInt64(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUInt64(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toDecimal(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUtf8(),UnsupportedConversion);
}


BOOST_AUTO_TEST_CASE(testFieldUtf8)
{
  Messages::FieldCPtr field = Messages::FieldUtf8::createNull();
  BOOST_CHECK(!field->isDefined());
  BOOST_CHECK(!field->isType(Messages::Field::ASCII));
  BOOST_CHECK(!field->isType(Messages::Field::BYTEVECTOR));
  BOOST_CHECK(!field->isType(Messages::Field::DECIMAL));
  BOOST_CHECK(!field->isType(Messages::Field::INT32));
  BOOST_CHECK(!field->isType(Messages::Field::INT64));
  BOOST_CHECK(!field->isType(Messages::Field::UINT32));
  BOOST_CHECK(!field->isType(Messages::Field::UINT64));
  BOOST_CHECK( field->isType(Messages::Field::UTF8));
  BOOST_CHECK(!field->isType(Messages::Field::SEQUENCE));

  field = Messages::FieldUtf8::create("Hello, World!");
  BOOST_CHECK( field->isDefined());
  BOOST_CHECK(!field->isType(Messages::Field::ASCII));
  BOOST_CHECK(!field->isType(Messages::Field::BYTEVECTOR));
  BOOST_CHECK(!field->isType(Messages::Field::DECIMAL));
  BOOST_CHECK(!field->isType(Messages::Field::INT32));
  BOOST_CHECK(!field->isType(Messages::Field::INT64));
  BOOST_CHECK(!field->isType(Messages::Field::UINT32));
  BOOST_CHECK(!field->isType(Messages::Field::UINT64));
  BOOST_CHECK( field->isType(Messages::Field::UTF8));
  BOOST_CHECK(!field->isType(Messages::Field::SEQUENCE));

  BOOST_CHECK_EQUAL(field->toUtf8(), "Hello, World!");

  BOOST_CHECK_THROW(field->toAscii(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toInt32(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUInt32(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toInt64(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUInt64(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toDecimal(),UnsupportedConversion);
}

BOOST_AUTO_TEST_CASE(testFieldByteVector)
{
  Messages::FieldCPtr field = Messages::FieldByteVector::createNull();
  BOOST_CHECK(!field->isDefined());
  BOOST_CHECK(!field->isType(Messages::Field::ASCII));
  BOOST_CHECK( field->isType(Messages::Field::BYTEVECTOR));
  BOOST_CHECK(!field->isType(Messages::Field::DECIMAL));
  BOOST_CHECK(!field->isType(Messages::Field::INT32));
  BOOST_CHECK(!field->isType(Messages::Field::INT64));
  BOOST_CHECK(!field->isType(Messages::Field::UINT32));
  BOOST_CHECK(!field->isType(Messages::Field::UINT64));
  BOOST_CHECK(!field->isType(Messages::Field::UTF8));
  BOOST_CHECK(!field->isType(Messages::Field::SEQUENCE));

  field = Messages::FieldByteVector::create("Hello, World!");
  BOOST_CHECK( field->isDefined());
  BOOST_CHECK(!field->isType(Messages::Field::ASCII));
  BOOST_CHECK( field->isType(Messages::Field::BYTEVECTOR));
  BOOST_CHECK(!field->isType(Messages::Field::DECIMAL));
  BOOST_CHECK(!field->isType(Messages::Field::INT32));
  BOOST_CHECK(!field->isType(Messages::Field::INT64));
  BOOST_CHECK(!field->isType(Messages::Field::UINT32));
  BOOST_CHECK(!field->isType(Messages::Field::UINT64));
  BOOST_CHECK(!field->isType(Messages::Field::UTF8));
  BOOST_CHECK(!field->isType(Messages::Field::SEQUENCE));

  BOOST_CHECK_EQUAL(field->toByteVector(), "Hello, World!");

  BOOST_CHECK_THROW(field->toAscii(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toInt32(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUInt32(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toInt64(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toUInt64(),UnsupportedConversion);
  BOOST_CHECK_THROW(field->toDecimal(),UnsupportedConversion);
}


