// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <Codecs/XMLTemplateParser.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/Encoder.h>
#include <Codecs/Decoder.h>
#include <Codecs/DataDestination.h>
#include <Codecs/DataSourceString.h>
#include <Codecs/SingleMessageConsumer.h>
#include <Codecs/GenericMessageBuilder.h>

#include <Messages/Message.h>
#include <Messages/FieldIdentity.h>
#include <Messages/FieldInt32.h>
#include <Messages/FieldUInt32.h>
#include <Messages/FieldInt64.h>
#include <Messages/FieldUInt64.h>
#include <Messages/FieldAscii.h>
#include <Messages/FieldByteVector.h>
#include <Messages/FieldDecimal.h>
#include <Messages/FieldGroup.h>
#include <Messages/FieldSequence.h>
#include <Messages/FieldUtf8.h>
#include <Messages/Sequence.h>

#include <Common/Exceptions.h>

using namespace QuickFAST;

   // test with various combinations: (8*5 + 4  + 3) * 2  = 94

   //8 primitive field types:
   //   int32, int64, uint32, uint64, decimal, ascii string, utf8 string,
   //byte vector

   //5 general purpose operators
   //   nop, constant, default, copy, delta,

   //2 special purpose operators:
   //   increment (applies only to 4 integer types), tail (applies only
   //to 3 types: ascii, utf8, and byte vector)

   //2 presence values
   //  mandatory, required

namespace{

