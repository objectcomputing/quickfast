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

namespace{
  Messages::FieldIdentity nameField("name");
  Messages::FieldIdentity nxCurrencyIDIdentity("nxCurrencyID");
  Messages::FieldIdentity conditionalProductID19Identity("conditionalProductID19");
  Messages::FieldIdentity nxOptionalOrderParametersIdentity("nxOptionalOrderParameters");
  Messages::FieldIdentity productID19Identity("productID19");
  Messages::FieldIdentity optionalOrderParameters68Identity("optionalOrderParameters68");
  Messages::FieldIdentity nxOrderIdentity("nxOrder");
  Messages::FieldIdentity order71Identity("order71");
  Messages::FieldIdentity fillVolumeIdentity("fillVolume");
}

namespace
{
  const char templateString[] = "\n\
      <templates>\n\
        <template id=\"19\" name=\"nxCurrencyID\">\n\
          <typeRef name=\"nxCurrencyIDType\"/>\n\
          <string id=\"2\" name=\"name\" presence=\"optional\">\n\
            <default value=\"\"/>\n\
          </string>\n\
        </template>\n\
        <template id=\"22\" name=\"nxEquityID\">\n\
          <typeRef name=\"nxEquityIDType\"/>\n\
          <string id=\"2\" name=\"name\" presence=\"optional\">\n\
            <default value=\"\"/>\n\
          </string>\n\
        </template>\n\
        <template id=\"37\" name=\"nxFutureID\">\n\
          <typeRef name=\"nxFutureIDType\"/>\n\
          <string id=\"2\" name=\"name\" presence=\"optional\">\n\
            <default value=\"\"/>\n\
          </string>\n\
        </template>\n\
        <template id=\"97\" name=\"nxStrategyID\">\n\
          <typeRef name=\"nxStrategyIDType\"/>\n\
          <string id=\"2\" name=\"name\" presence=\"optional\">\n\
            <default value=\"\"/>\n\
          </string>\n\
        </template>\n\
        <template id=\"67\" name=\"nxOptionID\">\n\
          <typeRef name=\"nxOptionIDType\"/>\n\
          <string id=\"2\" name=\"name\" presence=\"optional\">\n\
            <default value=\"\"/>\n\
          </string>\n\
        </template>\n\
        <template id=\"68\" name=\"nxOptionalOrderParameters\">\n\
          <typeRef name=\"nxOptionalOrderParametersType\"/>\n\
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
          <typeRef name=\"nxOrderTransactionType\"/>\n\
          <group name=\"order71\" presence=\"optional\">\n\
            <templateRef name=\"nxOrder\"/>\n\
          </group>\n\
          <uInt32 id=\"9\" name=\"fillVolume\">\n\
            <default value=\"4294967294\"/>\n\
          </uInt32>\n\
        </template>\n\
        <template id=\"71\" name=\"nxOrder\">\n\
          <typeRef name=\"nxOrderType\"/>\n\
          <group name=\"productID19\" presence=\"optional\">\n\
            <templateRef name=\"nxCurrencyID\"/>\n\
          </group>\n\
          <group name=\"optionalOrderParameters68\" presence=\"optional\">\n\
            <templateRef name=\"nxOptionalOrderParameters\"/>\n\
          </group>\n\
        </template>\n\
        <!-- Fully expanded version -->\n\
        <template id=\"77\" name=\"nxOrderTransaction77\">\n\
          <typeRef name=\"nxOrderTransactionType\"/>\n\
          <group name=\"order71\" presence=\"optional\">\n\
            <group name=\"nxOrder\">\n\
                <typeRef name=\"nxOrderType\"/>\n\
                <group name=\"productID19\" presence=\"optional\">\n\
                  <group name=\"nxCurrencyID\">\n\
                    <typeRef name=\"nxCurrencyIDType\"/>\n\
                    <string id=\"2\" name=\"name\" presence=\"optional\">\n\
                      <default value=\"\"/>\n\
                    </string>\n\
                  </group>\n\
                </group>\n\
                <group name=\"optionalOrderParameters68\" presence=\"optional\">\n\
                  <group name=\"nxOptionalOrderParameters\">\n\
                    <typeRef name=\"nxOptionalOrderParametersType\"/>\n\
                    <group name=\"conditionalProductID19\" presence=\"optional\">\n\
                      <group name=\"nxCurrencyID\">\n\
                        <typeRef name=\"nxCurrencyIDType\"/>\n\
                        <string id=\"2\" name=\"name\" presence=\"optional\">\n\
                          <default value=\"\"/>\n\
                        </string>\n\
                      </group>\n\
                    </group>\n\
                    <group name=\"conditionalProductID22\" presence=\"optional\">\n\
                      <group name=\"nxEquityID\">\n\
                        <typeRef name=\"nxEquityIDType\"/>\n\
                        <string id=\"2\" name=\"name\" presence=\"optional\">\n\
                          <default value=\"\"/>\n\
                        </string>\n\
                      </group>\n\
                    </group>\n\
                    <group name=\"conditionalProductID37\" presence=\"optional\">\n\
                      <group name=\"nxFutureID\">\n\
                        <typeRef name=\"nxFutureIDType\"/>\n\
                        <string id=\"2\" name=\"name\" presence=\"optional\">\n\
                          <default value=\"\"/>\n\
                        </string>\n\
                      </group>\n\
                    </group>\n\
                    <group name=\"conditionalProductID67\" presence=\"optional\">\n\
                      <group name=\"nxOptionID\">\n\
                        <typeRef name=\"nxOptionIDType\"/>\n\
                        <string id=\"2\" name=\"name\" presence=\"optional\">\n\
                          <default value=\"\"/>\n\
                        </string>\n\
                      </group>\n\
                    </group>\n\
                    <group name=\"conditionalProductID97\" presence=\"optional\">\n\
                      <group name=\"nxStrategyID\">\n\
                        <typeRef name=\"nxStrategyIDType\"/>\n\
                        <string id=\"2\" name=\"name\" presence=\"optional\">\n\
                          <default value=\"\"/>\n\
                        </string>\n\
                      </group>\n\
                    </group>\n\
                  </group>\n\
                </group>\n\
             </group>\n\
          </group>\n\
          <uInt32 id=\"9\" name=\"fillVolume\">\n\
            <default value=\"4294967294\"/>\n\
          </uInt32>\n\
        </template>\n\
      </templates>\n\
      ";

