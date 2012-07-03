// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Examples/ExamplesPch.h>
#include "ValueToFix.h"
#include <Messages/FieldIdentity.h>
#include <Common/Decimal.h>

using namespace QuickFAST;
using namespace Examples;

ValueToFix::ValueToFix(std::ostream & out, const char * recordSeparator)
  : out_(out)
  , recordSeparator_(recordSeparator)
  , logLevel_(Common::Logger::QF_LOG_WARNING)
{
}

ValueToFix::~ValueToFix()
{
}

bool
ValueToFix::wantLog(unsigned short level)
{
  return level <= logLevel_;
}

bool
ValueToFix::logMessage(unsigned short level, const std::string & logMessage)
{
  if(level <= logLevel_)
  {
    std::cerr << logMessage << std::endl;
  }
  return true;
}


bool
ValueToFix::reportDecodingError(const std::string & errorMessage)
{
  std::cerr << "Decoding error: " << errorMessage << std::endl;
  return false;
}

bool
ValueToFix::reportCommunicationError(const std::string & errorMessage)
{
  //out_
  std::cerr << "Communication error: " << errorMessage << std::endl;
  return false;
}

const std::string &
ValueToFix::getApplicationType()const
{
  return applicationType_;
}

const std::string &
ValueToFix::getApplicationTypeNs()const
{
  return applicationTypeNamespace_;
}

void
ValueToFix::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int64 value)
{
  out_ << identity.id() << '=' << value << '\x01';
}

void
ValueToFix::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint64 value)
{
  out_ << identity.id() << '=' << value << '\x01';
}

void
ValueToFix::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int32 value)
{
  out_ << identity.id() << '=' << value << '\x01';
}

void
ValueToFix::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint32 value)
{
  out_ << identity.id() << '=' << value << '\x01';
}

void
ValueToFix::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int16 value)
{
  out_ << identity.id() << '=' << value << '\x01';
}

void
ValueToFix::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uint16 value)
{
  out_ << identity.id() << '=' << value << '\x01';
}

void
ValueToFix::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const int8 value)
{
  out_ << identity.id() << '=' << (short)(value) << '\x01';
}

void
ValueToFix::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const uchar value)
{
  out_ << identity.id() << '=' << (unsigned short)(value) << '\x01';
}

void
ValueToFix::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const Decimal& value)
{
  std::string v;
  value.toString(v);
  out_ << identity.id() << '=' << v << '\x01';
}

void
ValueToFix::addValue(const Messages::FieldIdentity & identity, ValueType::Type type, const unsigned char * value, size_t length)
{
  out_ << identity.id() << '=' << std::string(reinterpret_cast<const char *>(value), length) << '\x01';
}

Messages::ValueMessageBuilder &
ValueToFix::startMessage(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  applicationType_ = applicationType;
  applicationTypeNamespace_ = applicationTypeNamespace;
  return *this;
}

bool
ValueToFix::endMessage(Messages::ValueMessageBuilder & messageBuilder)
{
  out_ << recordSeparator_;
  return true;
}

bool
ValueToFix::ignoreMessage(Messages::ValueMessageBuilder & messageBuilder)
{
  return true;
}

Messages::ValueMessageBuilder &
ValueToFix::startSequence(
  const Messages::FieldIdentity & identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t fieldCount,
  const Messages::FieldIdentity & lengthIdentity,
  size_t length)
{
  return *this;
}

void
ValueToFix::endSequence(
  const Messages::FieldIdentity & identity,
  Messages::ValueMessageBuilder & sequenceBuilder)
{
}

Messages::ValueMessageBuilder &
ValueToFix::startSequenceEntry(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  return *this;
}

void
ValueToFix::endSequenceEntry(Messages::ValueMessageBuilder & entry)
{
}

Messages::ValueMessageBuilder &
ValueToFix::startGroup(
  const Messages::FieldIdentity & identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  return *this;
}

void
ValueToFix::endGroup(
  const Messages::FieldIdentity & identity,
  Messages::ValueMessageBuilder & groupBuilder)
{
}


