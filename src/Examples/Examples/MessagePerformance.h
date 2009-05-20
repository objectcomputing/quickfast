// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGEPERFORMANCE_H
#define MESSAGEPERFORMANCE_H
#include <Codecs/MessageConsumer.h>
#include <Messages/Field_fwd.h>
#include <Messages/FieldIdentity.h>
namespace QuickFAST{
  namespace Examples{

    /// @brief A message consumer that attempts to produce a human readable version
    /// of a message that has been decoded by QuickFAST.
    class MessagePerformance : public Codecs::MessageConsumer
    {
    public:
      /// @brief Construct given a ostream to which to write the interpreted results.
      MessagePerformance(size_t recordLimit, size_t recordDuplicate);
      virtual ~MessagePerformance();

      ////////////////////////////
      // Implement MessageConsumer
      virtual bool consumeMessage(Messages::Message & message);
      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);
      virtual void decodingStopped();

      size_t getMessageCount() const
      {
        return recordCount_;
      }
    private:
      void retrieveMessage(const Messages::Message & message);
      void retrieveSequence(
        const Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & field);
      void retrieveGroup(const Messages::FieldCPtr & field);

      void retrieveFieldValue(const Messages::FieldCPtr & field);

    private:
      size_t recordLimit_;
      size_t recordDuplicate_;
      size_t recordCount_;
    };
  }
}
#endif /* MESSAGEPERFORMANCE_H */
