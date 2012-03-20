// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>
#include <Codecs/FieldInstructionAscii.h>
#include <Codecs/FieldInstructionUtf8.h>
#include <Codecs/FieldInstructionByteVector.h>
#include <Codecs/FieldInstructionInt8.h>
#include <Codecs/FieldInstructionUInt8.h>
#include <Codecs/FieldInstructionInt16.h>
#include <Codecs/FieldInstructionUInt16.h>
#include <Codecs/FieldInstructionInt32.h>
#include <Codecs/FieldInstructionUInt32.h>
#include <Codecs/FieldInstructionInt64.h>
#include <Codecs/FieldInstructionUInt64.h>
#include <Codecs/FieldInstructionDecimal.h>
#include <Codecs/FieldInstructionTemplateRef.h>
#include <Codecs/FieldInstructionGroup.h>
#include <Codecs/FieldInstructionSequence.h>
#include <Codecs/FieldOpConstant.h>
#include <Codecs/FieldOpCopy.h>
#include <Codecs/FieldOpDefault.h>
#include <Codecs/FieldOpDelta.h>
#include <Codecs/FieldOpIncrement.h>
#include <Codecs/FieldOpNop.h>
#include <Codecs/FieldOpTail.h>
#include <Codecs/DataSourceString.h>
#include <Codecs/DataSourceStream.h>
#include <Codecs/DataDestination.h>
#include <Codecs/SingleMessageConsumer.h>
#include <Codecs/GenericMessageBuilder.h>
#include <Codecs/Decoder.h>
#include <Codecs/DictionaryIndexer.h>
#include <Codecs/TemplateRegistry.h>
#include <Messages/FieldSet.h>
#include <Messages/FieldUInt32.h>
#include <Tests/FieldInstructionMock.h>

using namespace QuickFAST;