  void validateMessage1(Messages::Message & message, bool verbose = false)
  {
    if(verbose)
    {
      Messages::MessageFormatter formatter(std::cout);
      std::cout << std::endl << "Validating Message: " << std::endl;
      formatter.formatMessage(message);
      std::cout << std::endl << "---END---" << std::endl;
    }

    BOOST_CHECK_EQUAL(message.getApplicationType(), "nxOrderTransactionType");

    //"order71"
    Messages::FieldCPtr order71Field;
    BOOST_REQUIRE(message.getField("order71", order71Field));
    Messages::FieldSetCPtr order71Group = order71Field->toGroup();

    Messages::FieldCPtr nxOrderField;
    BOOST_REQUIRE(order71Group->getField("nxOrder", nxOrderField));
    Messages::FieldSetCPtr nxOrderGroup = nxOrderField->toGroup();

    Messages::FieldCPtr productID19Field;
    BOOST_REQUIRE(nxOrderGroup->getField("productID19", productID19Field));
//    BOOST_REQUIRE(order71Group->getField("productID19", productID19Field));
    Messages::FieldSetCPtr productID19Group = productID19Field->toGroup();

    Messages::FieldCPtr nxCurrencyIDField;
    BOOST_REQUIRE(productID19Group->getField("nxCurrencyID", nxCurrencyIDField));
    Messages::FieldSetCPtr nxCurrencyIDGroup = nxCurrencyIDField->toGroup();
    BOOST_CHECK_EQUAL(nxCurrencyIDGroup->getApplicationType(), "nxCurrencyIDType");
  }

