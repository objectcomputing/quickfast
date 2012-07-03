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

namespace{
      //<uInt32 name="field1" id="A">
    Messages::FieldIdentity identity_field1("field1");
    //<uInt32 name="field2" id="B">
    Messages::FieldIdentity identity_field2("field2");
    //<group name="Group" presence="optional">
    Messages::FieldIdentity identity_group("Group");
}
namespace
{
  const char templateString[] = "\n\
    <templates>\n\
      <template name=\"TEMPLATE_1\" id=\"2\">\n\
        <uInt32 name=\"field1\" id=\"A\">\n\
        </uInt32>\n\
        <uInt32 name=\"field2\" id=\"B\">\n\
        </uInt32>\n\
      </template>\n\
      <template name=\"unittest\" id=\"3\">\n\
        <typeRef name=\"unittestdata\"/>\"\n\
        <group name=\"Group\" presence=\"optional\">\n\
          <templateRef name=\"TEMPLATE_1\"/>\n\
        </group>\n\
      </template>\n\
    </templates>\n\
  ";


  void validateMessage1(Messages::Message & message)
  {
    BOOST_CHECK_EQUAL(message.getApplicationType(), "unittestdata");
    Messages::FieldCPtr value;

    //<group name="Group" presence="optional">
    BOOST_REQUIRE(message.getField("Group", value));

    Messages::FieldSetCPtr group = value->toGroup();

  //<uInt32 name="field1" id="A">
    BOOST_CHECK(group->getField("field1", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 1);

  //<uInt32 name="field2" id="B">
    BOOST_CHECK(group->getField("field2", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 2);

  }


  BOOST_AUTO_TEST_CASE(TestGroupReferences)
  {
//    _crtBreakAlloc = 15607;
//    _crtBreakAlloc = 15613;
    std::stringstream templateStream(templateString);

    Codecs::XMLTemplateParser parser;
    Codecs::TemplateRegistryPtr templateRegistry =
      parser.parse(templateStream);

    Messages::Message msg(templateRegistry->maxFieldCount());

    // create field set to go into the group
    Messages::FieldSetPtr entry(new Messages::FieldSet(2));
    entry->addField(identity_field1, Messages::FieldUInt32::create(1));
    entry->addField(identity_field2, Messages::FieldUInt32::create(2));

    // add the group field
    msg.addField( identity_group, Messages::FieldGroup::create(entry) );

    Codecs::Encoder encoder(templateRegistry);
    Codecs::DataDestination destination;
    template_id_t templId = 3; // from the XML file
    encoder.encodeMessage(destination, templId, msg);
    std::string fastString;
    destination.toString(fastString);

    destination.clear();

    Codecs::Decoder decoder(templateRegistry);

    Codecs::DataSourceString source(fastString);
    Codecs::SingleMessageConsumer consumer;
    Codecs::GenericMessageBuilder builder(consumer);
    decoder.decodeMessage(source, builder);
    Messages::Message & msgOut = consumer.message();

    validateMessage1(msgOut);

    // wanna see it again?
    encoder.reset();
    encoder.encodeMessage(destination, templId, msgOut);
    std::string reencoded;
    destination.toString(reencoded);

    destination.clear();

    BOOST_CHECK(fastString == reencoded);
  }
}