BOOST_AUTO_TEST_CASE(testFieldOperationDispatch)
{
  // the Field Operator does half of a double dispatch
  // The tests below make sure the dispatch gets to the
  // right place.

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // create an "empty" presence map.
  Codecs::PresenceMap pmap(1);

  Tests::FieldInstructionMock field;
  field.setPresence(false);
  // NOP operator is the default
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));

  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 0);

  // create enough infrastructure to call decode
  Codecs::DataSourceString source("");
  Codecs::Decoder decoder(registry);
  Messages::FieldSet fieldSet1(10);
  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);
  // and encode
  Codecs::DataDestination destination;
  Codecs::Encoder encoder(registry);


  field.decode(source, pmap, decoder, builder);
  BOOST_CHECK_EQUAL(field.readDecodeNop(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeNop(), 1);
  BOOST_CHECK(field.isClear());

  Codecs::FieldOpPtr constantOp(new Codecs::FieldOpConstant);
  constantOp->setValue("1");
  field.setFieldOp(constantOp);
  field.decode(source, pmap, decoder, builder);//fieldSet1));
  BOOST_CHECK_EQUAL(field.readDecodeConstant(), 1);
  BOOST_CHECK_EQUAL(field.readInterpretValue(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeConstant(), 1);
  BOOST_CHECK(field.isClear());

  Codecs::FieldOpPtr defaultOp(new Codecs::FieldOpDefault);
  defaultOp->setValue("1");
  field.setFieldOp(defaultOp);
  field.decode(source, pmap, decoder, builder);//fieldSet1));
  BOOST_CHECK_EQUAL(field.readDecodeDefault(), 1);
  BOOST_CHECK_EQUAL(field.readInterpretValue(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeDefault(), 1);
  BOOST_CHECK(field.isClear());

  Codecs::FieldOpPtr copyOp(new Codecs::FieldOpCopy);
  copyOp->setValue("1");
  field.setFieldOp(copyOp);
  field.decode(source, pmap, decoder, builder);//fieldSet1));
  BOOST_CHECK_EQUAL(field.readDecodeCopy(), 1);
  BOOST_CHECK_EQUAL(field.readInterpretValue(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeCopy(), 1);
  BOOST_CHECK(field.isClear());

  Codecs::FieldOpPtr deltaOp(new Codecs::FieldOpDelta);
  deltaOp->setValue("1");
  field.setFieldOp(deltaOp);
  field.decode(source, pmap, decoder, builder);//fieldSet1));
  BOOST_CHECK_EQUAL(field.readDecodeDelta(), 1);
  BOOST_CHECK_EQUAL(field.readInterpretValue(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeDelta(), 1);
  BOOST_CHECK(field.isClear());

  Codecs::FieldOpPtr incrementOp(new Codecs::FieldOpIncrement);
  incrementOp->setValue("1");
  field.setFieldOp(incrementOp);
  field.decode(source, pmap, decoder, builder);//fieldSet1));
  BOOST_CHECK_EQUAL(field.readDecodeIncrement(), 1);
  BOOST_CHECK_EQUAL(field.readInterpretValue(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeIncrement(), 1);
  BOOST_CHECK(field.isClear());

  Codecs::FieldOpPtr tailOp(new Codecs::FieldOpTail);
  tailOp->setValue("1");
  field.setFieldOp(tailOp);
  field.decode(source, pmap, decoder, builder);//fieldSet1));
  BOOST_CHECK_EQUAL(field.readDecodeTail(), 1);
  BOOST_CHECK_EQUAL(field.readInterpretValue(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeTail(), 1);
  BOOST_CHECK(field.isClear());

}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_1_1)
{
// Constant Operator Example – Mandatory Unsigned Integer
// <uInt32 id="1" presence="mandatory" name="Flag"> <constant value="0"/></uInt32>
// Input   Prior   Encoded   Pmap      FAST Hex/Binary
// 0       N/A     None      No        None
// 99      N/A     Error     Error     Error
// None    N/A     Error     Error     Error

  const char testData[] = "";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;

  Codecs::PresenceMap pmap(1);
  Codecs::FieldInstructionUInt32 field("Flag", "");
  field.setPresence(true);
  Codecs::FieldOpPtr fieldOp(new Codecs::FieldOpConstant);
  fieldOp->setValue("0");
  field.setFieldOp(fieldOp);
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));

  // verify no presence map needed
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 0);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message & fieldSet1 = consumer.message();

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet1.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  // the value should be the constant zero
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 0);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);

  // check the error condition: constant value doesn't match
  // Note: removed the checks for mandatory constant fields.  It added busywork for the application
  //       without adding value.
  // Messages::FieldSet fieldSet2(10);
  // Messages::FieldCPtr dataField = Messages::FieldUInt32::create(99);
  // fieldSet2.addField(field.getIdentity(), dataField);
  // BOOST_CHECK_THROW(field.encode(destination, pmapResult, encoder, fieldSet2), EncodingError);
  // check the error condition: missing mandatory field
  // Messages::FieldSet fieldSet3(10);
  // BOOST_CHECK_THROW(field.encode(destination, pmapResult, encoder, fieldSet3), EncodingError);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_1_2)
{
//Constant Operator Example – Optional Unsigned Integer
// <uInt32 id="1" presence="optional" name="Flag"> <constant value="0"/> </uInt32>
// Input  Prior Encoded Pmap FAST Hex/Binary
// 0      N/A   None    1    None
// None   N/A   None    0    None
  const char testData[] = "";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(2);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.rewind();

  Codecs::FieldInstructionInt32 field("Flag", "");
  field.setPresence(false);
  Codecs::FieldOpPtr fieldOp(new Codecs::FieldOpConstant);
  fieldOp->setValue("0");
  field.setFieldOp(fieldOp);
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));

  // verify 1 presence map bit needed
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 1);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);
  builder.startMessage("UNIT_TEST", "", 10);

  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet1.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::INT32));
  // the value should be the constant zero
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 0);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // test b: pmap = 0
  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message & fieldSet2 = consumer.message();

  // Check to be sure no data was generated
  BOOST_CHECK(fieldSet2.begin() == fieldSet2.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_2_1)
{
  // Default Operator Example – Mandatory Unsigned Integer
  // <uInt32 id="1" presence="mandatory" name="Flag"> <default value="0"/> </uInt32>// Input  Prior Encoded Pmap FAST Hex/Binary
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // 0      N/A   None    0     None
  // 1      N/A   1       1     0x81: 1000 0001
  const char testData[] = "\x81";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(2);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.rewind();

  Codecs::FieldInstructionInt32 field("Flag", "");
  field.setPresence(true);
  Codecs::FieldOpPtr fieldOp(new Codecs::FieldOpDefault);
  fieldOp->setValue("0");
  field.setFieldOp(fieldOp);
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));

  // verify 1 presence map bit needed
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 1);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  // test a: pmap = 0
  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  // should generate 1 field
  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet1.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::INT32));
  // the value should be the constant zero
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 0);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // test b: pmap = 1

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message & fieldSet2 = consumer.message();

  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet2.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::INT32));
  // the value should be the explicit 1
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 1);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet2.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_2_2)
{
  // Default Operator Example for NULL– Optional Unsigned Integer
  // <uInt32 id="1" presence="optional" name="Flag"> <default/> </uInt32>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // none   N/A   None    0     None
  const char testData[] = "";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(1);
  pmap.setNextField(false);
  pmap.rewind();

  Codecs::FieldInstructionInt32 field("Flag", "");
  field.setPresence(false);
  field.setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpDefault));
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  // verify 1 presence map bit needed
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 1);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message & fieldSet1 = consumer.message();
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_3_1)
{
  // Copy Operator Example – Mandatory String
  // <string id="1" presence="mandatory" name="Flag"> <copy/> </string>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // CME    None  CME     1     0x43 0x4d 0xc5 /01000011 01001101 11000101
  // CME    CME   None    0     none
  // ISE    CME   ISE     1     0x49 0x53 0xC5 /01001001 01010011 11000101


  const char testData[] = "\x43\x4d\xc5\x49\x53\xC5";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(3);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.rewind();

  Codecs::FieldInstructionAscii field("Flag", "");
  field.setPresence(true);
  field.setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpCopy));
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));

  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 1);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet1.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "CME");

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet2.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "CME");

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet3.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "ISE");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_3_2)
{
  // Copy Operator Example for NULL – Optional String
  // <string id="1" presence="optional" name="Flag"> <copy/> </string>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // None   None  Null    1     0x80 / 10000000
  // None   Null  None    0     None
  // CME    Null  CME     1     0x43 0x4d 0xc5 / 01000011 01001101 11000101
  //
  // This is an error in the specification.
  // original, incorrect (see above)  const char testData[] = "\x80\x43\x4d\xc5";
  //
  // 6.3.5 of FAST 1.x.1 says:
  // When the value is not present in the input stream [...]
  //  If the field has optional presence and no initial value, the
  //  field is considered absent and the state of the previous value
  //  is changed to empty.
  //
  // The revised example:
  // Copy Operator Example for NULL – Optional String
  // <string id="1" presence="optional" name="Flag"> <copy/> </string>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // None   None  Null    0     None
  // None   Null  None    0     None
  // CME    Null  CME     1     0x43 0x4d 0xc5 / 01000011 01001101 11000101


  const char testData[] = "\x43\x4d\xc5";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(3);
// original, incorrect (see above)  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.rewind();

  Codecs::FieldInstructionAscii field("Flag", "");
  field.setPresence(false);
  field.setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpCopy));
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 1);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // check generated fields
  BOOST_CHECK_EQUAL(fieldSet1.size(), 0);
  BOOST_CHECK_EQUAL(fieldSet2.size(), 0);
  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);

  Messages::FieldSet::const_iterator pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "CME");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_4_1)
{
  // Increment Operator Example – Mandatory Unsigned Integer
  // <uInt32 id="1" presence="mandatory" name="Flag"> <increment value="1"/> </uInt32>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // 1      1     None    0     None
  // 2      1     None    0     None
  // 4      2     4       1     0x84 / 10000010
  // 5      4     None    0     None

  const char testData[] = "\x84";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(4);
  pmap.setNextField(false);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.rewind();

  Codecs::FieldInstructionUInt32 field("Flag", "");
  field.setPresence(true);
  Codecs::FieldOpPtr fieldOp(new Codecs::FieldOpIncrement);
  fieldOp->setValue("1");
  field.setFieldOp(fieldOp);
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 1);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet4(1);
  fieldSet4.swap(consumer.message());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 4 fields; one in each fieldset
  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 1);

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 2);

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 4);

  BOOST_CHECK_EQUAL(fieldSet4.size(), 1);
  pFieldEntry = fieldSet4.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 5);

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  field.encode(destination, pmapResult, encoder, fieldSet4);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_5_1)
{
  // Delta Operator Example – Mandatory Signed Integer
  // <int32 id="1" presence="mandatory" name="Price"> <delta/> </int32>
  // Input  Prior   Encoded PMap  FAST Hex/Binary
  // 942755 0       942755  n/a   0x39 0x45 0xa3 /00111001 01000101 10100011
  // 942750 942755  -5      n/a   0xfb / 11111011
  // 942745 942750  -5      n/a   0xfb / 11111011
  // 942745 942745  0       n/a   0x80 / 10000000

  const char testData[] = "\x39\x45\xa3\xfb\xfb\x80";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(1);

  Codecs::FieldInstructionUInt32 field("Price", "");
  field.setPresence(true);
  field.setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpDelta));
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 0);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet4(1);
  fieldSet4.swap(consumer.message());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 942755);

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 942750);

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 942745);

  BOOST_CHECK_EQUAL(fieldSet4.size(), 1);
  pFieldEntry = fieldSet4.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 942745);

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  field.encode(destination, pmapResult, encoder, fieldSet4);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_5_2)
{
  // Delta Operator Example – Mandatory Decimal
  // <decimal id="1" presence="mandatory" name="Price"> <delta/> </decimal>
  // No PMAP bits are used
  // Input    Prior     Encoded     FAST Hex/Binary
  //          Exp:Man   Exp:Man     Exp:Man
  // 9427.55  0:0       -2:942755   FE:39 45 A3/
  //                                11111110:00111001 01000101 10100011
  // 9427.51  -2:942755 0:-4        0x80:FC/10000000 11111100
  // 9427.46  -2:942751 0:-5        0x80:FB/10000000 11111011

  const char testData[] = "\xFE\x39\x45\xA3\x80\xFC\x80\xFB";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(1);

  Codecs::FieldInstructionDecimal field("Price", "");
  field.setPresence(true);
  field.setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpDelta));
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 0);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  Decimal expectA(942755,-2);
  BOOST_REQUIRE(pFieldEntry != fieldSet1.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::DECIMAL));
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectA);

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  Decimal expectB(942751,-2);
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::DECIMAL));
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectB);

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  Decimal expectC(942746,-2);
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::DECIMAL));
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectC);

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_5_3)
{
  // Delta Operator Example – Mandatory Decimal with Initial Value
  // <decimal id="1" presence="mandatory" name="Price"> <delta value="12000"/> </decimal>
  // No PMAP bits are used
  // Input    Prior     Encoded     FAST Hex/Binary
  //          Exp:Man   Exp:Man     Exp:Man
  // 12100    3:12      -2:1198     FE:09 AE/11111110:00001001 10101110
  // 12150    1:1210    0:5         80:85/10000000:10000101
  // 12200    1:1215    0:5         80:85/10000000:10000101

  const char testData[] = "\xFE\x09\xAE\x80\x85\x80\x85";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(1);

  Codecs::FieldInstructionDecimal field("Price", "");
  field.setPresence(true);
  Codecs::FieldOpPtr fieldOp(new Codecs::FieldOpDelta);
  fieldOp->setValue("12000");
  field.setFieldOp(fieldOp);
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 0);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  Decimal expectB(12100);
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::DECIMAL));
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectB);


  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  Decimal expectC(12150);
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::DECIMAL));
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectC);

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  Decimal expectD(12200);
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::DECIMAL));
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectD);

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_5_4)
{
  // Delta Operator Example – Mandatory String
  // <string id="1" presence="mandatory" name="Security"> <delta/> </string>
  // No PMAP bits are used
  // Input    Prior     Encoded     FAST Hex/Binary
  //                    Len:Value   Len/Value
  // GEH6     Empty     0:GEH6      80:47 45 48 B6
  //                                1000000:01000111 01000101 01001000 10110110
  // GEM6     GEH6      2:M6        82:4D B6 / 10000010 01001101 10110110
  // ESM6     GEM6      -2:ES       FD:45 D3 / 11111101 01000101 11010011
  // RSESM6   ESM6      -0:RS       FF:52 D3 / 11111111 01010010 11010011

  const char testData[] = "\x80\x47\x45\x48\xb6\x82\x4D\xB6\xFD\x45\xD3\xFF\x52\xD3";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(1);

  Codecs::FieldInstructionAscii field("Security", "");
  field.setPresence(true);
  field.setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpDelta));
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 0);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet4(1);
  fieldSet4.swap(consumer.message());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // check generated fields
  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet1.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "GEH6");

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "GEM6");

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "ESM6");

  BOOST_CHECK_EQUAL(fieldSet4.size(), 1);
  pFieldEntry = fieldSet4.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "RSESM6");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  field.encode(destination, pmapResult, encoder, fieldSet4);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAsciiTailMandatory)
{
  //  Not in spec
  // Tail Operator Example – Mandatory String
  // <string id="1" presence="mandatory" name="Security"> <tail/> </string>
  // Input    Prior   PMAP  Encoded     FAST Hex/Binary
  // GEH6     Empty   1     GEH6        47 45 48 B6
  //                                    01000111 01000101 01001000 10110110
  // GEM6     GEH6    1     M6          4D B6 / 01001101 10110110
  // GEM6     GEM6    0     None
  const char testData[] = "\x47\x45\x48\xb6\x4D\xB6";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(3);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.rewind();

  Codecs::FieldInstructionAscii field("Security", "");
  field.setPresence(true);
  field.setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpTail));
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 1);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // check generated fields
  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet1.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "GEH6");

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "GEM6");

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "GEM6");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(3);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_6) // SPEC ERROR: _3 s/b _1
{
  // Multiple Pmap Slot Example – Optional Positive Decimal with individual field operators

  // <decimal id="1" presence="optional" name="Value">
  //    <exponent> <copy/> </exponent>
  //    <mantissa> <copy/> </mantissa>
  // </decimal>
  //    Input    Exponent      Mantissa     PMAP   FAST Hex/Binary
  //             Value>Encode  Value>Encode        Exponent^Mantissa
  //[1] 9427.55  -2>-2         942755>942755 11    FE:39 45 A3 /
  //                                               11111110:00111001 01000101 10100011
  //[2] 9427.60  -2/None       942760>942760 01    None:39 45 A7 /
  //                                               :00111001 01000101 10100111
  //    None     NULL/NULL     None/None     1     0x80:  / 10000000:
  //
  // Errata.  The spec wrongly says:
  //[1] 9427.55  -2>-2         942755>942755 11    FE:39 45 A4 /
  //[1]                                            11111110:00111001 01000101 10100100
  //[2] 9427.60  -2/None       942760>942760 01    None:39 45 A8 /
  //[2]                                            :00111001 01000101 10101000


  const char testData[] = "\xFE\x39\x45\xA3\x39\x45\xA8\x80";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(5);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.rewind();

  Codecs::FieldInstructionDecimal field("Value", "");
  field.setPresence(false);

  Codecs::FieldInstructionExponentPtr exponent(new Codecs::FieldInstructionExponent("value|exponent", ""));
  exponent->setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpCopy));
  field.setExponentInstruction(exponent);

  Codecs::FieldInstructionMantissaPtr mantissa(new Codecs::FieldInstructionMantissa("value|mantissa", ""));
  mantissa->setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpCopy));
  field.setMantissaInstruction(mantissa);
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::DECIMAL));
  Decimal expectedA(942755, -2);
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectedA);


  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::DECIMAL));
  Decimal expectedB(942760, -2);
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectedB);

  BOOST_CHECK_EQUAL(fieldSet3.size(), 0);

  // Now reencode the data
  Codecs::PresenceMap pmapResult(3);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Utf8_Copy_Mandatory)
{
  // Copy Operator Example – Mandatory Unicode String
  // <string id="1" charset="unicode" presence="mandatory" name="Flag"> <copy/> </string>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // CME    None  3:CME   1     83 43 4d 45 /10000011 01000011 01001101 01000101
  // CME    CME   None    0     none
  // ISE    CME   3:ISE   1     83 49 53 C5 /80000011 01001001 01010011 01000101

  const char testData[] = "\x83\x43\x4d\x45\x83\x49\x53\x45";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(3);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.rewind();

  Codecs::FieldInstructionUtf8 field("Flag", "");
  field.setPresence(true);
  field.setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpCopy));
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 1);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet1.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "CME");

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet2.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "CME");

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet3.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "ISE");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Utf8_Copy_optional)
{
  // Copy Operator Example for NULL – Optional Unicode String
  // <string id="1" charset="unicode" presence="optional" name="Flag"> <copy/> </string>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // None   None  Null    1     80 / 10000000
  // None   Null  None    0     None
  // CME    Null  3:CME   1     84 43 4d 45 / 10000100 01000011 01001101 01000101
  //
  // This is an error in the specification.
  //
  // 6.3.5 of FAST 1.x.1 says:
  // When the value is not present in the input stream [...]
  //  If the field has optional presence and no initial value, the
  //  field is considered absent and the state of the previous value
  //  is changed to empty.
  //
  // The revised example:
  // Copy Operator Example for NULL – Optional Unicode String
  // <string id="1" charset="unicode" presence="optional" name="Flag"> <copy/> </string>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // None   None  Null    0     None
  // None   Null  None    0     None
  // CME    Null  3:CME   1     84 43 4d 45 / 10000100 01000011 01001101 01000101
  //


// Original, incorrect (see above)  const char testData[] = "\x80\x84\x43\x4d\x45";
  const char testData[] = "\x84\x43\x4d\x45";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(3);
// Original, incorrect (see above)  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.rewind();

  Codecs::FieldInstructionUtf8 field("Flag", "");
  field.setPresence(false);
  field.setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpCopy));
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 1);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // check generated fields
  BOOST_CHECK_EQUAL(fieldSet1.size(), 0);
  BOOST_CHECK_EQUAL(fieldSet2.size(), 0);
  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);

  Messages::FieldSet::const_iterator pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "CME");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Utf8_Delta_Mandatory)
{
  // Delta Operator Example – Mandatory Unicode String
  // <string id="1" charset="unicode" presence="mandatory" name="Security"> <delta/> </string>
  // No PMAP bits are used
  // Input    Prior     Encoded     FAST Hex/Binary
  //                    Len:Value   Len/Value
  // GEH6     Empty     0:4:GEH6    80:84:47 45 48 36
  //                                1000000:80000100:01000111 01000101 01001000 00110110
  // GEM6     GEH6      2:2:M6      82:82:4D 36 / 10000010 10000010 01001101 00110110
  // ESM6     GEM6      -2:2:ES     FD:82:45 53 / 11111101 10000010 01000101 01010011
  // RSESM6   ESM6      -0:2:RS     FF:82:52 53 / 11111111 01010010 01010011

  const char testData[] = "\x80\x84\x47\x45\x48\x36\x82\x82\x4D\x36\xFD\x82\x45\x53\xFF\x82\x52\x53";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(1);

  Codecs::FieldInstructionUtf8 field("Security", "");
  field.setPresence(true);
  field.setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpDelta));
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 0);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet4(1);
  fieldSet4.swap(consumer.message());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // check generated fields
  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet1.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "GEH6");

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "GEM6");

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "ESM6");

  BOOST_CHECK_EQUAL(fieldSet4.size(), 1);
  pFieldEntry = fieldSet4.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "RSESM6");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  field.encode(destination, pmapResult, encoder, fieldSet4);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testUtf8_Tail_Mandatory)
{
  //  Not in spec
  // Tail Operator Example – Mandatory Unicode String
  // <string id="1" charset="unicode" presence="mandatory" name="Security"> <tail/> </string>
  // Input    Prior   PMAP  Encoded     FAST Hex/Binary
  // GEH6     Empty   1     4:GEH6      84:47 45 48 36
  //                                    10000100:01000111 01000101 01001000 00110110
  // GEM6     GEH6    1     2:M6        82:4D 36 / 80000010:01001101 00110110
  // GEM6     GEM6    0     None
  const char testData[] = "\x84\x47\x45\x48\x36\x82\x4D\x36";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(3);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.rewind();

  Codecs::FieldInstructionUtf8 field("Security", "");
  field.setPresence(true);
  field.setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpTail));
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);
  BOOST_CHECK_EQUAL(field.getPresenceMapBitsUsed(), 1);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // check generated fields
  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet1.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "GEH6");

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "GEM6");

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "GEM6");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(3);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_issue_30)
{
  /*
  <uInt32 name="NAME" presence="optional"><copy/></uInt32>

            Input    Prior Encoded PMAP   FAST
    NAME      1      NULL  2       true   0x82
    NAME     None    1     NULL    true   0x80
    NAME      0      NULL  1       true   0x81

*/
  const char testData[] = "\x82\x80\x81";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(3);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.rewind();

  Codecs::FieldInstructionUInt32 field("NAME","");
  field.setPresence(false);

  Codecs::FieldOpPtr fieldOp(new Codecs::FieldOpCopy);
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,1,indexer.size()));
  field.finalize(*registry);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  // test a: decode the 1
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet1.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  // the value should be the one
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 1);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // test b: decode the NULL
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());
  pFieldEntry = fieldSet2.begin();
  BOOST_CHECK(pFieldEntry == fieldSet2.end());

  // test c: decode the 0
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());
  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet3.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  // the value should be the zero
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 0);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet3.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(3);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_issue_36)
{
/* <uInt64 name="MsgSeqNum" id="34"><increment/></uInt64>
               Input    Prior Encoded PMAP   FAST
    MsgSeqNum      1      NULL  1     true   0x81
    MsgSeqNum      2      1     --    false  --
    MsgSeqNum      3      2     --    false  --
    MsgSeqNum      5      3     3     true   0x85
*/
  const char testData[] = "\x81\x85";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(4);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.rewind();

  Codecs::FieldInstructionUInt64 field("MsgSeqNum","");
  field.setPresence(true);

  Codecs::FieldOpPtr fieldOp(new Codecs::FieldOpIncrement);
  field.setFieldOp(fieldOp);
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,1,indexer.size()));
  field.finalize(*registry);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  // test a: decode the 1
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet1.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT64));
  // the value should be the one
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt64(), 1);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // test b: decode the 2
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet2.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT64));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt64(), 2);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet2.end());

  // test c: decode the 3
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());
  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet3.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT64));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt64(), 3);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet3.end());

  // test d: decode the 5
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet4(1);
  fieldSet4.swap(consumer.message());
  pFieldEntry = fieldSet4.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet4.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT64));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt64(), 5);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet4.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(4);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  field.encode(destination, pmapResult, encoder, fieldSet4);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_swxess_mdpricelevel_problem)
{
/* <uInt32 name="MDPriceLevel" id="1023" presence="optional"><increment value="1"/></uInt32>
                  Input    Prior Encoded PMAP   FAST
    MDPriceLevel     --      1    NULL   true   0x80
    MDPriceLevel     --   NULL     --    false  --
*/
  const char testData[] = "\x80";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(2);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.rewind();

  Codecs::FieldInstructionUInt64 field("MDPriceLevel","");
  field.setPresence(false);

  Codecs::FieldOpPtr fieldOp(new Codecs::FieldOpIncrement);
  field.setFieldOp(fieldOp);
  fieldOp->setValue("1");
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,1,indexer.size()));
  field.finalize(*registry);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  // test a: decode explicit NULL
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // test b: decode implicit NULL
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  pFieldEntry = fieldSet2.begin();
  BOOST_CHECK(pFieldEntry == fieldSet2.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(4);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}



