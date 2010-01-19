// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>

#include <Codecs/XMLTemplateParser.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/Encoder.h>
#include <Codecs/Decoder.h>
#include <Codecs/DataDestinationString.h>
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
  void validateMessage1(Messages::Message & message)
  {
    BOOST_CHECK_EQUAL(message.getApplicationType(), "instrumentreferencedata");
    Messages::FieldCPtr value;

    //msg.addField(identity_timestamp, Messages::FieldUInt32::create(1));
    BOOST_CHECK(message.getField("timestamp", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 1);

    //msg.addField(identity_srcId, Messages::FieldUInt32::create(2));
    BOOST_CHECK(message.getField("srcId", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 2);

    //msg.addField(identity_seqNum, Messages::FieldUInt32::create(3));
    BOOST_CHECK(message.getField("seqNum", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 3);

    //msg.addField(identity_isix, Messages::FieldUInt32::create(4));
    BOOST_CHECK(message.getField("isix", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 4);

    //msg.addField(identity_isin, Messages::FieldAscii::create("isin"));
    BOOST_CHECK(message.getField("isin", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "isin");

    //msg.addField(identity_exchId, Messages::FieldAscii::create("exchId"));
    BOOST_CHECK(message.getField("exchId", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "exchId");

    //msg.addField(identity_instGrp, Messages::FieldAscii::create("instGrp"));
    BOOST_CHECK(message.getField("instGrp", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "instGrp");

    //msg.addField(identity_instTypCod, Messages::FieldAscii::create("instTypCod"));
    BOOST_CHECK(message.getField("instTypCod", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "instTypCod");

    //msg.addField(identity_currCode, Messages::FieldAscii::create("currCode"));
    BOOST_CHECK(message.getField("currCode", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "currCode");

    //msg.addField(identity_ticSiz, Messages::FieldDecimal::create(Decimal(123, -1)));
    BOOST_CHECK(message.getField("ticSiz", value));
    BOOST_CHECK_EQUAL(value->toDecimal(), Decimal(123, -1));
    //msg.addField(identity_setId, Messages::FieldUInt32::create(5));
    BOOST_CHECK(message.getField("setId", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 5);

    BOOST_CHECK(message.getField("MDFeedTypes", value));
    const Messages::SequenceCPtr & mdft = value->toSequence();
    BOOST_CHECK_EQUAL(mdft->size(), 2);
    Messages::FieldSetCPtr seqEntry = (*mdft)[0];

    //entry->addField(identity_streamType, Messages::FieldAscii::create("streamType"));
    BOOST_CHECK( seqEntry->getField("streamType", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "streamType");

    //entry->addField(identity_streamService, Messages::FieldAscii::create("streamService"));
    BOOST_CHECK( seqEntry->getField("streamService", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "streamService");

    //entry->addField(identity_inetAddr, Messages::FieldAscii::create("inetAddr.com"));
    BOOST_CHECK( seqEntry->getField("inetAddr", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "inetAddr.com");

    //entry->addField(identity_port, Messages::FieldUInt32::create(2222));
    BOOST_CHECK( seqEntry->getField("port", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 2222);

    //entry->addField(identity_mktDepth, Messages::FieldUInt32::create(10));
    BOOST_CHECK( seqEntry->getField("mktDepth", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 10);

    // optional field omitted
    BOOST_CHECK(!seqEntry->getField("mdBookType", value));

    seqEntry = (*mdft)[1];

    //entry->addField(identity_streamType, Messages::FieldAscii::create("streamType2"));
    BOOST_CHECK( seqEntry->getField("streamType", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "streamType2");

    //entry->addField(identity_streamService, Messages::FieldAscii::create("streamService2"));
    BOOST_CHECK( seqEntry->getField("streamService", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "streamService2");

    //entry->addField(identity_inetAddr, Messages::FieldAscii::create("inetAddr.org"));
    BOOST_CHECK( seqEntry->getField("inetAddr", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "inetAddr.org");

    //entry->addField(identity_port, Messages::FieldUInt32::create(2224));
    BOOST_CHECK( seqEntry->getField("port", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 2224);
    // optional field omitted
    BOOST_CHECK(!seqEntry->getField("mktDepth", value));
    //entry->addField(identity_mdBookType, Messages::FieldUInt32::create(3));
    BOOST_CHECK(seqEntry->getField("mdBookType", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 3);
  }
}

BOOST_AUTO_TEST_CASE(testRoundTripSequenceNoPMAP)
{
  // Template from Issue #1
  std::string theTemplate =
    "<templates>"
    "  <template name=\"InstrumentReferenceData\" id=\"3\">"
    "    <typeRef name=\"instrumentreferencedata\"/>"
    "    <uInt32 name=\"timestamp\" id=\"52\">"
    "      <delta/>"
    "    </uInt32>"
    "    <uInt32 name=\"srcId\" id=\"50\">"
    "      <copy/>"
    "    </uInt32>"
    "    <uInt32 name=\"seqNum\" id=\"34\">"
    "      <increment value=\"1\"/>"
    "    </uInt32>"
    "    <uInt32 name=\"isix\" id=\"48\">"
    "      <delta/>"
    "    </uInt32>"
    "    <string name=\"isin\" id=\"455\">"
    "      <delta/>"
    "    </string>"
    "    <string name=\"exchId\" id=\"207\">"
    "      <copy/>"
    "    </string>"
    "    <string name=\"instGrp\" id=\"1151\">"
    "      <copy/>"
    "    </string>"
    "    <string name=\"instTypCod\" id=\"461\">"
    "      <copy/>"
    "    </string>"
    "    <string name=\"currCode\" id=\"15\">"
    "      <copy/>"
    "    </string>"
    "    <decimal name=\"ticSiz\" id=\"969\">"
    "      <delta/>"
    "    </decimal>"
    "    <uInt32 name=\"setId\" id=\"TBD\">"
    "      <copy/>"
    "    </uInt32>"
    "    <sequence name=\"MDFeedTypes\">"
    "      <length name=\"noOfStreams\" id=\"1141\"/>"
    "      <string name=\"streamType\" id=\"1022\"/>"
    "      <string name=\"streamService\"/>"
    "      <string name=\"inetAddr\">"
    "        <delta/>"
    "      </string>"
    "      <uInt32 name=\"port\" id=\"TBD\">"
    "        <delta/>"
    "      </uInt32>"
    "      <uInt32 name=\"mktDepth\" id=\"264\" presence=\"optional\"/>"
    "      <uInt32 name=\"mdBookType\" id=\"1021\" presence=\"optional\"/>"
    "    </sequence>"
    "  </template>"
    "</templates>";

  Codecs::XMLTemplateParser parser;
  std::stringstream templateStream(theTemplate);
  Codecs::TemplateRegistryPtr templateRegistry =
    parser.parse(templateStream);

  BOOST_CHECK(templateRegistry);
  BOOST_CHECK_EQUAL(templateRegistry->maxFieldCount(), 12);

  Messages::FieldIdentityCPtr identity_timestamp = new Messages::FieldIdentity("timestamp");
  Messages::FieldIdentityCPtr identity_srcId = new Messages::FieldIdentity("srcId");
  Messages::FieldIdentityCPtr identity_seqNum = new Messages::FieldIdentity("seqNum");
  Messages::FieldIdentityCPtr identity_isix = new Messages::FieldIdentity("isix");
  Messages::FieldIdentityCPtr identity_isin = new Messages::FieldIdentity("isin");
  Messages::FieldIdentityCPtr identity_exchId = new Messages::FieldIdentity("exchId");
  Messages::FieldIdentityCPtr identity_instGrp = new Messages::FieldIdentity("instGrp");
  Messages::FieldIdentityCPtr identity_instTypCod = new Messages::FieldIdentity("instTypCod");
  Messages::FieldIdentityCPtr identity_currCode = new Messages::FieldIdentity("currCode");
  Messages::FieldIdentityCPtr identity_ticSiz = new Messages::FieldIdentity("ticSiz");
  Messages::FieldIdentityCPtr identity_setId = new Messages::FieldIdentity("setId");
  Messages::FieldIdentityCPtr identity_MDFeedTypes = new Messages::FieldIdentity("MDFeedTypes");
// We don't need a fieldIdentity for the length field.
//  Messages::FieldIdentityCPtr identity_noOfStreams = new Messages::FieldIdentity("noOfStreams");
  Messages::FieldIdentityCPtr identity_streamType = new Messages::FieldIdentity("streamType");
  Messages::FieldIdentityCPtr identity_streamService = new Messages::FieldIdentity("streamService");
  Messages::FieldIdentityCPtr identity_inetAddr = new Messages::FieldIdentity("inetAddr");
  Messages::FieldIdentityCPtr identity_port = new Messages::FieldIdentity("port");
  Messages::FieldIdentityCPtr identity_mktDepth = new Messages::FieldIdentity("mktDepth");
  Messages::FieldIdentityCPtr identity_mdBookType = new Messages::FieldIdentity("mdBookType");

  Messages::MessagePtr msg(new Messages::Message(templateRegistry->maxFieldCount()));

//   <uInt32 name=\"timestamp\" id=\"52\"><delta/></uInt32>"
  msg->addField(identity_timestamp, Messages::FieldUInt32::create(1));
//   <uInt32 name=\"srcId\" id=\"50\"><copy/></uInt32>"
  msg->addField(identity_srcId, Messages::FieldUInt32::create(2));
//   <uInt32 name=\"seqNum\" id=\"34\"><increment value=\"1\"/></uInt32>"
  msg->addField(identity_seqNum, Messages::FieldUInt32::create(3));
//   <uInt32 name=\"isix\" id=\"48\"><delta/></uInt32>"
  msg->addField(identity_isix, Messages::FieldUInt32::create(4));
//   <string name=\"isin\" id=\"455\"><delta/></string>"
  msg->addField(identity_isin, Messages::FieldAscii::create("isin"));
//   <string name=\"exchId\" id=\"207\"><copy/></string>"
  msg->addField(identity_exchId, Messages::FieldAscii::create("exchId"));
//   <string name=\"instGrp\" id=\"1151\"><copy/></string>"
  msg->addField(identity_instGrp, Messages::FieldAscii::create("instGrp"));
//   <string name=\"instTypCod\" id=\"461\"><copy/></string>"
  msg->addField(identity_instTypCod, Messages::FieldAscii::create("instTypCod"));
//   <string name=\"currCode\" id=\"15\"><copy/></string>"
  msg->addField(identity_currCode, Messages::FieldAscii::create("currCode"));
//   <decimal name=\"ticSiz\" id=\"969\"><delta/></decimal>"
  msg->addField(identity_ticSiz, Messages::FieldDecimal::create(Decimal(123, -1)));
//   <uInt32 name=\"setId\" id=\"TBD\"><copy/></uInt32>"
  msg->addField(identity_setId, Messages::FieldUInt32::create(5));

//   <sequence name=\"MDFeedTypes\">"
//     <length name=\"noOfStreams\" id=\"1141\"/>"
  Messages::FieldIdentityPtr lengthIdentity(new Messages::FieldIdentity("noOfStreams",""));
  Messages::FieldIdentityCPtr cLengthIdentity(lengthIdentity);
  lengthIdentity->setId("1141");
  Messages::SequencePtr sequence_MDFeedTypes(new Messages::Sequence(cLengthIdentity, 2));
  Messages::FieldSetPtr entry(new Messages::FieldSet(6)); // todo Hardcoded 6?

//     <string name=\"streamType\" id=\"1022\"/>"
  entry->addField(identity_streamType, Messages::FieldAscii::create("streamType"));
//     <string name=\"streamService\"/>"
  entry->addField(identity_streamService, Messages::FieldAscii::create("streamService"));
//     <string name=\"inetAddr\"><delta/></string>"
  entry->addField(identity_inetAddr, Messages::FieldAscii::create("inetAddr.com"));
//     <uInt32 name=\"port\" id=\"TBD\"><delta/></uInt32>"
  entry->addField(identity_port, Messages::FieldUInt32::create(2222));
//     <uInt32 name=\"mktDepth\" id=\"264\" presence=\"optional\"/>"
  entry->addField(identity_mktDepth, Messages::FieldUInt32::create(10));
//     <uInt32 name=\"mdBookType\" id=\"1021\" presence=\"optional\"/>"
// optional field omitted

  sequence_MDFeedTypes->addEntry(Messages::FieldSetCPtr(entry));

  entry.reset(new Messages::FieldSet(6));
//     <string name=\"streamType\" id=\"1022\"/>"
  entry->addField(identity_streamType, Messages::FieldAscii::create("streamType2"));
//     <string name=\"streamService\"/>"
  entry->addField(identity_streamService, Messages::FieldAscii::create("streamService2"));
//     <string name=\"inetAddr\"><delta/></string>"
  entry->addField(identity_inetAddr, Messages::FieldAscii::create("inetAddr.org"));
//     <uInt32 name=\"port\" id=\"TBD\"><delta/></uInt32>"
  entry->addField(identity_port, Messages::FieldUInt32::create(2224));
//     <uInt32 name=\"mktDepth\" id=\"264\" presence=\"optional\"/>"
// optional field omitted
//     <uInt32 name=\"mdBookType\" id=\"1021\" presence=\"optional\"/>"
  entry->addField(identity_mdBookType, Messages::FieldUInt32::create(3));

  sequence_MDFeedTypes->addEntry(Messages::FieldSetCPtr(entry));

  msg->addField(identity_MDFeedTypes, Messages::FieldSequence::create(sequence_MDFeedTypes));

  Codecs::Encoder encoder(templateRegistry);
  Codecs::DataDestinationString destination;
  template_id_t templId = 3; // from the XML above
  encoder.encodeMessage(destination, templId, *msg);
  const std::string & fastString = destination.getValue();

  Codecs::Decoder decoder(templateRegistry);
  Codecs::DataSourceString source(fastString);
  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);
  decoder.decodeMessage(source, builder);

  Messages::Message & msgOut(consumer.message());

  validateMessage1(msgOut);

  // wanna see it again?
  encoder.encodeMessage(destination, templId, msgOut);
  const std::string reencoded = destination.getValue();

  BOOST_CHECK(fastString == reencoded);
}
