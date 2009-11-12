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

  BOOST_CHECK(templateRegistry);

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
  BOOST_CHECK(instruction->getFieldOp(fieldOp));
  // ...

  BOOST_CHECK(template203->getInstruction("UInt1", instruction));
  BOOST_CHECK_EQUAL(instruction->getId(), field_id_t());
  BOOST_CHECK_EQUAL(instruction->getName(), "UInt1");
  BOOST_CHECK(!instruction->getFieldOp(fieldOp));

  BOOST_CHECK(template203->getInstruction("Decimal1", instruction));
  BOOST_CHECK_EQUAL(instruction->getId(), field_id_t());
  BOOST_CHECK_EQUAL(instruction->getName(), "Decimal1");
  BOOST_CHECK(!instruction->getFieldOp(fieldOp));
  {
    Codecs::FieldInstructionCPtr exponent, mantissa;
    BOOST_CHECK(instruction->getMantissaInstruction(exponent));
    BOOST_CHECK(instruction->getMantissaInstruction(mantissa));
  }
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

  BOOST_CHECK(templateRegistry);

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
  BOOST_CHECK(instruction->getFieldOp(fieldOp));
  // ...

  BOOST_CHECK(template203->getInstruction("UInt1", instruction));
  BOOST_CHECK_EQUAL(instruction->getId(), field_id_t());
  BOOST_CHECK_EQUAL(instruction->getName(), "UInt1");
  BOOST_CHECK(!instruction->getFieldOp(fieldOp));

  BOOST_CHECK(template203->getInstruction("Decimal1", instruction));
  BOOST_CHECK_EQUAL(instruction->getId(), field_id_t());
  BOOST_CHECK_EQUAL(instruction->getName(), "Decimal1");
  BOOST_CHECK(!instruction->getFieldOp(fieldOp));
  {
    Codecs::FieldInstructionCPtr exponent, mantissa;
    BOOST_CHECK(instruction->getMantissaInstruction(exponent));
    BOOST_CHECK(instruction->getMantissaInstruction(mantissa));
  }
}