BOOST_AUTO_TEST_CASE(test_issue_38_string)
{
/* <string name="TestRequestId" presence="optional" id="112"><copy/></string>
               Input    Prior Encoded PMAP   FAST
TestRequestId   --       --    --      0      --
TestRequestId   "ID"     --    "ID"    1      49 C4
TestRequestId   "ID"     "ID"  "ID"    0      --
TestRequestId   "JD"     "ID"  "JD"    1      4A C4
TestRequestID   --       "JD"  NULL    1      80
TestRequestID   --       NULL  --      0      --
*/
  const char testData[] = "\x49\xC4\x4a\xC4\x80";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(6);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.rewind();

  Codecs::FieldInstructionAscii field("TestRequestId","");
  field.setPresence(false);

  Codecs::FieldOpPtr fieldOp(new Codecs::FieldOpCopy);
  field.setFieldOp(fieldOp);
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(4,1,indexer.size()));
  field.finalize(*registry);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  // test1: decode the ID that isn't there
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry == fieldSet1.end()); // should be empty

  // test2: decode the "ID"
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet2.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "ID");
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet2.end());

  // test3: decode copied "ID"
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet3.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "ID");
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet3.end());

  // test4: decode explicit "JD"
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet4(1);
  fieldSet4.swap(consumer.message());

  pFieldEntry = fieldSet4.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet4.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "JD");
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet4.end());


  // test5: decode the explicitly nulled value
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet5(1);
  fieldSet5.swap(consumer.message());
  pFieldEntry = fieldSet5.begin();
  BOOST_CHECK(pFieldEntry == fieldSet5.end());

  // test 6: decode the implicilty null entry
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet6(1);
  fieldSet5.swap(consumer.message());
  pFieldEntry = fieldSet6.begin();
  BOOST_CHECK(pFieldEntry == fieldSet6.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(6);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  field.encode(destination, pmapResult, encoder, fieldSet4);
  field.encode(destination, pmapResult, encoder, fieldSet5);
  field.encode(destination, pmapResult, encoder, fieldSet6);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_issue_38_integer)
{
/* <int32 name="OptionalNumberCopy" presence="optional" id="112"><copy/></int32>
                  Input    Prior Encoded PMAP   FAST
OptionalNumberCopy   --       --    --      0      --
OptionalNumberCopy   10       --    10      1      8B
OptionalNumberCopy   10       10    --      0      --
OptionalNumberCopy   20       10    20      1      95
OptionalNumberCopy   --       20    NULL    1      80
OptionalNumberCopy   --       NULL  --      0      --
*/
  const char testData[] = "\x8B\x95\x80";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(6);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.rewind();

  Codecs::FieldInstructionInt32 field("OptionalNumberCopy","");
  field.setPresence(false);

  Codecs::FieldOpPtr fieldOp(new Codecs::FieldOpCopy);
  field.setFieldOp(fieldOp);
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(4,1,indexer.size()));
  field.finalize(*registry);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  // test1: decode the number that isn't there
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry == fieldSet1.end()); // should be empty

  // test2: decode the 10
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet2.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::INT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 10);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet2.end());

  // test3: decode copied 10
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet3.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::INT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 10);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet3.end());

  // test4: decode explicit 20
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet4(1);
  fieldSet4.swap(consumer.message());

  pFieldEntry = fieldSet4.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet4.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::INT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 20);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet4.end());

  // test5: decode the explicitly nulled value
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet5(1);
  fieldSet5.swap(consumer.message());
  pFieldEntry = fieldSet5.begin();
  BOOST_CHECK(pFieldEntry == fieldSet5.end());

  // test 6: decode the implicilty null entry
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet6(1);
  fieldSet5.swap(consumer.message());
  pFieldEntry = fieldSet6.begin();
  BOOST_CHECK(pFieldEntry == fieldSet6.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(6);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  field.encode(destination, pmapResult, encoder, fieldSet4);
  field.encode(destination, pmapResult, encoder, fieldSet5);
  field.encode(destination, pmapResult, encoder, fieldSet6);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}



