#ifdef _MSC_VER
# pragma once
#endif
#ifndef ARCAMESSAGEBUILDER_H
#define ARCAMESSAGEBUILDER_H
#include "ArcaMessageBuilder_fwd.h"
#include <Arca/QuickFASTArca_Export.h>
#include <Codecs/BufferConsumer.h>
#include <Codecs/Decoder.h>
#include <Messages/MessageBuilder.h>
#include <Common/Types.h>
#include <Arca/ArcaMessageStructure.h>
#include <Arca/ArcaMessageHandler.h>

namespace QuickFASTArca{
  /// @brief Receives a raw Arca data packet; decodes it if necessary, and sends it to an ArcaMessageConsumer
  class QuickFASTArca_Export ArcaMessageBuilder
    : public QuickFAST::Codecs::BufferConsumer    // receive buffer from input stream
    , public QuickFAST::Messages::MessageBuilder  // and build message from decoded fields
  {
  public:
    typedef std::vector<unsigned short> SymbolIDFilter;
    /// @brief Construct
    ArcaMessageBuilder(
      QuickFAST::Codecs::TemplateRegistryPtr registry,
      ArcaMessageHandler & handler);
    virtual ~ArcaMessageBuilder();

    void setVerbose(bool verbose);
    void setFilters(const SymbolIDFilter & filter);

    ///////////////////////////
    // Implement BufferConsumer
    virtual bool consumeBuffer(const unsigned char * buffer, size_t bufferSize);
    virtual bool reportCommunicationError(const std::string & errorBuffer);
    virtual void receiverStarted();
    virtual void receiverStopped();

    ////////////////////////////
    // Implement MessageBuilder
    virtual void clear(size_t capacity);
    virtual void reserve(size_t capacity);
    virtual size_t size()const;
    virtual void addField(
      const QuickFAST::Messages::FieldIdentityCPtr & identity,
      const QuickFAST::Messages::FieldCPtr & value);
    virtual bool getIdentity(
      const std::string &name,
      QuickFAST::Messages::FieldIdentityCPtr & identity) const;
    virtual void setApplicationType(const std::string & type, const std::string & ns);
    virtual const std::string & getApplicationType()const;
    virtual const std::string & getApplicationTypeNs()const;
    virtual QuickFAST::Messages::MessageBuilderPtr startSequenceEntry(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size)const;
    virtual QuickFAST::Messages::MessageBuilderPtr startGroup(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size)const;
    virtual const QuickFAST::Messages::FieldSet & getFieldSet() const;

  private:
    static ArcaMsgType getId(const QuickFAST::Messages::FieldIdentityCPtr & identity);
    void decode(const unsigned char * buffer, size_t bufferSize);
    bool filter();
  private:
    QuickFAST::Codecs::Decoder decoder_;
    ArcaMessageHandler & messageHandler_;
    ArcaMessageStructure body_;
    size_t filtered_;
    std::string applicationType_;
    std::string applicationTypeNamespace_;
    SymbolIDFilter filters_;
    size_t discarded_;
    bool verbose_;
  };
}
#endif /* ARCAMESSAGEBUILDER_H */
