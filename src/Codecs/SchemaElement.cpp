// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "SchemaElement.h"
#include <Common/Exceptions.h>
#include <Codecs/FieldOp.h>
using namespace QuickFAST;
using namespace QuickFAST::Codecs;

void
SchemaElement::finalize(TemplateRegistry & templateRegistry)
{
  // nothing to do by default
}

void
SchemaElement::addTemplate(TemplatePtr value)
{
  throw TemplateDefinitionError("<template> not allowed in this context.");
}

void
SchemaElement::setApplicationType(const std::string & /*type*/, const std::string & /*ns*/)
{
  throw TemplateDefinitionError("<typeRef> not allowed in this context.");
}
void
SchemaElement::setFieldOp(FieldOpPtr fieldOp)
{
  std::stringstream msg;
  msg << "Field operation <" << fieldOp->opName(fieldOp->opType()) << "> not allowed in this context.";
  throw TemplateDefinitionError(msg.str());
}
void
SchemaElement::addLengthInstruction(FieldInstructionPtr & /*field*/)
{
  throw TemplateDefinitionError("<length> not allowed in this context.");
}


void
SchemaElement::addInstruction(FieldInstructionPtr & /*field*/)
{
  throw TemplateDefinitionError("Field instruction not allowed in this context.");
}
void
SchemaElement::setMantissaInstruction(FieldInstructionPtr mantissa)
{
  throw TemplateDefinitionError("Mantissa applies only to Decimal Field instruction.");
}

bool
SchemaElement::getMantissaInstruction(FieldInstructionCPtr & /*mantissa*/) const
{
  throw TemplateDefinitionError("Mantissa applies only to Decimal Field instruction.");
}

void
SchemaElement::setExponentInstruction(FieldInstructionPtr exponent)
{
  throw TemplateDefinitionError("Exponent applies only to Decimal Field instruction.");
}

bool
SchemaElement::getExponentInstruction(FieldInstructionCPtr & /*exponent*/) const
{
  throw TemplateDefinitionError("Exponent applies only to Decimal Field instruction.");
}
