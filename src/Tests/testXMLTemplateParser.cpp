// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>
#include <Codecs/XMLTemplateParser.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/Template.h>
#include <Codecs/FieldInstruction.h>
#include <sstream>

using namespace QuickFAST;
BOOST_AUTO_TEST_CASE(testXMLTemplateParser)
{
  BOOST_CHECKPOINT("Start testXMLTemplateParser");
  Codecs::XMLTemplateParser parser;
  std::stringstream myDocument;

  myDocument << "<templates>" << std::endl;
  myDocument << "  <template name='Test1' id='203'>" << std::endl;
  myDocument << "    <typeRef name='MarketTest' />" << std::endl;
  myDocument << "    <int32 name='Int1' id='23'>" << std::endl;
  myDocument << "      <constant value='4' />" << std::endl;
  myDocument << "    </int32>" << std::endl;
  myDocument << "    <uInt64 name='UInt1' />" << std::endl;
  myDocument << "    <decimal name='Decimal1' presence='optional'>" << std::endl;
  myDocument << "      <exponent>" << std::endl;
  myDocument << "        <copy />" << std::endl;
  myDocument << "      </exponent>" << std::endl;
  myDocument << "      <mantissa>" << std::endl;
  myDocument << "        <delta />" << std::endl;
  myDocument << "      </mantissa>" << std::endl;
  myDocument << "    </decimal>" << std::endl;
  myDocument << "    <uInt8 name=\"Issue22\" id=\"Issue22\">" << std::endl;
  myDocument << "      <default value=\"20\"/>" << std::endl;
  myDocument << "    </uInt8>" << std::endl;
  myDocument << "  </template>" << std::endl;
  myDocument << "</templates>" << std::endl;

  Codecs::TemplateRegistryPtr templateRegistry =
    parser.parse(myDocument);

  BOOST_REQUIRE(templateRegistry);

  Codecs::TemplateCPtr template203;
  BOOST_CHECK(!templateRegistry->getTemplate(205, template203));
  BOOST_CHECK(templateRegistry->getTemplate(203, template203));
  BOOST_CHECK_EQUAL(template203->getId(), template_id_t(203));
  BOOST_CHECK_EQUAL(template203->getTemplateName(), "Test1");
  Codecs::FieldInstructionCPtr instruction;
  BOOST_CHECK(!template203->getInstruction("Int2", instruction));
  BOOST_CHECK(template203->getInstruction("Int1", instruction));
  BOOST_CHECK_EQUAL(instruction->getId(), field_id_t("23"));
  BOOST_CHECK_EQUAL(instruction->getName(), "Int1");
  Codecs::FieldOpCPtr fieldOp;
  fieldOp = instruction->getFieldOp();
  // ...

  BOOST_CHECK(template203->getInstruction("UInt1", instruction));
  BOOST_CHECK_EQUAL(instruction->getId(), field_id_t());
  BOOST_CHECK_EQUAL(instruction->getName(), "UInt1");
  fieldOp = instruction->getFieldOp();

  BOOST_CHECK(template203->getInstruction("Decimal1", instruction));
  BOOST_CHECK_EQUAL(instruction->getId(), field_id_t());
  BOOST_CHECK_EQUAL(instruction->getName(), "Decimal1");
  fieldOp = instruction->getFieldOp();
  Codecs::FieldInstructionCPtr exponent;
  BOOST_CHECK(instruction->getMantissaInstruction(exponent));
  Codecs::FieldInstructionCPtr mantissa;
  BOOST_CHECK(instruction->getMantissaInstruction(mantissa));
}

