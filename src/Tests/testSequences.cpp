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
#include <Codecs/DataSourceBuffer.h>
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
#include <Messages/MessageFormatter.h>

#include <Common/Exceptions.h>

using namespace QuickFAST;


namespace
{

// template string where template 5 (nxAccountGroupDefinition) comes BEFORE template 4 (nxAccountDefinition)
  const char id4FirstTemplateString[] = "\n\
<templates>\n\
  <template id=\"4\" name=\"nxAccountDefinition\">\n\
    <typeRef name=\"nxAccountDefinition\"/>\n\
    <uInt32 id=\"1\" name=\"objectType\">\n\
      <constant value=\"3\"/>\n\
    </uInt32>\n\
    <uInt32 id=\"2\" name=\"ID\">\n\
      <default value=\"0\"/>\n\
    </uInt32>\n\
    <sequence id=\"3\" name=\"childNodes\">\n\
      <length name=\"childNodesLength\"/>\n\
      <uInt32 name=\"childNodesSeq\"/>\n\
    </sequence>\n\
    <sequence name=\"childNodes4\" presence=\"optional\">\n\
      <length name=\"childNodesLength4\"/>\n\
      <templateRef name=\"nxAccountDefinition\"/>\n\
    </sequence>\n\
    <sequence name=\"childNodes5\" presence=\"optional\">\n\
      <length name=\"childNodesLength5\"/>\n\
      <templateRef name=\"nxAccountGroupDefinition\"/>\n\
    </sequence>\n\
    <uInt32 id=\"5\" name=\"state\">\n\
      <default value=\"0\"/>\n\
    </uInt32>\n\
    <string id=\"6\" name=\"strName\">\n\
      <default value=\"\"/>\n\
    </string>\n\
    <uInt32 id=\"7\" name=\"accessLevel\">\n\
      <default value=\"0\"/>\n\
    </uInt32>\n\
    <string id=\"8\" name=\"clearingAccount\">\n\
      <default value=\"\"/>\n\
    </string>\n\
  </template>\n\
  <template id=\"5\" name=\"nxAccountGroupDefinition\">\n\
    <typeRef name=\"nxAccountGroupDefinition\"/>\n\
    <uInt32 id=\"1\" name=\"objectType\">\n\
      <constant value=\"3\"/>\n\
    </uInt32>\n\
    <uInt32 id=\"2\" name=\"ID\">\n\
      <default value=\"0\"/>\n\
    </uInt32>\n\
    <sequence id=\"3\" name=\"childNodes\">\n\
      <length name=\"childNodesLength\"/>\n\
      <uInt32 name=\"childNodesSeq\"/>\n\
    </sequence>\n\
    <sequence name=\"childNodes4\" presence=\"optional\">\n\
      <length name=\"childNodesLength4\"/>\n\
      <templateRef name=\"nxAccountDefinition\"/>\n\
    </sequence>\n\
    <sequence name=\"childNodes5\" presence=\"optional\">\n\
      <length name=\"childNodesLength5\"/>\n\
      <templateRef name=\"nxAccountGroupDefinition\"/>\n\
    </sequence>\n\
    <uInt32 id=\"5\" name=\"state\">\n\
      <default value=\"0\"/>\n\
    </uInt32>\n\
    <string id=\"6\" name=\"strName\">\n\
      <default value=\"\"/>\n\
    </string>\n\
    <uInt32 id=\"7\" name=\"accessLevel\">\n\
      <default value=\"0\"/>\n\
    </uInt32>\n\
    <string id=\"8\" name=\"clearingAccount\">\n\
      <default value=\"\"/>\n\
    </string>\n\
  </template>\n\
</templates>\n\
";


// template string where template 5 (nxAccountGroupDefinition) comes BEFORE template 4 (nxAccountDefinition)
  const char id5FirstTemplateString[] = "\n\
<templates>\n\
  <template id=\"5\" name=\"nxAccountGroupDefinition\">\n\
    <typeRef name=\"nxAccountGroupDefinition\"/>\n\
    <uInt32 id=\"1\" name=\"objectType\">\n\
      <constant value=\"3\"/>\n\
    </uInt32>\n\
    <uInt32 id=\"2\" name=\"ID\">\n\
      <default value=\"0\"/>\n\
    </uInt32>\n\
    <sequence id=\"3\" name=\"childNodes\">\n\
      <length name=\"childNodesLength\"/>\n\
      <uInt32 name=\"childNodesSeq\"/>\n\
    </sequence>\n\
    <sequence name=\"childNodes4\" presence=\"optional\">\n\
      <length name=\"childNodesLength4\"/>\n\
      <templateRef name=\"nxAccountDefinition\"/>\n\
    </sequence>\n\
    <sequence name=\"childNodes5\" presence=\"optional\">\n\
      <length name=\"childNodesLength5\"/>\n\
      <templateRef name=\"nxAccountGroupDefinition\"/>\n\
    </sequence>\n\
    <uInt32 id=\"5\" name=\"state\">\n\
      <default value=\"0\"/>\n\
    </uInt32>\n\
    <string id=\"6\" name=\"strName\">\n\
      <default value=\"\"/>\n\
    </string>\n\
    <uInt32 id=\"7\" name=\"accessLevel\">\n\
      <default value=\"0\"/>\n\
    </uInt32>\n\
    <string id=\"8\" name=\"clearingAccount\">\n\
      <default value=\"\"/>\n\
    </string>\n\
  </template>\n\
  <template id=\"4\" name=\"nxAccountDefinition\">\n\
    <typeRef name=\"nxAccountDefinition\"/>\n\
    <uInt32 id=\"1\" name=\"objectType\">\n\
      <constant value=\"3\"/>\n\
    </uInt32>\n\
    <uInt32 id=\"2\" name=\"ID\">\n\
      <default value=\"0\"/>\n\
    </uInt32>\n\
    <sequence id=\"3\" name=\"childNodes\">\n\
      <length name=\"childNodesLength\"/>\n\
      <uInt32 name=\"childNodesSeq\"/>\n\
    </sequence>\n\
    <sequence name=\"childNodes4\" presence=\"optional\">\n\
      <length name=\"childNodesLength4\"/>\n\
      <templateRef name=\"nxAccountDefinition\"/>\n\
    </sequence>\n\
    <sequence name=\"childNodes5\" presence=\"optional\">\n\
      <length name=\"childNodesLength5\"/>\n\
      <templateRef name=\"nxAccountGroupDefinition\"/>\n\
    </sequence>\n\
    <uInt32 id=\"5\" name=\"state\">\n\
      <default value=\"0\"/>\n\
    </uInt32>\n\
    <string id=\"6\" name=\"strName\">\n\
      <default value=\"\"/>\n\
    </string>\n\
    <uInt32 id=\"7\" name=\"accessLevel\">\n\
      <default value=\"0\"/>\n\
    </uInt32>\n\
    <string id=\"8\" name=\"clearingAccount\">\n\
      <default value=\"\"/>\n\
    </string>\n\
  </template>\n\
</templates>\n\
";

// fields for nxAccountDefinition and nxAccountGroupDefinition
  Messages::FieldIdentity objectTypeField("objectType");
  Messages::FieldIdentity idFieldId("ID");
  Messages::FieldIdentity childNodesFieldId("childNodes");
  Messages::FieldIdentity childNodesLengthId( "childNodesLength" );
  Messages::FieldIdentity childNodesSeqId( "childNodesSeq" );
  Messages::FieldIdentity stateId( "state" );
  Messages::FieldIdentity strNameId( "strName" );
  Messages::FieldIdentity accessLevelId( "accessLevel" );
  Messages::FieldIdentity clearingAcountId( "clearingAccount" );
  Messages::FieldIdentity childNodes4FieldId("childNodes4");
  Messages::FieldIdentity childNodesLength4Id( "childNodesLength4" );

