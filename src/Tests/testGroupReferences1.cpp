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
  const char templateString[] = "\n\
<templates>\n\
  <template id=\"19\" name=\"nxCurrencyID\">\n\
    <typeRef name=\"nxCurrencyID\"/>\n\
    <string id=\"2\" name=\"name\" presence=\"optional\">\n\
      <default value=\"\"/>\n\
    </string>\n\
  </template>\n\
  <template id=\"22\" name=\"nxEquityID\">\n\
    <typeRef name=\"nxEquityID\"/>\n\
    <string id=\"2\" name=\"name\" presence=\"optional\">\n\
      <default value=\"\"/>\n\
    </string>\n\
  </template>\n\
  <template id=\"37\" name=\"nxFutureID\">\n\
    <typeRef name=\"nxFutureID\"/>\n\
    <string id=\"2\" name=\"name\" presence=\"optional\">\n\
      <default value=\"\"/>\n\
    </string>\n\
  </template>\n\
    <template id=\"97\" name=\"nxStrategyID\">\n\
    <typeRef name=\"nxStrategyID\"/>\n\
    <string id=\"2\" name=\"name\" presence=\"optional\">\n\
      <default value=\"\"/>\n\
    </string>\n\
  </template>\n\
  <template id=\"67\" name=\"nxOptionID\">\n\
    <typeRef name=\"nxOptionID\"/>\n\
    <string id=\"2\" name=\"name\" presence=\"optional\">\n\
      <default value=\"\"/>\n\
    </string>\n\
  </template>\n\
  <template id=\"68\" name=\"nxOptionalOrderParameters\">\n\
    <typeRef name=\"nxOptionalOrderParameters\"/>\n\
    <group name=\"conditionalProductID19\" presence=\"optional\">\n\
      <templateRef name=\"nxCurrencyID\"/>\n\
    </group>\n\
    <group name=\"conditionalProductID22\" presence=\"optional\">\n\
      <templateRef name=\"nxEquityID\"/>\n\
    </group>\n\
    <group name=\"conditionalProductID37\" presence=\"optional\">\n\
      <templateRef name=\"nxFutureID\"/>\n\
    </group>\n\
    <group name=\"conditionalProductID67\" presence=\"optional\">\n\
      <templateRef name=\"nxOptionID\"/>\n\
    </group>\n\
    <group name=\"conditionalProductID97\" presence=\"optional\">\n\
      <templateRef name=\"nxStrategyID\"/>\n\
    </group>\n\
  </template>\n\
  <template id=\"70\" name=\"nxOrderTransaction\">\n\
    <typeRef name=\"nxOrderTransaction\"/>\n\
    <group name=\"order71\" presence=\"optional\">\n\
      <templateRef name=\"nxOrder\"/>\n\
    </group>\n\
    <uInt32 id=\"9\" name=\"fillVolume\">\n\
      <default value=\"4294967294\"/>\n\
    </uInt32>\n\
  </template>\n\
  <template id=\"71\" name=\"nxOrder\">\n\
    <typeRef name=\"nxOrder\"/>\n\
    <group name=\"productID19\" presence=\"optional\">\n\
      <templateRef name=\"nxCurrencyID\"/>\n\
  </group>\n\
    <group name=\"optionalOrderParameters68\" presence=\"optional\">\n\
      <templateRef name=\"nxOptionalOrderParameters\"/>\n\
    </group>\n\
  </template>\n\
  <!-- Fully expanded version -->\n\
  <template id=\"77\" name=\"nxOrderTransaction77\">\n\
    <typeRef name=\"nxOrderTransaction\"/>\n\
    <group name=\"order71\" presence=\"optional\">\n\
      <typeRef name=\"nxOrderType\"/>\n\
      <group name=\"productID19\" presence=\"optional\">\n\
        <typeRef name=\"nxCurrencyIDType\"/>\n\
        <string id=\"2\" name=\"name\" presence=\"optional\">\n\
          <default value=\"\"/>\n\
        </string>\n\
      </group>\n\
      <group name=\"optionalOrderParameters68\" presence=\"optional\">\n\
        <typeRef name=\"nxOptionalOrderParametersType\"/>\n\
        <group name=\"conditionalProductID19\" presence=\"optional\">\n\
          <typeRef name=\"nxCurrencyIDType\"/>\n\
          <string id=\"2\" name=\"name\" presence=\"optional\">\n\
            <default value=\"\"/>\n\
          </string>\n\
        </group>\n\
        <group name=\"conditionalProductID22\" presence=\"optional\">\n\
          <typeRef name=\"nxEquityIDType\"/>\n\
          <string id=\"2\" name=\"name\" presence=\"optional\">\n\
            <default value=\"\"/>\n\
          </string>\n\
        </group>\n\
        <group name=\"conditionalProductID37\" presence=\"optional\">\n\
          <typeRef name=\"nxFutureIDType\"/>\n\
          <string id=\"2\" name=\"name\" presence=\"optional\">\n\
            <default value=\"\"/>\n\
          </string>\n\
        </group>\n\
        <group name=\"conditionalProductID67\" presence=\"optional\">\n\
          <typeRef name=\"nxOptionIDType\"/>\n\
          <string id=\"2\" name=\"name\" presence=\"optional\">\n\
            <default value=\"\"/>\n\
          </string>\n\
        </group>\n\
        <group name=\"conditionalProductID97\" presence=\"optional\">\n\
          <typeRef name=\"nxStrategyIDType\"/>\n\
          <string id=\"2\" name=\"name\" presence=\"optional\">\n\
            <default value=\"\"/>\n\
          </string>\n\
        </group>\n\
      </group>\n\
    </group>\n\
    <uInt32 id=\"9\" name=\"fillVolume\">\n\
      <default value=\"4294967294\"/>\n\
    </uInt32>\n\
  </template>\n\
</templates>\n\
";



  void validateMessage1(Messages::Message & message)
  {
    BOOST_CHECK_EQUAL(message.getApplicationType(), "nxOrderTransaction");
    Messages::FieldCPtr value;

    //"order71" group
    BOOST_REQUIRE(message.getField("order71", value));
    Messages::FieldSetCPtr order71Group = value->toGroup();
    BOOST_REQUIRE(order71Group->getField("productID19", value));
    Messages::FieldSetCPtr nxOrderGroup = value->toGroup();
    BOOST_CHECK_EQUAL(nxOrderGroup->getApplicationType(), "nxCurrencyIDType");
  }
}