BOOST_AUTO_TEST_CASE(test_default_string)
{
/* <string name="MDEntryType" id="269">  <default value="2" /> </string>
               Input    Prior Encoded PMAP   FAST
MDEntryType     "2"      "2"   --      0      --
MDEntryType     "3"      "2"   "3"     1      B3
MDEntryType     "2"      "2"   --      0      --
*/
  const char testData[] = "\xB3";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(3);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.rewind();

  Codecs::FieldInstructionAscii field("MDEntryType","");
  field.setPresence(true);
  Codecs::FieldOpPtr fieldOp(new Codecs::FieldOpDefault);
  fieldOp->setValue("2");
  field.setFieldOp(fieldOp);

  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(4,1,indexer.size()));
  field.finalize(*registry);

  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  // test1: decode the default value
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());

  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet1.end()); // should not be empty
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "2");
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // test2: decode the "3"
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());

  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet2.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "3");
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet2.end());

  // test3: decode "2" again
  builder.startMessage("UNIT_TEST", "", 1);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet3.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "2");
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet3.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(4);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_optionalDefaultNonNullUnsignedInt)
{
/* Default Operator Example for nonnull– Optional Unsigned Integer
   <uInt32 id="1" presence="optional" name="Flag"> <default value="1"/> </uInt32>

Name    Input  Prior Encoded PMap  FAST Hex/Binary
Flag    none   1     NULL    1     80
Flag    1      1     --      0     --
Flag    2      1     2+1     1     83
Flag    126    1     126+1   1     FF
*/
  const char testData[] = "\x80\x83\xFF";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(4);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.rewind();

  Codecs::FieldInstructionUInt32 field("Flag", "");
  field.setPresence(false);

  Codecs::FieldOpPtr fieldOp(new Codecs::FieldOpDefault);
  fieldOp->setValue("1");
  field.setFieldOp(fieldOp);
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);


  // test1: decode missing field
  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));

  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // test2: decode defaulted field
  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());
  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet2.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 1);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet2.end());

  // test3: decode explicit field
  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());

  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet3.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 2);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet3.end());

  // test4: decode field that would be negative if signed
  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet4(1);
  fieldSet4.swap(consumer.message());

  pFieldEntry = fieldSet4.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet4.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 126);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet4.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(4);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  field.encode(destination, pmapResult, encoder, fieldSet4);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_optionalDefaultNonNullSignedInt)
{
/* Default Operator Example for nonnull– Optional signed Integer
   <int32 id="1" presence="optional" name="Flag"> <default value="1"/> </uInt32>

Name    Input  Prior Encoded PMap  FAST Hex/Binary
Flag    none   1     NULL    1     80
Flag    1      1     --      0     --
Flag    2      1     2       1     83
Flag    -1     1     -1      1     FF
*/
  const char testData[] = "\x80\x83\xFF";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  Codecs::DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(4);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.rewind();

  Codecs::FieldInstructionInt32 field("Flag", "");
  field.setPresence(false);

  Codecs::FieldOpPtr fieldOp(new Codecs::FieldOpDefault);
  fieldOp->setValue("1");
  field.setFieldOp(fieldOp);
  field.indexDictionaries(indexer, "global", "", "");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  field.finalize(*registry);

  // We neeed the helper routines in the decoder
  Codecs::Decoder decoder(registry);

  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);

  // test1: decode missing field
  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet1(1);
  fieldSet1.swap(consumer.message());
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // test2: decode defaulted 1
  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet2(1);
  fieldSet2.swap(consumer.message());
  pFieldEntry = fieldSet2.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet2.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::INT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 1);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet2.end());

  // test3: decode explicit 2
  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet3(1);
  fieldSet3.swap(consumer.message());
  pFieldEntry = fieldSet3.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet3.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::INT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 2);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet3.end());

  // test4: decode explicit -1
  builder.startMessage("UNIT_TEST", "", 10);
  field.decode(source, pmap, decoder, builder);
  BOOST_REQUIRE(builder.endMessage(builder));
  Messages::Message fieldSet4(1);
  fieldSet4.swap(consumer.message());
  pFieldEntry = fieldSet4.begin();
  BOOST_REQUIRE(pFieldEntry != fieldSet4.end());
  BOOST_REQUIRE(pFieldEntry->getField()->isType(ValueType::INT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), -1);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet4.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(4);
  Codecs::DataDestination destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  field.encode(destination, pmapResult, encoder, fieldSet4);
  destination.endMessage();
  std::string result;
  destination.toString(result);
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

