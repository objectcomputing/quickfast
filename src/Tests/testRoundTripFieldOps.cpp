// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>

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
  const std::string theTemplates =
    "<templates>"
    "  <template name=\"template_1\" id=\"1\">"
    "    <typeRef name=\"data_1\"/>"
    "    <uInt32 name=\"field_1\">"                           // integer required delta
    "      <delta/>"
    "    </uInt32>"
    "    <uInt32 name=\"field_2\">"                           // integer required copy
    "      <copy/>"
    "    </uInt32>"
    "    <uInt32 name=\"field_3\">"                           // integer required increment
    "      <increment/>"
    "    </uInt32>"
    "    <uInt32 name=\"field_4\" presence=\"optional\">"     // integer optional delta
    "      <delta/>"
    "    </uInt32>"
    "    <uInt32 name=\"field_5\" presence=\"optional\">"     // integer optional copy
    "      <copy/>"
    "    </uInt32>"
    "    <uInt32 name=\"field_6\" presence=\"optional\">"     // integer optional increment
    "      <increment/>"
    "    </uInt32>"
    "    <string name=\"field_7\">"                           // string required delta
    "      <delta/>"
    "    </string>"
    "    <string name=\"field_8\">"                           // string required copy
    "      <copy/>"
    "    </string>"
    "    <string name=\"field_9\" presence=\"optional\">"     // string optional delta
    "      <delta/>"
    "    </string>"
    "    <string name=\"field_10\" presence=\"optional\">"     // string optional copy
    "      <copy/>"
    "    </string>"
    "    <decimal name=\"field_11\">"                          // decimal required delta
    "      <delta/>"
    "    </decimal>"
    "    <decimal name=\"field_12\">"                          // decimal required copy
    "      <copy/>"
    "    </decimal>"
    "    <decimal name=\"field_13\" presence=\"optional\">"    // decimal optional delta
    "      <delta/>"
    "    </decimal>"
    "    <decimal name=\"field_14\" presence=\"optional\">"    // decimal optional copy
    "      <copy/>"
    "    </decimal>"
    "  </template>"

    "  <template name=\"MDAdd\" id=\"5000\">"
    "    <uInt32 name=\"MDUpdateAction\" id=\"269\"><constant value=\"0\"/></uInt32>"
    "    <string name=\"MDEntryID\" id=\"278\"/>"
    "    <decimal name=\"MDEntryPx\" id=\"270\"><delta/></decimal>"
    "    <uInt32 name=\"MDEntrySize\" id=\"271\"><copy/></uInt32>"
    "    <string name=\"Currency\" id=\"15\" presence=\"optional\"/>"
    "  </template>"
    "  <template name=\"MDDel\" id=\"5001\">"
    "    <uInt32 name=\"MDUpdateAction\" id=\"269\"><constant value=\"2\"/></uInt32>"
    "    <string name=\"MDEntryID\" id=\"278\"/>"
    "  </template>"
    "  <template name=\"MarketDataSnapshot\" id=\"20\">"
    "    <group name=\"MDUpdateAdd\" presence=\"optional\">"
    "      <templateRef name=\"MDAdd\"/>"
    "    </group>"
    "    <group name=\"MDUpdateDel\" presence=\"optional\">"
    "      <templateRef name=\"MDDel\"/>"
    "    </group>"
    "  </template>"
    "</templates>";


  Messages::FieldIdentity identity_1("field_1");
  Messages::FieldIdentity identity_2("field_2");
  Messages::FieldIdentity identity_3("field_3");
  Messages::FieldIdentity identity_4("field_4");
  Messages::FieldIdentity identity_5("field_5");
  Messages::FieldIdentity identity_6("field_6");
  Messages::FieldIdentity identity_7("field_7");
  Messages::FieldIdentity identity_8("field_8");
  Messages::FieldIdentity identity_9("field_9");
  Messages::FieldIdentity identity_10("field_10");
  Messages::FieldIdentity identity_11("field_11");
  Messages::FieldIdentity identity_12("field_12");
  Messages::FieldIdentity identity_13("field_13");
  Messages::FieldIdentity identity_14("field_14");

  Messages::FieldIdentity MDUpdateAdd("MDUpdateAdd");
  Messages::FieldIdentity MDUpdateDel("MDUpdateDel");

  Messages::FieldIdentity MDUpdateAction("MDUpdateAction");
  Messages::FieldIdentity MDEntryID("MDEntryID");
  Messages::FieldIdentity MDEntryPx("MDEntryPx");
  Messages::FieldIdentity MDEntrySize("MDEntrySize");
  Messages::FieldIdentity Currency("Currency");


  bool compareMessages(Messages::Message & lhs, Messages::Message & rhs)
  {
    std::stringstream reason;
    if(!lhs.equals(rhs, reason))
    {
      std::cerr << "Reason: " << reason.str() << std::endl;
      return false;
    }
    return true;
  }
}

