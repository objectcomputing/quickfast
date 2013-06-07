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

BOOST_AUTO_TEST_CASE(TestReplaceField)
{
  std::string xml (std::getenv ("QUICKFAST_ROOT"));
  xml += "/src/Tests/resources/unittest_optional.xml";

  std::ifstream templateStream(xml.c_str(), std::ifstream::binary);

  Codecs::XMLTemplateParser parser;
  Codecs::TemplateRegistryPtr templateRegistry =
    parser.parse(templateStream);

  Messages::Message msg(templateRegistry->maxFieldCount());

  Messages::FieldIdentity identity_int32_nop("int32_nop");
  Messages::FieldSet fields(templateRegistry->maxFieldCount());
  // Add the field
  fields.addField(identity_int32_nop, Messages::FieldInt32::create(17));
  
  int64 value;
  fields.getSignedInteger(identity_int32_nop, ValueType::INT32, value);
  BOOST_CHECK_EQUAL(17, value);

  // Replace the field
  bool result;
  result = fields.replaceField(identity_int32_nop, 
                               Messages::FieldInt32::create(429));
  fields.getSignedInteger(identity_int32_nop, ValueType::INT32, value);
  BOOST_CHECK(result);
  BOOST_CHECK_EQUAL(429, value);

  // Replace field not present
  Messages::FieldIdentity identity_uint64_nop("uint64_nop");
  result = fields.replaceField(identity_uint64_nop, 
                               Messages::FieldInt32::create(333));
  BOOST_CHECK(!result);
  BOOST_CHECK(!fields.isPresent(identity_uint64_nop));
}