  void validateMessage1(Messages::Message & message, const std::string buffer)
  {
    BOOST_CHECK_EQUAL(message.getApplicationType(), "unittestdata");
    Messages::FieldCPtr value;

    //<int32 name="int32_nop" id="1">
    //msg.addField(identity_int32_nop, Messages::FieldInt32::create(2147483647));
    BOOST_CHECK(message.getField("int32_nop", value));
    BOOST_CHECK_EQUAL(value->toInt32(), 2147483647);
    //<uInt32 name="uint32_nop" id="2">
    //msg.addField(identity_uint32_nop, Messages::FieldUInt32::create(4294967295));
    BOOST_CHECK(message.getField("uint32_nop", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 4294967295UL);
    //<int64 name="int64_nop" id="3">
    //msg.addField(identity_int64_nop, Messages::FieldInt64::create(9223372036854775807));
    BOOST_CHECK(message.getField("int64_nop", value));
    BOOST_CHECK_EQUAL(value->toInt64(), 9223372036854775807LL);

    //<uInt64 name="uint64_nop" id="4">
    //msg.addField(identity_uint64_nop, Messages::FieldUInt64::create(18446744073709551615));
    BOOST_CHECK(message.getField("uint64_nop", value));
    BOOST_CHECK_EQUAL(value->toUInt64(), 18446744073709551615ULL);

    //<decimal name="decimal_nop" id="5">
    //msg.addField(identity_decimal_nop, Messages::FieldDecimal::create(Decimal(9223372036854775807, 63)));
    BOOST_CHECK(message.getField("decimal_nop", value));
    BOOST_CHECK_EQUAL(value->toDecimal(), Decimal (9223372036854775807LL, 63));

    //<string name="asciistring_nop" charset="ascii" id="6">
    //msg.addField(identity_asciistring_nop, Messages::FieldAscii::create(buffer.c_str()));
    BOOST_CHECK(message.getField("asciistring_nop", value));
    BOOST_CHECK_EQUAL(value->toAscii(), buffer.c_str());

    //<string name="utf8string_nop" charset="unicode" id="7">
    //msg.addField(identity_utf8string_nop, Messages::FieldAscii::create(buffer.c_str()));
    BOOST_CHECK(message.getField("utf8string_nop", value));
    BOOST_CHECK_EQUAL(value->toUtf8(), buffer.c_str());

    //<byteVector name="bytevector_nop" id="8">
    //msg.addField(identity_bytevector_nop, Messages::FieldByteVector::create(""));
    BOOST_CHECK(message.getField("bytevector_nop", value));
    BOOST_CHECK_EQUAL(value->toByteVector(), buffer.c_str());

    //  <int32 name="int32_const" id="9"><constant value="2147483647"/>
    //msg.addField(identity_int32_const, Messages::FieldInt32::create(2147483647));
    BOOST_CHECK(message.getField("int32_const", value));
    BOOST_CHECK_EQUAL(value->toInt32(), 2147483647L);

    //  <uInt32 name="uint32_const" id="10"><constant value="0"/>
    //msg.addField(identity_uint32_const, Messages::FieldUInt32::create(4294967295));
    BOOST_CHECK(message.getField("uint32_const", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 4294967295UL);

    //  <int64 name="int64_const" id="11"><constant value="9223372036854775807"/>
    //msg.addField(identity_int64_const, Messages::FieldInt64::create(9223372036854775807));
    BOOST_CHECK(message.getField("int64_const", value));
    BOOST_CHECK_EQUAL(value->toInt64(), 9223372036854775807LL);

#if 0 // Disable this test due to a bug in boost::lexical_cast: https://svn.boost.org/trac/boost/ticket/2295
    //  <uInt64 name="uint64_const" id="12"><constant value="0"/>
    //msg.addField(identity_uint64_const, Messages::FieldUInt64::create(18446744073709551615));
    BOOST_CHECK(message.getField("uint64_const", value));
    BOOST_CHECK_EQUAL(value->toUInt64(), 18446744073709551615ULL);
#endif
    //  <decimal name="decimal_const" id="13"><constant value="9223372036854775807000000000000000000000000000000000000000000000000000000000000000"/>
    //msg.addField(identity_decimal_const, Messages::FieldDecimal::create(Decimal(9223372036854775807, 63)));
    BOOST_CHECK(message.getField("decimal_const", value));
    BOOST_CHECK_EQUAL(value->toDecimal(), Decimal(9223372036854775807LL, 63));

    //  <string name="asciistring_const" charset="ascii" id="14"><constant value="$buffer"/>
    //msg.addField(identity_asciistring_const, Messages::FieldAscii::create(buffer.c_str()));
    BOOST_CHECK(message.getField("asciistring_const", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "$buffer");

    //  <string name="utf8string_const" charset="unicode" id="15"><constant value="$buffer"/>
    //msg.addField(identity_utf8string_const, Messages::FieldAscii::create(buffer.c_str()));
    BOOST_CHECK(message.getField("utf8string_const", value));
    BOOST_CHECK_EQUAL(value->toUtf8(), "$buffer");

    //  <byteVector name="bytevector_const" id="16"><constant value="longgest string"/>
    //msg.addField(identity_bytevector_const, Messages::FieldByteVector::create(buffer.c_str()));
    BOOST_CHECK(message.getField("bytevector_const", value));
    BOOST_CHECK_EQUAL(value->toByteVector(), "$buffer");

    //  <int32 name="int32_default" id="17"><default value="2147483647"/>
    //msg.addField(identity_int32_default, Messages::FieldInt32::create(2147483647));
    BOOST_CHECK(message.getField("int32_default", value));
    BOOST_CHECK_EQUAL(value->toInt32(), 2147483647);

    //  <uInt32 name="uint32_default" id="18"><default value="0"/>
    //msg.addField(identity_uint32_default, Messages::FieldUInt32::create(4294967295));
    BOOST_CHECK(message.getField("uint32_default", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 4294967295UL);

    //  <int64 name="int64_default" id="19"><default value="9223372036854775807"/>
    //msg.addField(identity_int64_default, Messages::FieldInt64::create(9223372036854775807));
    BOOST_CHECK(message.getField("int64_default", value));
    BOOST_CHECK_EQUAL(value->toInt64(), 9223372036854775807LL);

#if 0 // Disable this test due to a bug in boost::lexical_cast: https://svn.boost.org/trac/boost/ticket/2295
    //  <uInt64 name="uint64_default" id="20"><default value="0"/>
    //msg.addField(identity_uint64_default, Messages::FieldUInt64::create(18446744073709551615));
    BOOST_CHECK(message.getField("uint64_default", value));
    BOOST_CHECK_EQUAL(value->toUInt64(), 18446744073709551615ULL);
#endif
    //  <decimal name="decimal_default" id="21"><default value="9223372036854775807000000000000000000000000000000000000000000000000000000000000000"/>
    //msg.addField(identity_decimal_default, Messages::FieldDecimal::create(Decimal(9223372036854775807, 63)));
    BOOST_CHECK(message.getField("decimal_default", value));
    BOOST_CHECK_EQUAL(value->toDecimal(), Decimal(9223372036854775807LL, 63));

    //  <string name="asciistring_default" charset="ascii" id="22"><default value="$buffer"/>
    //msg.addField(identity_asciistring_default, Messages::FieldAscii::create(buffer.c_str()));
    BOOST_CHECK(message.getField("asciistring_default", value));
    BOOST_CHECK_EQUAL(value->toAscii(), buffer.c_str());

    //  <string name="utf8string_default" charset="unicode" id="23"><default value="$buffer"/>
    //msg.addField(identity_utf8string_default, Messages::FieldAscii::create(buffer.c_str()));
    BOOST_CHECK(message.getField("utf8string_default", value));
    BOOST_CHECK_EQUAL(value->toUtf8(), buffer.c_str());

    //  <byteVector name="bytevector_default" id="24"><default value="$buffer"/>
    //msg.addField(identity_bytevector_default, Messages::FieldByteVector::create(buffer.c_str()));
    BOOST_CHECK(message.getField("bytevector_default", value));
    BOOST_CHECK_EQUAL(value->toByteVector(), buffer.c_str());

    //  <int32 name="int32_copy" id="25"><copy/>
    //msg.addField(identity_int32_copy, Messages::FieldInt32::create(2147483647));
    BOOST_CHECK(message.getField("int32_copy", value));
    BOOST_CHECK_EQUAL(value->toInt32(), 2147483647);

    //  <uInt32 name="uint32_copy" id="26"><copy/>
    //msg.addField(identity_uint32_copy, Messages::FieldUInt32::create(4294967295));
    BOOST_CHECK(message.getField("uint32_copy", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 4294967295UL);

    //  <int64 name="int64_copy" id="27"><copy/>
    //msg.addField(identity_int64_copy, Messages::FieldInt64::create(9223372036854775807));
    BOOST_CHECK(message.getField("int64_copy", value));
    BOOST_CHECK_EQUAL(value->toInt64(), 9223372036854775807LL);

    //  <uInt64 name="uint64_copy" id="28"><copy/>
    // msg.addField(identity_uint64_copy, Messages::FieldUInt64::create(18446744073709551615));
    BOOST_CHECK(message.getField("uint64_copy", value));
    BOOST_CHECK_EQUAL(value->toUInt64(), 18446744073709551615ULL);

    //  <decimal name="decimal_copy" id="29"><copy value="9223372036854775807000000000000000000000000000000000000000000000000000000000000000"/>
    //msg.addField(identity_decimal_copy, Messages::FieldDecimal::create(Decimal(9223372036854775807, 63)));
    BOOST_CHECK(message.getField("decimal_copy", value));
    BOOST_CHECK_EQUAL(value->toDecimal(), Decimal(9223372036854775807LL, 63));

    //  <string name="asciistring_copy" charset="ascii" id="30"><copy/>
    //msg.addField(identity_asciistring_copy, Messages::FieldAscii::create(buffer.c_str()));
    BOOST_CHECK(message.getField("asciistring_copy", value));
    BOOST_CHECK_EQUAL(value->toAscii(), buffer.c_str());

    //  <string name="utf8string_copy" charset="unicode" id="31"><copy/>
    //msg.addField(identity_utf8string_copy, Messages::FieldAscii::create(buffer.c_str()));
    BOOST_CHECK(message.getField("utf8string_copy", value));
    BOOST_CHECK_EQUAL(value->toUtf8(), buffer.c_str());

    //  <byteVector name="bytevector_copy" id="32"><copy/>
    //msg.addField(identity_bytevector_copy, Messages::FieldByteVector::create(buffer.c_str()));
    BOOST_CHECK(message.getField("bytevector_copy", value));
    BOOST_CHECK_EQUAL(value->toByteVector(), buffer.c_str());

    //  <int32 name="int32_delta" id="33"><copy/>
    //msg.addField(identity_int32_delta, Messages::FieldInt32::create(2147483647));
    BOOST_CHECK(message.getField("int32_delta", value));
    BOOST_CHECK_EQUAL(value->toInt32(), 2147483647);

    //  <uInt32 name="uint32_delta" id="34"><delta/>
    //msg.addField(identity_uint32_delta, Messages::FieldUInt32::create(4294967295));
    BOOST_CHECK(message.getField("uint32_delta", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 4294967295UL);

    //  <int64 name="int64_delta" id="35"><delta/>
    //msg.addField(identity_int64_delta, Messages::FieldInt64::create(9223372036854775807));
    BOOST_CHECK(message.getField("int64_delta", value));
    BOOST_CHECK_EQUAL(value->toInt64(), 9223372036854775807LL);

    //  <uInt64 name="uint64_delta" id="36"><delta/>
    //msg.addField(identity_uint64_delta, Messages::FieldUInt64::create(18446744073709551615));
    BOOST_CHECK(message.getField("uint64_delta", value));
    BOOST_CHECK_EQUAL(value->toUInt64(), 18446744073709551615ULL);

    //  <decimal name="decimal_delta" id="37"><delta/>
    //msg.addField(identity_decimal_delta, Messages::FieldDecimal::create(Decimal(9223372036854775807, 63)));
    BOOST_CHECK(message.getField("decimal_delta", value));
    BOOST_CHECK_EQUAL(value->toDecimal(), Decimal(9223372036854775807LL, 63));

    //  <string name="asciistring_delta" charset="ascii" id="38"><delta/>
    //msg.addField(identity_asciistring_delta, Messages::FieldAscii::create(buffer.c_str()));
    BOOST_CHECK(message.getField("asciistring_delta", value));
    BOOST_CHECK_EQUAL(value->toAscii(), buffer.c_str());

    //  <string name="utf8string_delta" charset="unicode" id="39"><delta/>
    //msg.addField(identity_utf8string_delta, Messages::FieldAscii::create(buffer.c_str()));
    BOOST_CHECK(message.getField("utf8string_delta", value));
    BOOST_CHECK_EQUAL(value->toUtf8(), buffer.c_str());

    //  <byteVector name="bytevector_delta" id="40"><delta/>
    //msg.addField(identity_bytevector_delta, Messages::FieldByteVector::create(buffer.c_str()));
    BOOST_CHECK(message.getField("bytevector_delta", value));
    BOOST_CHECK_EQUAL(value->toByteVector(), buffer.c_str());

    //  <int32 name="int32_incre" id="41"><increment value="1"/>
    //msg.addField(identity_int32_incre, Messages::FieldInt32::create(1));
    BOOST_CHECK(message.getField("int32_incre", value));
    BOOST_CHECK_EQUAL(value->toInt32(), 1);

    //  <uInt32 name="uint32_incre" id="42"><increment value="1"/>
    //msg.addField(identity_uint32_incre, Messages::FieldUInt32::create(1));
    BOOST_CHECK(message.getField("uint32_incre", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 1);

    //  <int64 name="int64_incre" id="43"><increment value="1"/>
    //msg.addField(identity_int64_incre, Messages::FieldInt64::create(1));
    BOOST_CHECK(message.getField("int64_incre", value));
    BOOST_CHECK_EQUAL(value->toInt64(), 1);

    //  <uInt64 name="uint64_incre" id="44"><increment value="1"/>
    //msg.addField(identity_uint64_incre, Messages::FieldUInt64::create(1));
    BOOST_CHECK(message.getField("uint64_incre", value));
    BOOST_CHECK_EQUAL(value->toUInt64(), 1);

    //  <string name="asciistring_tail" charset="ascii" id="45"><tail/>
    //msg.addField(identity_asciistring_tail, Messages::FieldAscii::create(buffer.c_str()));
    BOOST_CHECK(message.getField("asciistring_tail", value));
    BOOST_CHECK_EQUAL(value->toAscii(), buffer.c_str ());

    //  <string name="utf8string_tail" charset="unicode" id="46"><tail/>
    //msg.addField(identity_utf8string_tail, Messages::FieldAscii::create(buffer.c_str()));
    BOOST_CHECK(message.getField("utf8string_tail", value));
    BOOST_CHECK_EQUAL(value->toUtf8(), buffer.c_str ());

    //  <byteVector name="bytevector_tail" id="47"><tail/>
    //msg.addField(identity_bytevector_tail, Messages::FieldByteVector::create(buffer.c_str()));
    BOOST_CHECK(message.getField("bytevector_tail", value));
    BOOST_CHECK_EQUAL(value->toByteVector(), buffer.c_str ());

  }


  void biggest_value_test (const std::string& xml, const std::string& buffer)
  {
    std::ifstream templateStream(xml.c_str(), std::ifstream::binary);

    Codecs::XMLTemplateParser parser;
    Codecs::TemplateRegistryPtr templateRegistry =
      parser.parse(templateStream);

    Messages::Message msg(templateRegistry->maxFieldCount());

    //<int32 name="int32_nop" id="1">
    Messages::FieldIdentity identity_int32_nop("int32_nop");
    //<uInt32 name="uint32_nop" id="2">
    Messages::FieldIdentity identity_uint32_nop("uint32_nop");
    //<int64 name="int64_nop" id="3">
    Messages::FieldIdentity identity_int64_nop("int64_nop");
    //<uInt64 name="uint64_nop" id="4">
    Messages::FieldIdentity identity_uint64_nop("uint64_nop");
    //<decimal name="decimal_nop" id="5">
    Messages::FieldIdentity identity_decimal_nop("decimal_nop");
    //<string name="asciistring_nop" charset="ascii" id="6">
    Messages::FieldIdentity identity_asciistring_nop("asciistring_nop");
    //<string name="utf8string_nop" charset="unicode" id="7">
    Messages::FieldIdentity identity_utf8string_nop("utf8string_nop");
    //<byteVector name="bytevector_nop" id="8">
    Messages::FieldIdentity identity_bytevector_nop("bytevector_nop");
    //  <int32 name="int32_const" id="9"><constant value="2147483647"/>
    Messages::FieldIdentity identity_int32_const("int32_const");
    //  <uInt32 name="uint32_const" id="10"><constant value="4294967295"/>
    Messages::FieldIdentity identity_uint32_const("uint32_const");
    //  <int64 name="int64_const" id="11"><constant value="9223372036854775807"/>
    Messages::FieldIdentity identity_int64_const("int64_const");
#if 0 // Disable this test due to a bug in boost::lexical_cast: https://svn.boost.org/trac/boost/ticket/2295
    //  <uInt64 name="uint64_const" id="12"><constant value="18446744073709551615"/>
    Messages::FieldIdentity identity_uint64_const("uint64_const");
#endif
    //<decimal name="decimal_const" id="13"><constant value="9223372036854775807000000000000000000000000000000000000000000000000000000000000000"/>
    Messages::FieldIdentity identity_decimal_const("decimal_const");
    //  <string name="asciistring_const" charset="ascii" id="14"><constant value="$buffer"/>
    Messages::FieldIdentity identity_asciistring_const("asciistring_const");
    //  <string name="utf8string_const" charset="unicode" id="15"><constant value="$buffer"/>
    Messages::FieldIdentity identity_utf8string_const("utf8string_const");
    //  <byteVector name="bytevector_const" id="16"><constant value="$buffer"/>
    Messages::FieldIdentity identity_bytevector_const("bytevector_const");
    //  <int32 name="int32_default" id="17"><default value="2147483647"/>
    Messages::FieldIdentity identity_int32_default("int32_default");
    //  <uInt32 name="uint32_default" id="18"><default value="0"/>
    Messages::FieldIdentity identity_uint32_default("uint32_default");
    //  <uInt64 name="int64_default" id="19"><default value="9223372036854775807"/>
    Messages::FieldIdentity identity_int64_default("int64_default");
#if 0 // Disable this test due to a bug in boost::lexical_cast: https://svn.boost.org/trac/boost/ticket/2295
    //  <uInt64 name="uint64_default" id="20"><default value="0"/>
    Messages::FieldIdentity identity_uint64_default("uint64_default");
#endif
    //  <decimal name="decimal_default" id="21"><default value="9223372036854775807000000000000000000000000000000000000000000000000000000000000000"/>
    Messages::FieldIdentity identity_decimal_default("decimal_default");
    //  <string name="asciistring_default" charset="ascii" id="22"><default value="default asciistring"/>
    Messages::FieldIdentity identity_asciistring_default("asciistring_default");
    //  <string name="utf8string_default" charset="unicode" id="23"><default value="default utf8string"/>
    Messages::FieldIdentity identity_utf8string_default("utf8string_default");
    //  <byteVector name="bytevector_default" id="24"><default value="$buffer"/>
    Messages::FieldIdentity identity_bytevector_default("bytevector_default");
    //  <int32 name="int32_copy" id="25"><copy/>
    Messages::FieldIdentity identity_int32_copy("int32_copy");
    //  <uInt32 name="uint32_copy" id="26"><copy/>
    Messages::FieldIdentity identity_uint32_copy("uint32_copy");
    //  <int64 name="int64_copy" id="27"><copy/>
    Messages::FieldIdentity identity_int64_copy("int64_copy");
    //  <uInt64 name="uint64_copy" id="28"><copy/>
    Messages::FieldIdentity identity_uint64_copy("uint64_copy");
    //  <decimal name="decimal_copy" id="29"><copy value="9223372036854775807000000000000000000000000000000000000000000000000000000000000000"/>
    Messages::FieldIdentity identity_decimal_copy("decimal_copy");
    //  <string name="asciistring_copy" charset="ascii" id="30"><copy/>
    Messages::FieldIdentity identity_asciistring_copy("asciistring_copy");
    //  <string name="utf8string_copy" charset="unicode" id="31"><copy/>
    Messages::FieldIdentity identity_utf8string_copy("utf8string_copy");
    //  <byteVector name="bytevector_copy" id="32"><copy/>
    Messages::FieldIdentity identity_bytevector_copy("bytevector_copy");
    //  <int32 name="int32_delta" id="33"><copy/>
    Messages::FieldIdentity identity_int32_delta("int32_delta");
    //  <uInt32 name="uint32_delta" id="34"><delta/>
    Messages::FieldIdentity identity_uint32_delta("uint32_delta");
    //  <int64 name="int64_delta" id="35"><delta/>
    Messages::FieldIdentity identity_int64_delta("int64_delta");
    //  <uInt64 name="uint64_delta" id="36"><delta/>
    Messages::FieldIdentity identity_uint64_delta("uint64_delta");
    //  <decimal name="decimal_delta" id="37"><delta/>
    Messages::FieldIdentity identity_decimal_delta("decimal_delta");
    //  <string name="asciistring_delta" charset="ascii" id="38"><delta/>
    Messages::FieldIdentity identity_asciistring_delta("asciistring_delta");
    //  <string name="utf8string_delta" charset="unicode" id="39"><delta/>
    Messages::FieldIdentity identity_utf8string_delta("utf8string_delta");
    //  <byteVector name="bytevector_delta" id="40"><delta/>
    Messages::FieldIdentity identity_bytevector_delta("bytevector_delta");
    //  <int32 name="int32_incre" id="41"><increment value="1"/>
    Messages::FieldIdentity identity_int32_incre("int32_incre");
    //  <uInt32 name="uint32_incre" id="42"><increment value="1"/>
    Messages::FieldIdentity identity_uint32_incre("uint32_incre");
    //  <int64 name="int64_incre" id="43"><increment value="1"/>
    Messages::FieldIdentity identity_int64_incre("int64_incre");
    //  <uInt64 name="uint64_incre" id="44"><increment value="1"/>
    Messages::FieldIdentity identity_uint64_incre("uint64_incre");
    //  <string name="asciistring_tail" charset="ascii" id="45"><tail/>
    Messages::FieldIdentity identity_asciistring_tail("asciistring_tail");
    //  <string name="utf8string_tail" charset="unicode" id="46"><tail/>
    Messages::FieldIdentity identity_utf8string_tail("utf8string_tail");
    //  <byteVector name="bytevector_tail" id="47"><tail/>
    Messages::FieldIdentity identity_bytevector_tail("bytevector_tail");

    //<int32 name="int32_nop" id="1">
    msg.addField(identity_int32_nop, Messages::FieldInt32::create(2147483647));
    //<uInt32 name="uint32_nop" id="2">
    msg.addField(identity_uint32_nop, Messages::FieldUInt32::create(4294967295UL));
    //<int64 name="int64_nop" id="3">
    msg.addField(identity_int64_nop, Messages::FieldInt64::create(9223372036854775807LL));
    //<uInt64 name="uint64_nop" id="4">
    msg.addField(identity_uint64_nop, Messages::FieldUInt64::create(18446744073709551615ULL));
    //<decimal name="decimal_nop" id="5">
    msg.addField(identity_decimal_nop, Messages::FieldDecimal::create(Decimal(9223372036854775807LL, 63)));
    //<string name="asciistring_nop" charset="ascii" id="6">
    msg.addField(identity_asciistring_nop, Messages::FieldAscii::create(buffer.c_str()));
    //<string name="utf8string_nop" charset="unicode" id="7">
    msg.addField(identity_utf8string_nop, Messages::FieldAscii::create(buffer.c_str()));
    //<byteVector name="bytevector_nop" id="8">
    msg.addField(identity_bytevector_nop, Messages::FieldByteVector::create(buffer.c_str()));
    //  <int32 name="int32_const" id="9"><constant value="2147483647"/>
    msg.addField(identity_int32_const, Messages::FieldInt32::create(2147483647));
    //  <uInt32 name="uint32_const" id="10"><constant value="0"/>
    msg.addField(identity_uint32_const, Messages::FieldUInt32::create(4294967295UL));
    //  <int64 name="int64_const" id="11"><constant value="9223372036854775807"/>
    msg.addField(identity_int64_const, Messages::FieldInt64::create(9223372036854775807LL));
#if 0 // Disable this test due to a bug in boost::lexical_cast: https://svn.boost.org/trac/boost/ticket/2295
    //  <uInt64 name="uint64_const" id="12"><constant value="9223372036854775807"/>
    msg.addField(identity_uint64_const, Messages::FieldUInt64::create(18446744073709551615ULL));
#endif
    //  <decimal name="decimal_const" id="13"><constant value="9223372036854775807000000000000000000000000000000000000000000000000000000000000000"/>
    msg.addField(identity_decimal_const, Messages::FieldDecimal::create(Decimal(9223372036854775807LL, 63)));
    //  <string name="asciistring_const" charset="ascii" id="14"><constant value="$buffer"/>
    msg.addField(identity_asciistring_const, Messages::FieldAscii::create("$buffer"));
    //  <string name="utf8string_const" charset="unicode" id="15"><constant value="$buffer"/>
    msg.addField(identity_utf8string_const, Messages::FieldAscii::create("$buffer"));
    //  <byteVector name="bytevector_const" id="16"><constant value="$buffer"/>
    msg.addField(identity_bytevector_const, Messages::FieldByteVector::create("$buffer"));
    //  <int32 name="int32_default" id="17"><default value="2147483647"/>
    msg.addField(identity_int32_default, Messages::FieldInt32::create(2147483647));
    //  <uInt32 name="uint32_default" id="18"><default value="0"/>
    msg.addField(identity_uint32_default, Messages::FieldUInt32::create(4294967295UL));
    //  <int64 name="int64_default" id="19"><default value="9223372036854775807"/>
    msg.addField(identity_int64_default, Messages::FieldInt64::create(9223372036854775807LL));
#if 0 // Disable this test due to a bug in boost::lexical_cast: https://svn.boost.org/trac/boost/ticket/2295
    //  <uInt64 name="uint64_default" id="20"><default value="0"/>
    msg.addField(identity_uint64_default, Messages::FieldUInt64::create(18446744073709551615ULL));
#endif
    //  <decimal name="decimal_default" id="21"><default value="9223372036854775807000000000000000000000000000000000000000000000000000000000000000"/>
    msg.addField(identity_decimal_default, Messages::FieldDecimal::create(Decimal(9223372036854775807LL, 63)));
    //  <string name="asciistring_default" charset="ascii" id="22"><default value="$buffer"/>
    msg.addField(identity_asciistring_default, Messages::FieldAscii::create(buffer.c_str()));
    //  <string name="utf8string_default" charset="unicode" id="23"><default value="$buffer"/>
    msg.addField(identity_utf8string_default, Messages::FieldAscii::create(buffer.c_str()));
    //  <byteVector name="bytevector_default" id="24"><default value="$buffer"/>
    msg.addField(identity_bytevector_default, Messages::FieldByteVector::create(buffer.c_str()));
    //  <int32 name="int32_copy" id="25"><copy/>
    msg.addField(identity_int32_copy, Messages::FieldInt32::create(2147483647));
    //  <uInt32 name="uint32_copy" id="26"><copy/>
    msg.addField(identity_uint32_copy, Messages::FieldUInt32::create(4294967295UL));
    //  <int64 name="int64_copy" id="27"><copy/>
    msg.addField(identity_int64_copy, Messages::FieldInt64::create(9223372036854775807LL));
    //  <uInt64 name="uint64_copy" id="28"><copy/>
    msg.addField(identity_uint64_copy, Messages::FieldUInt64::create(18446744073709551615ULL));
    //  <decimal name="decimal_copy" id="29"><copy value="9223372036854775807000000000000000000000000000000000000000000000000000000000000000"/>
    msg.addField(identity_decimal_copy, Messages::FieldDecimal::create(Decimal(9223372036854775807LL, 63)));
    //  <string name="asciistring_copy" charset="ascii" id="30"><copy/>
    msg.addField(identity_asciistring_copy, Messages::FieldAscii::create(buffer.c_str()));
    //  <string name="utf8string_copy" charset="unicode" id="31"><copy/>
    msg.addField(identity_utf8string_copy, Messages::FieldAscii::create(buffer.c_str()));
    //  <byteVector name="bytevector_copy" id="32"><copy/>
    msg.addField(identity_bytevector_copy, Messages::FieldByteVector::create(buffer.c_str()));
    //  <int32 name="int32_delta" id="33"><copy/>
    msg.addField(identity_int32_delta, Messages::FieldInt32::create(2147483647));
    //  <uInt32 name="uint32_delta" id="34"><delta/>
    msg.addField(identity_uint32_delta, Messages::FieldUInt32::create(4294967295UL));
    //  <int64 name="int64_delta" id="35"><delta/>
    msg.addField(identity_int64_delta, Messages::FieldInt64::create(9223372036854775807LL));
    //  <uInt64 name="uint64_delta" id="36"><delta/>
    msg.addField(identity_uint64_delta, Messages::FieldUInt64::create(18446744073709551615ULL));
    //  <decimal name="decimal_delta" id="37"><delta/>
    msg.addField(identity_decimal_delta, Messages::FieldDecimal::create(Decimal(9223372036854775807LL, 63)));
    //  <string name="asciistring_delta" charset="ascii" id="38"><delta/>
    msg.addField(identity_asciistring_delta, Messages::FieldAscii::create(buffer.c_str()));
    //  <string name="utf8string_delta" charset="unicode" id="39"><delta/>
    msg.addField(identity_utf8string_delta, Messages::FieldAscii::create(buffer.c_str()));
    //  <byteVector name="bytevector_delta" id="40"><delta/>
    msg.addField(identity_bytevector_delta, Messages::FieldByteVector::create(buffer.c_str()));
    //  <int32 name="int32_incre" id="41"><increment value="1"/>
    msg.addField(identity_int32_incre, Messages::FieldInt32::create(1));
    //  <uInt32 name="uint32_incre" id="42"><increment value="1"/>
    msg.addField(identity_uint32_incre, Messages::FieldUInt32::create(1));
    //  <int64 name="int64_incre" id="43"><increment value="1"/>
    msg.addField(identity_int64_incre, Messages::FieldInt64::create(1));
    //  <uInt64 name="uint64_incre" id="44"><increment value="1"/>
    msg.addField(identity_uint64_incre, Messages::FieldUInt64::create(1));
    //  <string name="asciistring_tail" charset="ascii" id="45"><tail/>
    msg.addField(identity_asciistring_tail, Messages::FieldAscii::create(buffer.c_str ()));
    //  <string name="utf8string_tail" charset="unicode" id="46"><tail/>
    msg.addField(identity_utf8string_tail, Messages::FieldAscii::create(buffer.c_str ()));
    //  <byteVector name="bytevector_tail" id="47"><tail/>
    msg.addField(identity_bytevector_tail, Messages::FieldByteVector::create(buffer.c_str ()));

    Codecs::Encoder encoder(templateRegistry);
    //encoder.setVerboseOutput (std::cout);
    Codecs::DataDestination destination;
    template_id_t templId = 3; // from the XML file
    encoder.encodeMessage(destination, templId, msg);
    std::string fastString;
    destination.toString(fastString);

    destination.clear();

    Codecs::Decoder decoder(templateRegistry);
    //decoder.setVerboseOutput (std::cout);

    Codecs::DataSourceString source(fastString);
    Codecs::SingleMessageConsumer consumer;
    Codecs::GenericMessageBuilder builder(consumer);
    decoder.decodeMessage(source, builder);
    Messages::Message & msgOut = consumer.message();

    validateMessage1(msgOut, buffer);

    // wanna see it again?
    encoder.reset();
    encoder.encodeMessage(destination, templId, msgOut);
    std::string reencoded;
    destination.toString(reencoded);

    destination.clear();

    BOOST_CHECK(fastString == reencoded);
  }

  char random_char()
  {
   //
   // n = ( rand() % ( max - min + 1 ) ) + min;
   //  122 = 'z', 97 = 'a'
   // n = ( rand() % ( 122 - 97 + 1 ) ) + 97
   return ( rand() % ( 26 ) ) + 97;
  }

  //
  // Returns a randomly generated string.
  //
  void string_generate(std::string& str, size_t len)
  {
   //
   // Generate 10 random characters.
   //
   std::generate_n( std::back_inserter( str ), len, random_char );
  }
}



BOOST_AUTO_TEST_CASE(TestBiggestValue)
{
//  uint64 compilerGenerated = 18446744073709551615ULL;
////  std::string biggestUInt64 = "18446744073709551615";
//  std::string biggestUInt64 = "9223372036854775808";
//  uint64 boostGenerated = boost::lexical_cast<unsigned long long>(biggestUInt64);

  std::string xml (std::getenv ("QUICKFAST_ROOT"));
  xml += "/src/Tests/resources/biggest_value.xml";

  std::string str;
  string_generate (str, 1000);

  biggest_value_test (xml, str);
}