BOOST_AUTO_TEST_CASE(testXMLTemplateParser2)
{
  Codecs::XMLTemplateParser parser;
  std::stringstream myDocument;

  // Test without <templates>
  myDocument << "  <template name='Test1' id='203'>" << std::endl;
  myDocument << "    <typeRef name='MarketTest' />" << std::endl;
  myDocument << "    <int32 name='Int1' id='23'>" << std::endl;
  myDocument << "      <constant value='4' />" << std::endl;
  myDocument << "    </int32>" << std::endl;
  myDocument << "    <uInt64 name='UInt1' />" << std::endl;
  myDocument << "    <decimal name='Decimal1' presence='optional'>" << std::endl;
  myDocument << "      <exponent>" << std::endl;
  myDocument << "        <copy />" << std::endl;
  myDocument << "      </exponent>" << std::endl;
  myDocument << "      <mantissa>" << std::endl;
  myDocument << "        <delta />" << std::endl;
  myDocument << "      </mantissa>" << std::endl;
  myDocument << "    </decimal>" << std::endl;
  myDocument << "  </template>" << std::endl;

  Codecs::TemplateRegistryPtr templateRegistry =
    parser.parse(myDocument);

  BOOST_REQUIRE(templateRegistry);

  Codecs::TemplateCPtr template203;
  BOOST_CHECK(!templateRegistry->getTemplate(205, template203));
  BOOST_REQUIRE(templateRegistry->getTemplate(203, template203));
  BOOST_CHECK_EQUAL(template203->getId(), template_id_t(203));
  BOOST_CHECK_EQUAL(template203->getTemplateName(), "Test1");
  Codecs::FieldInstructionCPtr instruction;
  BOOST_CHECK(!template203->getInstruction("Int2", instruction));
  BOOST_CHECK(template203->getInstruction("Int1", instruction));
  BOOST_CHECK_EQUAL(instruction->getId(), field_id_t("23"));
  BOOST_CHECK_EQUAL(instruction->getName(), "Int1");
  Codecs::FieldOpCPtr fieldOp;
  fieldOp = instruction->getFieldOp();
  // ...

  BOOST_REQUIRE(template203->getInstruction("UInt1", instruction));
  BOOST_CHECK_EQUAL(instruction->getId(), field_id_t());
  BOOST_CHECK_EQUAL(instruction->getName(), "UInt1");
  fieldOp = instruction->getFieldOp();

  BOOST_REQUIRE(template203->getInstruction("Decimal1", instruction));
  BOOST_CHECK_EQUAL(instruction->getId(), field_id_t());
  BOOST_CHECK_EQUAL(instruction->getName(), "Decimal1");
  fieldOp = instruction->getFieldOp();
  Codecs::FieldInstructionCPtr exponent;
  BOOST_CHECK(instruction->getMantissaInstruction(exponent));
  Codecs::FieldInstructionCPtr mantissa;
  BOOST_CHECK(instruction->getMantissaInstruction(mantissa));
}