BOOST_AUTO_TEST_CASE(TestGroupReferences1)
{
  Messages::MessageFormatter formatter(std::cout);

  std::stringstream templateStream(templateString);

  Codecs::XMLTemplateParser parser;
  Codecs::TemplateRegistryPtr templateRegistry =
    parser.parse(templateStream);

  // for debugging: display the parsed templates
//  templateRegistry->display(std::cout);

  Messages::Message msg(templateRegistry->maxFieldCount());

// Create the nxCurrencyID group fieldset
  Messages::FieldSetPtr nxCurrencyIDFieldSet(new Messages::FieldSet(1));	// name
  nxCurrencyIDFieldSet->setApplicationType("nxCurrencyIDType", "");
  Messages::FieldIdentityCPtr nameField = new Messages::FieldIdentity("name");
  nxCurrencyIDFieldSet->addField(nameField, Messages::FieldAscii::create(""));

// Create the nxOptionalOrderParameters group fieldset
  Messages::FieldSetPtr nxOptionalOrderParametersFieldSet(new Messages::FieldSet(1));	// conditionalProductID19Field
  nxOptionalOrderParametersFieldSet->setApplicationType("nxOptionalOrderParametersType", "");
  Messages::FieldIdentityCPtr conditionalProductID19Field = new Messages::FieldIdentity("conditionalProductID19");
  nxOptionalOrderParametersFieldSet->addField(conditionalProductID19Field, Messages::FieldGroup::create(nxCurrencyIDFieldSet));

// Create the nxOrder group fieldset and add in the sub groups
  Messages::FieldSetPtr nxOrderFieldSet(new Messages::FieldSet(2)); // productID19 & optionalOrderParameters68
  nxOrderFieldSet->setApplicationType("nxOrderType","");
  Messages::FieldIdentityCPtr productID19Field = new Messages::FieldIdentity("productID19");
  Messages::FieldIdentityCPtr optionalOrderParameters68Field = new Messages::FieldIdentity("optionalOrderParameters68");

  nxOrderFieldSet->addField(productID19Field, Messages::FieldGroup::create(nxCurrencyIDFieldSet));
  nxOrderFieldSet->addField(optionalOrderParameters68Field, Messages::FieldGroup::create(nxOptionalOrderParametersFieldSet));

  // add in the fields to the message
  Messages::FieldIdentityCPtr order71Field = new Messages::FieldIdentity("order71");
  msg.addField( order71Field, Messages::FieldGroup::create(nxOrderFieldSet) );
  Messages::FieldIdentityCPtr fillVolumeField = new Messages::FieldIdentity("fillVolume");
  msg.addField( fillVolumeField, Messages::FieldUInt32::create(4294967295) );

//  // Now let's see what it looks like "before"
//  std::cout << "Before Encoding: " << std::endl;
//  formatter.formatMessage(msg);
//  std::cout << std::endl << "--END--" << std::endl;

  Codecs::Encoder encoder(templateRegistry);
//  encoder.setVerboseOutput(std::cout);

  Codecs::DataDestination destination;
//  destination.setVerbose(std::cout);

//  template_id_t templId = 70; // from the XML file
  template_id_t templId = 77; // fully expanded
  encoder.encodeMessage(destination, templId, msg);
  WorkingBuffer fastMsg;
  destination.toWorkingBuffer(fastMsg);
//  fastMsg.hexDisplay(std::cout);

  destination.clear();

  Codecs::Decoder decoder(templateRegistry);

  Codecs::DataSourceBuffer source(fastMsg.begin(), fastMsg.size());
//  source.setEcho(std::cout, Codecs::DataSource::HEX, true, true);
  Codecs::SingleMessageConsumer consumer;
  Codecs::GenericMessageBuilder builder(consumer);
  decoder.decodeMessage(source, builder);
  Messages::Message & msgOut = consumer.message();
//  std::cout << std::endl << "Decoded Message: " << std::endl;
//  formatter.formatMessage(msgOut);
//  std::cout << "---END---" << std::endl;

  validateMessage1(msgOut);

  // wanna see it again?
  encoder.reset();
  encoder.encodeMessage(destination, templId, msgOut);
  WorkingBuffer reencoded;
  destination.toWorkingBuffer(reencoded);

  destination.clear();

//  reencoded.hexDisplay(std::cout);
  BOOST_CHECK(fastMsg == reencoded);
}
