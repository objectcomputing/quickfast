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

GenericSequenceBuilder::GenericSequenceBuilder(MessageBuilder * parent)
: parent_(parent)
{
}

GenericSequenceBuilder::~GenericSequenceBuilder()
{
}

const Messages::FieldSetPtr &
GenericSequenceBuilder::fieldSet()const
{
  if(!fieldSet_)
  {
    throw QuickFAST::UsageError("Coding Error: Illegal sequence of calls", "FieldSet not defined in Generic Sequence Builder.");
  }
  return fieldSet_;
}

void
GenericSequenceBuilder::initialize(
  const Messages::FieldIdentity & /*identity*/,
  const std::string & /*applicationType*/,
  const std::string & /*applicationTypeNamespace*/,
  size_t, /*size*/
  const Messages::FieldIdentity & lengthIdentity,
  size_t length
  )
{
  this->sequence_.reset(new Messages::Sequence(lengthIdentity, length));
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
  const Messages::FieldIdentity & identity,
  const Messages::FieldCPtr & value)
{
  fieldSet()->addField(identity, value);
}

Messages::MessageBuilder &
GenericSequenceBuilder::startMessage(
  const std::string & /*applicationType*/,
  const std::string & /*applicationTypeNamespace*/,
  size_t /*size*/)
{
  throw QuickFAST::UsageError("Internal Error", "GenericSequenceBuilder: Attempt to start nested message");
}


bool
GenericSequenceBuilder::endMessage(Messages::ValueMessageBuilder & /*messageBuilder*/)
{
  throw QuickFAST::UsageError("Internal Error", "GenericSequenceBuilder: Attempt to end nested message");
}

bool
GenericSequenceBuilder::ignoreMessage(ValueMessageBuilder & /*messageBuilder*/)
{
  throw QuickFAST::UsageError("Internal Error", "GenericSequenceBuilder: Attempt to ignore nested message");
}


Messages::MessageBuilder &
GenericSequenceBuilder::startSequence(
  const Messages::FieldIdentity & identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t fieldCount,
  const Messages::FieldIdentity & lengthIdentity,
  size_t length)
{
  // This is called to start a nested sequence
  // To see the start of THIS sequence, see initialize
  if(!sequenceBuilder_)
  {
    sequenceBuilder_.reset(new GenericSequenceBuilder(this));
  }
  sequenceBuilder_->initialize(
    identity,
    applicationType,
    applicationTypeNamespace,
    fieldCount,
    lengthIdentity,
    length);
  return *sequenceBuilder_;
}

void
GenericSequenceBuilder::endSequence(
  const Messages::FieldIdentity & identity,
  Messages::ValueMessageBuilder & /*sequenceBuilder*/)
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
    throw QuickFAST::UsageError("Internal Error", "GenericSequenceBuilder: ending an unstarted nested sequence");
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
  Messages::ValueMessageBuilder & /*entry*/)
{
  sequence_->addEntry(fieldSet_);
  fieldSet_.reset();
}


