// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "GenericMessageBuilder.h"
#include <Messages/Message.h>
#include <Messages/Sequence.h>
#include <Messages/Group.h>
#include <Messages/FieldSequence.h>
#include <Messages/FieldGroup.h>
#include <Common/Exceptions.h>

using namespace QuickFAST;
using namespace Codecs;

//////////////////////////
// GenericSequenceBuilder

GenericSequenceBuilder::~GenericSequenceBuilder()
{
}

const Messages::FieldSetPtr &
GenericSequenceBuilder::fieldSet()const
{
  if(!fieldSet_)
  {
    throw QuickFAST::UsageError("Illegal sequence of calls", "Generic Sequence Builder.");
  }
  return fieldSet_;
}

void
GenericSequenceBuilder::initialize(
  Messages::FieldIdentityCPtr identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  this->sequence_.reset(new Messages::Sequence);
}

size_t
GenericSequenceBuilder::size()const
{
  return fieldSet()->size();
}

const std::string &
GenericSequenceBuilder::getApplicationType()const
{
  return fieldSet()->getApplicationType();
}

const std::string &
GenericSequenceBuilder::getApplicationTypeNs()const
{
  return fieldSet()->getApplicationTypeNs();
}

void
GenericSequenceBuilder::addField(
  const Messages::FieldIdentityCPtr & identity,
  const Messages::FieldCPtr & value)
{
  fieldSet()->addField(identity, value);
}

Messages::MessageBuilder &
GenericSequenceBuilder::startMessage(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  throw QuickFAST::UsageError("Attempt to build message.", "GenericSequenceBuilder");
}


bool
GenericSequenceBuilder::endMessage(Messages::MessageBuilder & messageBuilder)
{
  throw QuickFAST::UsageError("Attempt to build message.", "GenericSequenceBuilder");
}


Messages::MessageBuilder &
GenericSequenceBuilder::startSequence(
  Messages::FieldIdentityCPtr identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  // This is called to start a nested sequence
  // To see the start of THIS sequence, see initialize
  if(!sequenceBuilder_)
  {
    sequenceBuilder_.reset(new GenericSequenceBuilder);
  }
  sequenceBuilder_->initialize(
    identity,
    applicationType,
    applicationTypeNamespace,
    size);
  return *sequenceBuilder_;
}

void
GenericSequenceBuilder::endSequence(
  Messages::FieldIdentityCPtr identity,
  Messages::MessageBuilder & sequenceBuilder)
{
  // Note this will be called to end a nested sequence
  if(sequenceBuilder_)
  {
    fieldSet()->addField(
      identity,
      QuickFAST::Messages::FieldSequence::create(sequenceBuilder_->getSequence())
      );
    sequenceBuilder_->reset();
  }
  else
  {
    throw QuickFAST::UsageError("ending an unstarted nested sequence", "GenericSequenceBuilder");
  }
}

Messages::MessageBuilder &
GenericSequenceBuilder::startSequenceEntry(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  fieldSet_.reset(new Messages::FieldSet(size));
  fieldSet_->setApplicationType(
    applicationType,
    applicationTypeNamespace);
  return *this;
}


void
GenericSequenceBuilder::endSequenceEntry(
  Messages::MessageBuilder & entry)
{
  sequence_->addEntry(fieldSet_);
  fieldSet_.reset();
}


Messages::MessageBuilder &
GenericSequenceBuilder::startGroup(
  Messages::FieldIdentityCPtr identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  // NOTE THIS WILL BE CALLED TO START A NESTED GROUP
  if(!groupBuilder_)
  {
    groupBuilder_.reset(new GenericGroupBuilder);
  }
  groupBuilder_->initialize(
    identity,
    applicationType,
    applicationTypeNamespace,
    size);
  return *groupBuilder_;
}

void
GenericSequenceBuilder::endGroup(
  Messages::FieldIdentityCPtr identity,
  Messages::MessageBuilder & entry)
{
  /// Note this will be called to end a nested group
  fieldSet()->addField(
    identity,
    QuickFAST::Messages::FieldGroup::create(groupBuilder_->getGroup())
    );
  groupBuilder_->reset();
}

const Messages::SequencePtr &
GenericSequenceBuilder::getSequence()const
{
  return sequence_;
}

void
GenericSequenceBuilder::reset()
{
  sequence_.reset();
}

//////////////////////////
// GenericGroupBuilder


GenericGroupBuilder::~GenericGroupBuilder()
{
}

const Messages::FieldSetPtr &
GenericGroupBuilder::fieldSet()const
{
  if(!fieldSet_)
  {
    throw QuickFAST::UsageError("Illegal sequence of calls", "Generic Group Builder.");
  }
  return fieldSet_;
}

void
GenericGroupBuilder::initialize(
  Messages::FieldIdentityCPtr identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  fieldSet_.reset(new Messages::FieldSet(size));
  fieldSet_->setApplicationType(applicationType, applicationTypeNamespace);
}

size_t
GenericGroupBuilder::size()const
{
  return fieldSet()->size();
}

const std::string &
GenericGroupBuilder::getApplicationType()const
{
  return fieldSet()->getApplicationType();
}

const std::string &
GenericGroupBuilder::getApplicationTypeNs()const
{
  return fieldSet()->getApplicationTypeNs();
}

void
GenericGroupBuilder::addField(
  const Messages::FieldIdentityCPtr & identity,
  const Messages::FieldCPtr & value)
{
  fieldSet()->addField(identity, value);
}

Messages::MessageBuilder &
GenericGroupBuilder::startMessage(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  throw QuickFAST::UsageError("Attempt to build message.", "GenericGroupBuilder");
}