BOOST_AUTO_TEST_CASE(testRoundTripFieldOperators)
{
  Codecs::XMLTemplateParser parser;
  std::stringstream templateStream(theTemplates);
  Codecs::TemplateRegistryPtr templateRegistry =
    parser.parse(templateStream);

  BOOST_CHECK(templateRegistry);
  BOOST_CHECK_EQUAL(templateRegistry->maxFieldCount(), 14);

  Messages::MessagePtr msg1(new Messages::Message(templateRegistry->maxFieldCount()));
  msg1->addField(identity_1, Messages::FieldUInt32::create(1));
  msg1->addField(identity_2, Messages::FieldUInt32::create(2));
  msg1->addField(identity_3, Messages::FieldUInt32::create(3));
  msg1->addField(identity_4, Messages::FieldUInt32::create(4));
  msg1->addField(identity_5, Messages::FieldUInt32::create(5));
  msg1->addField(identity_6, Messages::FieldUInt32::create(6));
  msg1->addField(identity_7, Messages::FieldAscii::create("field_7"));
  msg1->addField(identity_8, Messages::FieldAscii::create("field_8"));
  msg1->addField(identity_9, Messages::FieldAscii::create("field_9"));
  msg1->addField(identity_10, Messages::FieldAscii::create("field_10"));
  msg1->addField(identity_11, Messages::FieldDecimal::create(123456, 1));
  msg1->addField(identity_12, Messages::FieldDecimal::create(123456, 2));
  msg1->addField(identity_13, Messages::FieldDecimal::create(123456, 3));
  msg1->addField(identity_14, Messages::FieldDecimal::create(123456, 4));


  Messages::MessagePtr msg2(new Messages::Message(templateRegistry->maxFieldCount()));
  msg2->addField(identity_1, Messages::FieldUInt32::create(2));
  msg2->addField(identity_2, Messages::FieldUInt32::create(3));
  msg2->addField(identity_3, Messages::FieldUInt32::create(4));
  msg2->addField(identity_4, Messages::FieldUInt32::create(5));
  msg2->addField(identity_5, Messages::FieldUInt32::create(6));
  msg2->addField(identity_6, Messages::FieldUInt32::create(7));
  msg2->addField(identity_7, Messages::FieldAscii::create("field_7x"));
  msg2->addField(identity_8, Messages::FieldAscii::create("field_8x"));
  msg2->addField(identity_9, Messages::FieldAscii::create("field_9x"));
  msg2->addField(identity_10, Messages::FieldAscii::create("field_10x"));
  msg2->addField(identity_11, Messages::FieldDecimal::create(123450, 2));
  msg2->addField(identity_12, Messages::FieldDecimal::create(123450, 3));
  msg2->addField(identity_13, Messages::FieldDecimal::create(123450, 2));
  msg2->addField(identity_14, Messages::FieldDecimal::create(123450, 1));

  Messages::MessagePtr msg3(new Messages::Message(templateRegistry->maxFieldCount()));
  msg3->addField(identity_1, Messages::FieldUInt32::create(2));
  msg3->addField(identity_2, Messages::FieldUInt32::create(3));
  msg3->addField(identity_3, Messages::FieldUInt32::create(4));
  msg3->addField(identity_6, Messages::FieldUInt32::create(7));
  msg3->addField(identity_7, Messages::FieldAscii::create("field_7x"));
  msg3->addField(identity_8, Messages::FieldAscii::create("field_8x"));
  msg3->addField(identity_11, Messages::FieldDecimal::create(123450, 2));
  msg3->addField(identity_12, Messages::FieldDecimal::create(123450, 3));

  Codecs::Encoder encoder(templateRegistry);
  template_id_t templId = 1; // from the XML above

  BOOST_TEST_CHECKPOINT("encode msg1A");
  Codecs::DataDestination destination1A;
  encoder.encodeMessage(destination1A, templId, *msg1);
  std::string fast1A;
  destination1A.toString(fast1A);

  BOOST_TEST_CHECKPOINT("encode msg1B");
  Codecs::DataDestination destination1B;
  encoder.encodeMessage(destination1B, templId, *msg1);
  std::string fast1B;
  destination1B.toString(fast1B);

  BOOST_TEST_CHECKPOINT("encode msg1C");
  Codecs::DataDestination destination1C;
  encoder.encodeMessage(destination1C, templId, *msg1);
  std::string fast1C;
  destination1C.toString(fast1C);

  BOOST_TEST_CHECKPOINT("encode msg2A");
  Codecs::DataDestination destination2A;
  encoder.encodeMessage(destination2A, templId, *msg2);
  std::string fast2A;
  destination2A.toString(fast2A);

  BOOST_TEST_CHECKPOINT("encode msg2B");
  Codecs::DataDestination destination2B;
  encoder.encodeMessage(destination2B, templId, *msg2);
  std::string fast2B;
  destination2B.toString(fast2B);

  BOOST_TEST_CHECKPOINT("encode msg3");
  Codecs::DataDestination destination3;
  encoder.encodeMessage(destination3, templId, *msg3);
  std::string fast3;
  destination3.toString(fast3);

  Codecs::Decoder decoder(templateRegistry);

  BOOST_TEST_CHECKPOINT("decode msg1A");
  Codecs::DataSourceString source1A(fast1A);
  Codecs::SingleMessageConsumer consumer1A;
  Codecs::GenericMessageBuilder builder1A(consumer1A);
  decoder.decodeMessage(source1A, builder1A);

  BOOST_TEST_CHECKPOINT("decode msg1B");
  Codecs::DataSourceString source1B(fast1B);
  Codecs::SingleMessageConsumer consumer1B;
  Codecs::GenericMessageBuilder builder1B(consumer1B);
  decoder.decodeMessage(source1B, builder1B);

  BOOST_TEST_CHECKPOINT("decode msg1C");
  Codecs::DataSourceString source1C(fast1C);
  Codecs::SingleMessageConsumer consumer1C;
  Codecs::GenericMessageBuilder builder1C(consumer1C);
  decoder.decodeMessage(source1C, builder1C);

  BOOST_TEST_CHECKPOINT("decode msg2A");
  Codecs::DataSourceString source2A(fast2A);
  Codecs::SingleMessageConsumer consumer2A;
  Codecs::GenericMessageBuilder builder2A(consumer2A);
  decoder.decodeMessage(source2A, builder2A);

  BOOST_TEST_CHECKPOINT("decode msg2B");
  Codecs::DataSourceString source2B(fast2B);
  Codecs::SingleMessageConsumer consumer2B;
  Codecs::GenericMessageBuilder builder2B(consumer2B);
  decoder.decodeMessage(source2B, builder2B);

  BOOST_TEST_CHECKPOINT("decode msg3");
  Codecs::DataSourceString source3(fast3);
  Codecs::SingleMessageConsumer consumer3;
  Codecs::GenericMessageBuilder builder3(consumer3);
  decoder.decodeMessage(source3, builder3);

  BOOST_CHECK(fast1A != fast1B);
  BOOST_CHECK(fast1B == fast1C);
  BOOST_CHECK(fast2A != fast2B);


  BOOST_CHECK(compareMessages(*msg1, consumer1A.message()));
  BOOST_CHECK(compareMessages(consumer1A.message(), consumer1B.message()));
  BOOST_CHECK(compareMessages(consumer1A.message(), consumer1C.message()));
  BOOST_CHECK(compareMessages(consumer2A.message(), consumer2B.message()));
}