Messages::MessageBuilder &
GenericSequenceBuilder::startGroup(
  const Messages::FieldIdentity & identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  // NOTE THIS WILL BE CALLED TO START A NESTED GROUP
  if(!groupBuilder_)
  {
    groupBuilder_.reset(new GenericGroupBuilder(this));
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
  const Messages::FieldIdentity & identity,
  Messages::ValueMessageBuilder & /*entry*/)
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

bool
GenericSequenceBuilder::wantLog(unsigned short level)
{
  return parent_->wantLog(level);
}

bool
GenericSequenceBuilder::logMessage(unsigned short level, const std::string & logMessage)
{
  return parent_->logMessage(level, logMessage);
}

bool
GenericSequenceBuilder::reportDecodingError(const std::string & errorMessage)
{
  return parent_->reportDecodingError(errorMessage);
}

bool
GenericSequenceBuilder::reportCommunicationError(const std::string & errorMessage)
{
  return parent_->reportCommunicationError(errorMessage);
}

//////////////////////////
// GenericGroupBuilder


GenericGroupBuilder::GenericGroupBuilder(MessageBuilder * parent)
: parent_(parent)
{
}

GenericGroupBuilder::~GenericGroupBuilder()
{
}

const Messages::GroupPtr &
GenericGroupBuilder::groupPtr()const
{
  if(!group_)
  {
    throw QuickFAST::UsageError("Coding Error", "Generic Group Builder: group is undefined");
  }
  return group_;
}

void
GenericGroupBuilder::initialize(
  const Messages::FieldIdentity & identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  group_.reset(new Messages::FieldSet(size));
  group_->setApplicationType(applicationType, applicationTypeNamespace);
}

const std::string &
GenericGroupBuilder::getApplicationType()const
{
  return groupPtr()->getApplicationType();
}

const std::string &
GenericGroupBuilder::getApplicationTypeNs()const
{
  return groupPtr()->getApplicationTypeNs();
}

void
GenericGroupBuilder::addField(
  const Messages::FieldIdentity & identity,
  const Messages::FieldCPtr & value)
{
  groupPtr()->addField(identity, value);
}

Messages::MessageBuilder &
GenericGroupBuilder::startMessage(
  const std::string & /*applicationType*/,
  const std::string & /*applicationTypeNamespace*/,
  size_t /*size*/)
{
  throw QuickFAST::UsageError("Internal Error", "GenericGroupBuilder: Attempt to start nested message");
}

bool
GenericGroupBuilder::endMessage(
  Messages::ValueMessageBuilder & /*messageBuilder*/)
{
  throw QuickFAST::UsageError("Internal Error", "GenericGroupBuilder: Attempt to end nested message");
}

bool
GenericGroupBuilder::ignoreMessage(Messages::ValueMessageBuilder & /*messageBuilder*/)
{
  throw QuickFAST::UsageError("Internal Error", "GenericGroupBuilder: Attempt to ignore nested message");
}


Messages::MessageBuilder &
GenericGroupBuilder::startSequence(
  const Messages::FieldIdentity & identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t fieldCount,
  const Messages::FieldIdentity & lengthIdentity,
  size_t length)
{
  // This is called to start a nested sequence
  if(!sequenceBuilder_)
  {
    sequenceBuilder_.reset(new GenericSequenceBuilder(this));
  }
  sequenceBuilder_->initialize(
    identity,
    applicationType,
    applicationTypeNamespace,
    fieldCount,
    lengthIdentity,
    length);
  return *sequenceBuilder_;
}

void
GenericGroupBuilder::endSequence(
   const Messages::FieldIdentity & identity,
  Messages::ValueMessageBuilder & /*sequenceBuilder*/)
{
  // Note this will be called to end a nested sequence
  if(sequenceBuilder_)
  {
    groupPtr()->addField(
      identity,
      QuickFAST::Messages::FieldSequence::create(sequenceBuilder_->getSequence())
      );
    sequenceBuilder_->reset();
  }
  else
  {
    throw QuickFAST::UsageError("Coding Error", "GenericGroupBuilder: ending an unstarted nested sequence");
  }
}

Messages::MessageBuilder &
GenericGroupBuilder::startSequenceEntry(
  const std::string & /*applicationType*/,
  const std::string & /*applicationTypeNamespace*/,
  size_t /*fieldCount*/)

{
  throw QuickFAST::UsageError("Internal Error", "GenericGroupBuilder: Start sequence operation applied to group.");
}

void
GenericGroupBuilder::endSequenceEntry(Messages::ValueMessageBuilder & /*entry*/)
{
  throw QuickFAST::UsageError("Internal Error", "GenericGroupBuilder: End sequence operation applied to group.");
}


Messages::MessageBuilder &
GenericGroupBuilder::startGroup(
  const Messages::FieldIdentity & identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  // NOTE THIS WILL BE CALLED TO START A NESTED GROUP
  if(!groupBuilder_)
  {
    groupBuilder_.reset(new GenericGroupBuilder(this));
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
  const Messages::FieldIdentity & identity,
  Messages::ValueMessageBuilder & /*entry*/)
{
  /// Note this will be called to end a nested group
  groupPtr()->addField(
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

bool
GenericGroupBuilder::wantLog(unsigned short level)
{
  return parent_->wantLog(level);
}

bool
GenericGroupBuilder::logMessage(unsigned short level, const std::string & logMessage)
{
  return parent_->logMessage(level, logMessage);
}

bool
GenericGroupBuilder::reportDecodingError(const std::string & errorMessage)
{
  return parent_->reportDecodingError(errorMessage);
}

bool
GenericGroupBuilder::reportCommunicationError(const std::string & errorMessage)
{
  return parent_->reportCommunicationError(errorMessage);
}

//////////////////////////
// GenericMessageBuilder
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4355) // C4355: 'this' : used in base member initializer list
#endif

GenericMessageBuilder::GenericMessageBuilder(MessageConsumer & consumer)
: consumer_(consumer)
, sequenceBuilder_(this)
, groupBuilder_(this)
{
}

#ifdef _MSC_VER
# pragma warning(pop)
#endif


GenericMessageBuilder::~GenericMessageBuilder()
{
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
  const Messages::FieldIdentity & identity,
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
GenericMessageBuilder::endMessage(Messages::ValueMessageBuilder &)
{
  ///////////////////////////////
  // This is where the message is
  // bassed to the MessageConsumer
  bool more = consumer_.consumeMessage(*message());

  // Once it's consumed, the message is no longer needed.
  message_.reset();
  return more;
}

bool
GenericMessageBuilder::ignoreMessage(Messages::ValueMessageBuilder & /*messageBuilder*/)
{
  return true;
}

Messages::MessageBuilder &
GenericMessageBuilder::startSequence(
  const Messages::FieldIdentity & identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t fieldCount,
  const Messages::FieldIdentity & lengthIdentity,
  size_t length)
{
  sequenceBuilder_.initialize(
    identity,
    applicationType,
    applicationTypeNamespace,
    fieldCount,
    lengthIdentity,
    length);
  return sequenceBuilder_;
}

void
GenericMessageBuilder::endSequence(
   const Messages::FieldIdentity & identity,
   Messages::ValueMessageBuilder &)
{
  message()->addField(
    identity,
    QuickFAST::Messages::FieldSequence::create(sequenceBuilder_.getSequence())
    );
  sequenceBuilder_.reset();
}

Messages::MessageBuilder &
GenericMessageBuilder::startSequenceEntry(
  const std::string & /*applicationType*/,
  const std::string & /*applicationTypeNamespace*/,
  size_t /*size*/)
{
  throw QuickFAST::UsageError("Internal Error", "GenericGroupBuilder: Attempt to add sequence entry to message");
}

void
GenericMessageBuilder::endSequenceEntry(Messages::ValueMessageBuilder & /*entry*/)
{
  throw QuickFAST::UsageError("Internal Error", "GenericGroupBuilder: Attempt to end sequence entry in a message");
}

Messages::MessageBuilder &
GenericMessageBuilder::startGroup(
  const Messages::FieldIdentity & identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  groupBuilder_.initialize(
    identity,
    applicationType,
    applicationTypeNamespace,
    size);
  return groupBuilder_;
}

void
GenericMessageBuilder::endGroup(
  const Messages::FieldIdentity & identity,
  Messages::ValueMessageBuilder & /*groupBuilder*/)
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
    throw QuickFAST::UsageError("Coding Error", "GenericMessageBuilder: message not defined when needed.");
  }
  return message_;
}

bool
GenericMessageBuilder::wantLog(unsigned short level)
{
  return consumer_.wantLog(level);
}

bool
GenericMessageBuilder::logMessage(unsigned short level, const std::string & logMessage)
{
  return consumer_.logMessage(level, logMessage);
}

bool
GenericMessageBuilder::reportDecodingError(const std::string & errorMessage)
{
  return consumer_.reportDecodingError(errorMessage);
}

bool
GenericMessageBuilder::reportCommunicationError(const std::string & errorMessage)
{
  return consumer_.reportCommunicationError(errorMessage);
}