  void populateMessage(Messages::Message &msg, bool verbose)
  {
    // Create the nxCurrencyID field set
    // contains name
    Messages::FieldSetPtr nxCurrencyIDFieldSet(new Messages::FieldSet(1));
    nxCurrencyIDFieldSet->addField(nameField, Messages::FieldAscii::create(""));

    // Create productID19 Field Set
    // contains nxCurrencyID
    Messages::FieldSetPtr productID19FieldSet(new Messages::FieldSet(2));
    productID19FieldSet->addField(nxCurrencyIDIdentity,
      Messages::FieldGroup::create(nxCurrencyIDFieldSet));

    // Create the conditionalProductID19 field set
    // contains nxCurrencyID
    Messages::FieldSetPtr conditionalProductID19FieldSet(new Messages::FieldSet(1));
    conditionalProductID19FieldSet->addField(nxCurrencyIDIdentity,
      Messages::FieldGroup::create(nxCurrencyIDFieldSet));

    // Create nxOptionalOrderParameters Field Set
    // contains conditionalProductID19
    // omits conditionalProductIDxx where xx=22,37, 67, 97
    Messages::FieldSetPtr nxOptionalOrderParametersFieldSet(new Messages::FieldSet(1));
    nxOptionalOrderParametersFieldSet->addField(conditionalProductID19Identity,
      Messages::FieldGroup::create(conditionalProductID19FieldSet));

    // Create optionalOrderParameters68 Field Set
    // contains nxOptionalOrderParameters
    Messages::FieldSetPtr optionalOrderParameters68FieldSet(new Messages::FieldSet(1));
    optionalOrderParameters68FieldSet->addField(nxOptionalOrderParametersIdentity,
      Messages::FieldGroup::create(nxOptionalOrderParametersFieldSet));

    // Create the nxOrder field set and add in the sub groups
    // contains productID19
    // contains optionalOrderParameters68
    Messages::FieldSetPtr nxOrderFieldSet(new Messages::FieldSet(2));
    nxOrderFieldSet->addField(productID19Identity, Messages::FieldGroup::create(productID19FieldSet));

    productID19FieldSet->addField(optionalOrderParameters68Identity,
      Messages::FieldGroup::create(optionalOrderParameters68FieldSet));


    // Create the order71FieldSet field set
    // contains nxOrder
    Messages::FieldSetPtr order71FieldSet(new Messages::FieldSet(1));
    order71FieldSet->addField(nxOrderIdentity, Messages::FieldGroup::create(nxOrderFieldSet));

    // Message
    // contains order71
    // contains fillVolume
    msg.addField( order71Identity, Messages::FieldGroup::create(order71FieldSet) );

    msg.addField( fillVolumeIdentity, Messages::FieldUInt32::create(4294967295) );
    if(verbose)
    {
      // Now let's see what it looks like "before"
      Messages::MessageFormatter formatter(std::cout);
      std::cout << std::endl << "Before Encoding: " << std::endl;
      formatter.formatMessage(msg);
      std::cout << std::endl << "--END--" << std::endl;
    }
  }

  void runTheTest(template_id_t templId, bool verbose)
  {
    std::stringstream templateStream(templateString);

    Codecs::XMLTemplateParser parser;
    Codecs::TemplateRegistryPtr templateRegistry =
      parser.parse(templateStream);

    if(verbose)
    {
      // for debugging: display the parsed templates
      templateRegistry->display(std::cout);
    }

    Messages::Message msg(templateRegistry->maxFieldCount());
    populateMessage(msg, verbose);

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
    validateMessage1(msgOut, verbose);

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
} // end of namespace

BOOST_AUTO_TEST_CASE(TestNestedGroupReferences)
{
  runTheTest(77, false);
}

BOOST_AUTO_TEST_CASE(TestNestingGroup)
{
  runTheTest(70, false);
}