  void validateMessage1(Messages::Message & message)
  {
    BOOST_CHECK_EQUAL(message.getApplicationType(), "nxAccountGroupDefinition");
    Messages::FieldCPtr value;
  }
}

BOOST_AUTO_TEST_CASE(TestSequences)
{
  bool verbose = false;
  Messages::MessageFormatter formatter(std::cout);

  // NOTE! replace with id5FirstTemplateString to see case where decoding succeeds.
  std::stringstream templateStream(id4FirstTemplateString);
//  std::stringstream templateStream(id5FirstTemplateString);

  Codecs::XMLTemplateParser parser;
//  parser.setVerboseOutput(std::cout);
  Codecs::TemplateRegistryPtr templateRegistry =
    parser.parse(templateStream);

  // for debugging: display the parsed templates
  if(verbose)
  {
    templateRegistry->display(std::cout);
  }

  Messages::Message msg(templateRegistry->maxFieldCount());

// create an empty childNodes sequence for the nxAccountDefinitions
  Messages::SequencePtr childNodesSequence( new Messages::Sequence( childNodesLengthId, 0 ) );

// create a "TesterA" nxAccountDefinition
  Messages::FieldSetPtr nxAccountDefinition1FieldSet(new Messages::FieldSet(7));
  nxAccountDefinition1FieldSet->addField(objectTypeField, Messages::FieldUInt32::create(3));
  nxAccountDefinition1FieldSet->addField(idFieldId, Messages::FieldUInt32::create(0));
  nxAccountDefinition1FieldSet->addField(childNodesFieldId, Messages::FieldSequence::create(childNodesSequence));
  nxAccountDefinition1FieldSet->addField(stateId, Messages::FieldUInt32::create(2801));
  nxAccountDefinition1FieldSet->addField(strNameId, Messages::FieldAscii::create("TesterA"));
  nxAccountDefinition1FieldSet->addField(accessLevelId, Messages::FieldUInt32::create(0));
  nxAccountDefinition1FieldSet->addField(clearingAcountId, Messages::FieldAscii::create(""));

// create a "TesterB" nxAccountDefinition
  Messages::FieldSetPtr nxAccountDefinition2FieldSet(new Messages::FieldSet(7));
  nxAccountDefinition2FieldSet->addField(objectTypeField, Messages::FieldUInt32::create(3));
  nxAccountDefinition2FieldSet->addField(idFieldId, Messages::FieldUInt32::create(0));
  nxAccountDefinition2FieldSet->addField(childNodesFieldId, Messages::FieldSequence::create(childNodesSequence));
  nxAccountDefinition2FieldSet->addField(stateId, Messages::FieldUInt32::create(2801));
  nxAccountDefinition2FieldSet->addField(strNameId, Messages::FieldAscii::create("TesterB"));
  nxAccountDefinition2FieldSet->addField(accessLevelId, Messages::FieldUInt32::create(0));
  nxAccountDefinition2FieldSet->addField(clearingAcountId, Messages::FieldAscii::create(""));

// create a "TesterB" nxAccountDefinition
  Messages::FieldSetPtr nxAccountDefinition3FieldSet(new Messages::FieldSet(7));
  nxAccountDefinition3FieldSet->addField(objectTypeField, Messages::FieldUInt32::create(3));
  nxAccountDefinition3FieldSet->addField(idFieldId, Messages::FieldUInt32::create(0));
  nxAccountDefinition3FieldSet->addField(childNodesFieldId, Messages::FieldSequence::create(childNodesSequence));
  nxAccountDefinition3FieldSet->addField(stateId, Messages::FieldUInt32::create(2801));
  nxAccountDefinition3FieldSet->addField(strNameId, Messages::FieldAscii::create("TesterC"));
  nxAccountDefinition3FieldSet->addField(accessLevelId, Messages::FieldUInt32::create(0));
  nxAccountDefinition3FieldSet->addField(clearingAcountId, Messages::FieldAscii::create(""));

// create another child node sequence for the main message
  Messages::SequencePtr childNodesSequence2( new Messages::Sequence( childNodesLengthId, 1 ) );
  Messages::FieldSetPtr childNodesFieldSet(new Messages::FieldSet(1));
  childNodesFieldSet->addField(childNodesSeqId, Messages::FieldUInt32::create(4));
  childNodesSequence2->addEntry(childNodesFieldSet);

// create a childNodes4 sequence to hold the 3 nxAccountDefinitions
  Messages::SequencePtr childNodes4Sequence( new Messages::Sequence( childNodesLength4Id, 3) );
  childNodes4Sequence->addEntry(nxAccountDefinition1FieldSet);
  //childNodes4Sequence->addEntry(nxAccountDefinition2FieldSet);	// can add these in for extended test
  //childNodes4Sequence->addEntry(nxAccountDefinition3FieldSet);	// can add these in for extended test

// now, populate the top level (nxAccountGroupDefinition) message
  msg.addField(objectTypeField, Messages::FieldUInt32::create(3));
  msg.addField(idFieldId, Messages::FieldUInt32::create(0));
  msg.addField(childNodes4FieldId, Messages::FieldSequence::create(childNodes4Sequence));
  msg.addField(childNodesFieldId, Messages::FieldSequence::create(childNodesSequence2));
  msg.addField(stateId, Messages::FieldUInt32::create(2801));
  msg.addField(strNameId, Messages::FieldAscii::create("Testers"));
  msg.addField(accessLevelId, Messages::FieldUInt32::create(0));
  msg.addField(clearingAcountId, Messages::FieldAscii::create(""));


  if(verbose)
  {
//  // Now let's see what it looks like "before"
    std::cout << "Before Encoding: " << std::endl;
    formatter.formatMessage(msg);
    std::cout << std::endl << "--END--" << std::endl;
  }

  Codecs::Encoder encoder(templateRegistry);
  if(verbose)
  {
    encoder.setVerboseOutput(std::cout);
  }

  Codecs::DataDestination destination;
  if(verbose)
  {
    destination.setVerbose(std::cout);
  }

  template_id_t templId = 5; // from the XML file
  encoder.encodeMessage(destination, templId, msg);
  WorkingBuffer fastMsg;
  destination.toWorkingBuffer(fastMsg);
  if(verbose)
  {
    fastMsg.hexDisplay(std::cout);
  }

  destination.clear();

  Codecs::Decoder decoder(templateRegistry);

  Codecs::DataSourceBuffer source(fastMsg.begin(), fastMsg.size());
  if(verbose)
  {
    source.setEcho(std::cout, Codecs::DataSource::HEX, true, true);
  }
  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);
  decoder.decodeMessage(source, builder);
  Messages::Message & msgOut = consumer.message();
  if(verbose)
  {
    std::cout << std::endl << "Decoded Message: " << std::endl;
    formatter.formatMessage(msgOut);
    std::cout << "---END---" << std::endl;
  }
  validateMessage1(msgOut);

  // wanna see it again?
  encoder.reset();
  encoder.encodeMessage(destination, templId, msgOut);
  WorkingBuffer reencoded;
  destination.toWorkingBuffer(reencoded);

  destination.clear();

  if(verbose)
  {
    reencoded.hexDisplay(std::cout);
  }
  BOOST_CHECK(fastMsg == reencoded);
}
