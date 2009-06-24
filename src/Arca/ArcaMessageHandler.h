#ifdef _MSC_VER
# pragma once
#endif
#ifndef ARCAMESSAGEHANDLER_H
#define ARCAMESSAGEHANDLER_H
#include <Arca/ArcaMessageStructure.h>

namespace QuickFASTArca{
  /// @brief A message consumer that attempts to produce a human readable version
  /// of a message that has been decoded by QuickFAST.
  class ArcaMessageHandler
  {
  public:
    /// @brief Construct given a ostream to which to write the interpreted results.
    ArcaMessageHandler() {};
    virtual ~ArcaMessageHandler() {};
    virtual bool handleMessage(
      size_t filterCount,
      const ArcaFullHeader * header,
      const ArcaMessageStructure * body) = 0;
    virtual bool reportCommunicationError(const std::string & message) = 0;
    virtual bool reportDecodingError(const std::string & message) = 0;
    virtual void receiverStarted() = 0;
    virtual void receiverStopped() = 0;
  };
}
#endif /* ARCAMESSAGEHANDLER_H */