BOOST_AUTO_TEST_CASE(testXMLTemplateParser3)
{
  Codecs::XMLTemplateParser parser;
  std::stringstream myDocument;

  myDocument << "<template name=\"MarketUpdate\" id=\"1\">" << std::endl;
  myDocument << "  <uInt32 name=\"MsgType\" id=\"0\" presence=\"mandatory\"> <constant value=\"702\"/> </uInt32>" << std::endl;
  myDocument << "  <uInt32 name=\"SymbolIndex\" id=\"1\" presence=\"mandatory\"> <default value=\"0\"/> </uInt32>" << std::endl;
  myDocument << "  <uInt32 name=\"SecurityIDSource\" id=\"2\" presence=\"mandatory\"> <default value=\"0\"/> </uInt32>" << std::endl;
  myDocument << "  <string name=\"SecurityID\" id=\"3\" presence=\"mandatory\"> <default value=\"               \"/> </string>" << std::endl;
  myDocument << "  <uInt32 name=\"SourceTime\" id=\"4\" presence=\"mandatory\"> <delta/> </uInt32>" << std::endl;
  myDocument << "  <uInt32 name=\"SeriesSequenceNumber\" id=\"5\" presence=\"mandatory\"> </uInt32>" << std::endl;
  myDocument << "  <uInt32 name=\"SnapshotFlag\" id=\"6\" presence=\"mandatory\"> <default value=\"0\"/> </uInt32>" << std::endl;
  myDocument << "  <sequence name=\"UpdateSubMsg\">" << std::endl;
  myDocument << "    <length name=\"UpdateCount\" id=\"7\"> <default value=\"1\"/> </length>" << std::endl;
  myDocument << "    <uInt32 name=\"UpdateType\" id=\"8\" presence=\"mandatory\"> </uInt32>" << std::endl;
  myDocument << "    <int32 name=\"Price\" id=\"9\" presence=\"optional\"> <delta/> </int32>" << std::endl;
  myDocument << "    <uInt32 name=\"Volume\" id=\"10\" presence=\"mandatory\"> </uInt32>" << std::endl;
  myDocument << "  </sequence>" << std::endl;
  myDocument << "</template>" << std::endl;

  Codecs::TemplateRegistryPtr templateRegistry =
    parser.parse(myDocument);

  BOOST_REQUIRE(templateRegistry);

  Codecs::TemplateCPtr template1;
  BOOST_REQUIRE(templateRegistry->getTemplate(1, template1));
  BOOST_REQUIRE_EQUAL(template1->size(), 8);
  size_t fieldIndex = 0;
  Codecs::FieldOpCPtr fieldOp;
  Codecs::FieldInstructionCPtr instruction;
  //MsgType
  instruction = template1->getInstruction(fieldIndex++);
  fieldOp = instruction->getFieldOp();
  BOOST_CHECK(!fieldOp->usesPresenceMap(true));
  BOOST_CHECK_EQUAL(instruction->getPresenceMapBitsUsed(), 0);

  //SymbolIndex
  instruction = template1->getInstruction(fieldIndex++);
  fieldOp = instruction->getFieldOp();
  BOOST_CHECK( fieldOp->usesPresenceMap(true));
  BOOST_CHECK_EQUAL(instruction->getPresenceMapBitsUsed(), 1);

  //SecurityIDSource
  instruction = template1->getInstruction(fieldIndex++);
  fieldOp = instruction->getFieldOp();
  BOOST_CHECK( fieldOp->usesPresenceMap(true));
  BOOST_CHECK_EQUAL(instruction->getPresenceMapBitsUsed(), 1);

  //SecurityID
  instruction = template1->getInstruction(fieldIndex++);
  fieldOp = instruction->getFieldOp();
  BOOST_CHECK( fieldOp->usesPresenceMap(true));
  BOOST_CHECK_EQUAL(instruction->getPresenceMapBitsUsed(), 1);

  //SourceTime
  instruction = template1->getInstruction(fieldIndex++);
  fieldOp = instruction->getFieldOp();
  BOOST_CHECK(!fieldOp->usesPresenceMap(true));
  BOOST_CHECK_EQUAL(instruction->getPresenceMapBitsUsed(), 0);

  //SeriesSequenceNumber
  instruction = template1->getInstruction(fieldIndex++);
  fieldOp = instruction->getFieldOp();
  BOOST_CHECK(!fieldOp->usesPresenceMap(true));
  BOOST_CHECK_EQUAL(instruction->getPresenceMapBitsUsed(), 0);

  //SnapshotFlag
  instruction = template1->getInstruction(fieldIndex++);
  fieldOp = instruction->getFieldOp();
  BOOST_CHECK( fieldOp->usesPresenceMap(true));
  BOOST_CHECK_EQUAL(instruction->getPresenceMapBitsUsed(), 1);

  //UpdateSubMsg
  instruction = template1->getInstruction(fieldIndex++);
  fieldOp = instruction->getFieldOp();
  BOOST_CHECK( fieldOp->usesPresenceMap(true));
  BOOST_CHECK_EQUAL(instruction->getPresenceMapBitsUsed(), 1);

  Codecs::SegmentBodyPtr segment;
  BOOST_REQUIRE(instruction->getSegmentBody(segment));

  // the pmap bit used by the Length field should be counted in the
  // enclosing segment.  The instructions in this segment use no
  // presence map bits.
  BOOST_CHECK_EQUAL(segment->presenceMapBitCount(), 0);

  size_t segmentIndex = 0;
  //UpdateType
  instruction = segment->getInstruction(segmentIndex++);
  fieldOp = instruction->getFieldOp();
  BOOST_CHECK(!fieldOp->usesPresenceMap(true));
  BOOST_CHECK_EQUAL(instruction->getPresenceMapBitsUsed(), 0);

  //Price
  instruction = segment->getInstruction(segmentIndex++);
  fieldOp = instruction->getFieldOp();
  BOOST_CHECK(!fieldOp->usesPresenceMap(false));
  BOOST_CHECK_EQUAL(instruction->getPresenceMapBitsUsed(), 0);

  //Volume
  instruction = segment->getInstruction(segmentIndex++);
  fieldOp = instruction->getFieldOp();
  BOOST_CHECK(!fieldOp->usesPresenceMap(true));
  BOOST_CHECK_EQUAL(instruction->getPresenceMapBitsUsed(), 0);

}