bool
GenericGroupBuilder::endMessage(
  Messages::MessageBuilder & messageBuilder)
{
  throw QuickFAST::UsageError("Attempt to build message.", "GenericGroupBuilder");
}


Messages::MessageBuilder &
GenericGroupBuilder::startSequence(
  Messages::FieldIdentityCPtr identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  // This is called to start a nested sequence
  if(!sequenceBuilder_)
  {
    sequenceBuilder_.reset(new GenericSequenceBuilder);
  }
  sequenceBuilder_->initialize(
    identity,
    applicationType,
    applicationTypeNamespace,
    size);
  return *sequenceBuilder_;
}

void
GenericGroupBuilder::endSequence(
   Messages::FieldIdentityCPtr identity,
  Messages::MessageBuilder & sequenceBuilder)
{
  // Note this will be called to end a nested sequence
  if(sequenceBuilder_)
  {
    fieldSet()->addField(
      identity,
      QuickFAST::Messages::FieldSequence::create(sequenceBuilder_->getSequence())
      );
    sequenceBuilder_->reset();
  }
  else
  {
    throw QuickFAST::UsageError("ending an unstarted nested sequence", "GenericSequenceBuilder");
  }
}

Messages::MessageBuilder &
GenericGroupBuilder::startSequenceEntry(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  throw QuickFAST::UsageError("Sequence operation applied to group.", "GenericGroupBuilder");
}

void
GenericGroupBuilder::endSequenceEntry(Messages::MessageBuilder & entry)
{
  throw QuickFAST::UsageError("Sequence operation applied to group.", "GenericGroupBuilder");
}


Messages::MessageBuilder &
GenericGroupBuilder::startGroup(
  Messages::FieldIdentityCPtr identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  // NOTE THIS WILL BE CALLED TO START A NESTED GROUP
  if(!groupBuilder_)
  {
    groupBuilder_.reset(new GenericGroupBuilder);
  }
  groupBuilder_->initialize(
    identity,
    applicationType,
    applicationTypeNamespace,
    size);
  return *groupBuilder_;
}

void
GenericGroupBuilder::endGroup(
  Messages::FieldIdentityCPtr identity,
  Messages::MessageBuilder & entry)
{
  /// Note this will be called to end a nested group
  fieldSet()->addField(
    identity,
    QuickFAST::Messages::FieldGroup::create(groupBuilder_->getGroup())
    );
  groupBuilder_->reset();
}

const Messages::GroupPtr &
GenericGroupBuilder::getGroup()const
{
  return group_;
}

void
GenericGroupBuilder::reset()
{
  group_.reset();
}


//////////////////////////
// GenericMessageBuilder

GenericMessageBuilder::GenericMessageBuilder(MessageConsumer & consumer)
: consumer_(consumer)
{
}

GenericMessageBuilder::~GenericMessageBuilder()
{
}

size_t
GenericMessageBuilder::size()const
{
  return message()->size();
}

const std::string &
GenericMessageBuilder::getApplicationType()const
{
  return message()->getApplicationType();
}

const std::string &
GenericMessageBuilder::getApplicationTypeNs()const
{
  return message()->getApplicationTypeNs();
}

void
GenericMessageBuilder::addField(
  const Messages::FieldIdentityCPtr & identity,
  const Messages::FieldCPtr & value)
{
  message()->addField(identity, value);
}

Messages::MessageBuilder &
GenericMessageBuilder::startMessage(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  message_.reset(new Messages::Message(size));
  message_->setApplicationType(applicationType, applicationTypeNamespace);
  return *this;
}

bool
GenericMessageBuilder::endMessage(Messages::MessageBuilder &)
{
  bool more = consumer_.consumeMessage(*message());
  message_.reset();
  return more;
}

Messages::MessageBuilder &
GenericMessageBuilder::startSequence(
  Messages::FieldIdentityCPtr identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  sequenceBuilder_.initialize(
    identity,
    applicationType,
    applicationTypeNamespace,
    size);
  return sequenceBuilder_;
}

void
GenericMessageBuilder::endSequence(
   Messages::FieldIdentityCPtr identity,
   Messages::MessageBuilder &)
{
  message()->addField(
    identity,
    QuickFAST::Messages::FieldSequence::create(sequenceBuilder_.getSequence())
    );
  sequenceBuilder_.reset();
}

Messages::MessageBuilder &
GenericMessageBuilder::startSequenceEntry(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  throw QuickFAST::UsageError("Attempt to add sequence entry to message", "Generic Message Builder");
}

void
GenericMessageBuilder::endSequenceEntry(Messages::MessageBuilder & entry)
{
  throw QuickFAST::UsageError("Attempt to add sequence entry to message", "Generic Message Builder");
}

Messages::MessageBuilder &
GenericMessageBuilder::startGroup(
  Messages::FieldIdentityCPtr identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  groupBuilder_.startGroup(
    identity,
    applicationType,
    applicationTypeNamespace,
    size);
  return groupBuilder_;
}

void
GenericMessageBuilder::endGroup(
  Messages::FieldIdentityCPtr identity,
  Messages::MessageBuilder & groupBuilder)
{
  message()->addField(
    identity,
    QuickFAST::Messages::FieldGroup::create(groupBuilder_.getGroup())
    );
  groupBuilder_.reset();
}

const Messages::MessagePtr &
GenericMessageBuilder::message()const
{
  if(!message_)
  {
    throw QuickFAST::UsageError("Illegal sequence of calls", "Generic Message Builder.");
  }
  return message_;
}