BOOST_AUTO_TEST_CASE(testRoundTripOptionalTemplateRefs)
{
  Codecs::XMLTemplateParser parser;
  std::stringstream templateStream(theTemplates);
  Codecs::TemplateRegistryPtr templateRegistry =
    parser.parse(templateStream);

  Messages::MessagePtr addMessage(new Messages::Message(templateRegistry->maxFieldCount()));
  Messages::MessagePtr addFlatMessage(new Messages::Message(templateRegistry->maxFieldCount()));

  Messages::GroupPtr addGroup(new Messages::Group(6));
  addGroup->addField(MDUpdateAction, Messages::FieldUInt32::create(0));
  addFlatMessage->addField(MDUpdateAction, Messages::FieldUInt32::create(0));
  addGroup->addField(MDEntryID, Messages::FieldAscii::create("act1"));
  addFlatMessage->addField(MDEntryID, Messages::FieldAscii::create("act1"));
  addGroup->addField(MDEntryPx, Messages::FieldDecimal::create(Decimal(2,5)));
  addFlatMessage->addField(MDEntryPx, Messages::FieldDecimal::create(Decimal(2,5)));
  addGroup->addField(MDEntrySize, Messages::FieldUInt32::create(100));
  addFlatMessage->addField(MDEntrySize, Messages::FieldUInt32::create(100));
  addGroup->addField(Currency, Messages::FieldAscii::create("USD"));
  addFlatMessage->addField(Currency, Messages::FieldAscii::create("USD"));
  addMessage->addField(MDUpdateAdd, Messages::FieldGroup::create(addGroup));

  Messages::MessagePtr delMessage(new Messages::Message(templateRegistry->maxFieldCount()));
  Messages::MessagePtr delFlatMessage(new Messages::Message(templateRegistry->maxFieldCount()));

  Messages::GroupPtr delGroup(new Messages::Group(6));
  delGroup->addField(MDUpdateAction, Messages::FieldUInt32::create(2));
  delFlatMessage->addField(MDUpdateAction, Messages::FieldUInt32::create(2));
  delGroup->addField(MDEntryID, Messages::FieldAscii::create("act1"));
  delFlatMessage->addField(MDEntryID, Messages::FieldAscii::create("act1"));
  delMessage->addField(MDUpdateDel, Messages::FieldGroup::create(delGroup));

  Codecs::Encoder encoder(templateRegistry);
  template_id_t templId = 20; // from the XML above

  BOOST_TEST_CHECKPOINT("encode addMessage");
  Codecs::DataDestination addDestination;
  encoder.encodeMessage(addDestination, templId, *addMessage);
  std::string encodedAdd;
  addDestination.toString(encodedAdd);

  BOOST_TEST_CHECKPOINT("encode delMessage");
  Codecs::DataDestination delDestination;
  encoder.encodeMessage(delDestination, templId, *delMessage);
  std::string encodedDel;
  delDestination.toString(encodedDel);

  Codecs::Decoder decoder(templateRegistry);

  BOOST_TEST_CHECKPOINT("decode addMessage");
  Codecs::DataSourceString addSource(encodedAdd);
  Codecs::SingleMessageConsumer consumerAdd;
  Codecs::GenericMessageBuilder builderAdd(consumerAdd);
  decoder.decodeMessage(addSource, builderAdd);

  BOOST_CHECK(compareMessages(*addFlatMessage, consumerAdd.message()));
  BOOST_TEST_CHECKPOINT("decode delMessage");
  Codecs::DataSourceString delSource(encodedDel);
  Codecs::SingleMessageConsumer consumerDel;
  Codecs::GenericMessageBuilder builderDel(consumerDel);
  decoder.decodeMessage(delSource, builderDel);

  BOOST_CHECK(compareMessages(*delFlatMessage, consumerDel.message()));
}
