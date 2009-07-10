#include <Arca/QuickFASTArca_pch.h>
#include "ArcaMessageBuilder.h"
#include <Codecs/DataSourceBuffer.h>
#include <Common/Exceptions.h>
#include <Messages/Field.h>
using namespace QuickFAST;
using namespace QuickFASTArca;

ArcaMessageBuilder::ArcaMessageBuilder(
  QuickFAST::Codecs::TemplateRegistryPtr registry,
  ArcaMessageHandler & handler)
: decoder_(registry)
, messageHandler_(handler)
, filtered_(0)
, discarded_(0)
, verbose_(false)
{
  clear(0);
}

ArcaMessageBuilder::~ArcaMessageBuilder()
{
}

void
ArcaMessageBuilder::setVerbose(bool verbose)
{
  verbose_ = verbose;
  if(verbose_)
  {
    decoder_.setVerboseOutput(std::cout);
  }
}

void
ArcaMessageBuilder::setFilters(const SymbolIDFilter & filter)
{
  filters_ = filter;
}

void
ArcaMessageBuilder::clear(size_t )
{
  memset(&body_, 0, sizeof(body_));
}
void
ArcaMessageBuilder::reserve(size_t capacity)
{
  // meaningless we have a fixed sized buffer
}

size_t
ArcaMessageBuilder::size() const
{
  return 0; // meaningless
}

ArcaMsgType
ArcaMessageBuilder::getId(const QuickFAST::Messages::FieldIdentityCPtr & identity)
{
  // BIG ASSUMPTION HERE that the XML file id is numeric and < 1000;
  QuickFAST::field_id_t id = identity->id();
  size_t result = 0;
  if(id.size() >= 1)
  {
    result = id[0] - '0';
    if(id.size() >= 2)
    {
      result *= 10;
      result += id[1] - '0';
      if(id.size() >= 3)
      {
        result *= 10;
        result += id[2] - '0';
      }
    }
  }
  return static_cast<ArcaMsgType>(result);

}

void
ArcaMessageBuilder::addField(
  const Messages::FieldIdentityCPtr & identity,
  const Messages::FieldCPtr & value)
{
  ArcaMsgType id = getId(identity);

  /// Ascii fields assume they have been primed with nulls
  switch (id)
  {
  case ARCATAG_MsgType:
    {
      body_.MsgType = value->toUInt16();
      break;
    }
  case ARCATAG_SymbolIndex:
    {
      body_.SymbolIndex = value->toUInt16();
      break;
    }
  case ARCATAG_SourceSeqNum:
    {
      body_.SourceSeqNum = value->toUInt32();
      break;
    }
  case ARCATAG_SourceTime:
    {
      body_.SourceTime = value->toUInt32();
      break;
    }
  case ARCATAG_OrderID:
    {
      body_.OrderID = value->toUInt32();
      break;
    }
  case ARCATAG_TotalImbalance:
    {
      body_.TotalImbalance = value->toUInt32();
      break;
    }
  case ARCATAG_Volume:
    {
      body_.Volume = value->toUInt32();
      break;
    }
  case ARCATAG_PriceNumerator:
    {
      body_.PriceNumerator = value->toUInt32();
      break;
    }
  case ARCATAG_PriceScaleCode:
    {
      body_.PriceScaleCode = value->toUInt8();
      break;
    }
  case ARCATAG_Side:
    {
      body_.Side = value->toUInt8();
      break;
    }
  case ARCATAG_AuctionType:
    {
      body_.AuctionType = value->toUInt8();
      break;
    }
  case ARCATAG_ExchangeID:
    {
      body_.ExchangeID = value->toUInt8();
      break;
    }
  case ARCATAG_SecurityType:
    {
      body_.SecurityType = value->toUInt8();
      break;
    }
  case ARCATAG_FirmIndex:
    {
      body_.FirmIndex = value->toUInt16();
      break;
    }
  case ARCATAG_AuctionTime:
    {
      body_.AuctionTime = value->toUInt16();
      break;
    }
  case ARCATAG_SessionID:
    {
      body_.SessionID = value->toUInt8();
      break;
    }
  case ARCATAG_MarketImbalance:
    {
      body_.MarketImbalance = value->toUInt32();
      break;
    }
  case ARCATAG_Symbol:
    {
      strncpy(reinterpret_cast<char *>(&body_.Symbol),
        value->toAscii().c_str(),
        sizeof(body_.Symbol));
      break;
    }
  case ARCATAG_Firm:
    {
      strncpy(reinterpret_cast<char *>(&body_.Firm),
        value->toAscii().c_str(),
        sizeof(body_.Firm));
      break;
    }
/* these fields are not sent compressed */
  case ARCATAG_BeginSeqNum:
  case ARCATAG_EndSeqNum:
  case ARCATAG_RetransmitMethod:
  case ARCATAG_Status:
  case ARCATAG_RejectReason:
  case ARCATAG_SourceID:
  default:
    {
      throw QuickFAST::TemplateDefinitionError("Unknown field id in template " + identity->id());
    }
  }
}

bool
ArcaMessageBuilder::getIdentity(
  const std::string &name,
  Messages::FieldIdentityCPtr & identity) const
{
  // We could define dummy identities to return.
  throw UsageError("U50", "getIdentity unsupported for Arca");
}

void
ArcaMessageBuilder::setApplicationType(
  const std::string & type,
  const std::string & ns)
{
  applicationType_ = type;
  applicationTypeNamespace_ = ns;
}

const std::string &
ArcaMessageBuilder::getApplicationType()const
{
  return applicationType_;
}

const std::string &
ArcaMessageBuilder::getApplicationTypeNs()const
{
  return applicationTypeNamespace_;
}
void
ArcaMessageBuilder::startSequence(
  Messages::FieldIdentityCPtr identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  throw QuickFAST::TemplateDefinitionError("Structures not supported for Arca");
}

Messages::MessageBuilder &
ArcaMessageBuilder::startSequenceEntry(
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  throw QuickFAST::TemplateDefinitionError("Structures not supported for Arca");
}

void
ArcaMessageBuilder::endSequenceEntry(QuickFAST::Messages::MessageBuilder & entry)
{
  throw QuickFAST::TemplateDefinitionError("Structures not supported for Arca");
}

void
ArcaMessageBuilder::endSequence( Messages::FieldIdentityCPtr identity)
{
  throw QuickFAST::TemplateDefinitionError("Structures not supported for Arca");
}

Messages::MessageBuilder &
ArcaMessageBuilder::startGroup(
  Messages::FieldIdentityCPtr identity,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace,
  size_t size)
{
  throw QuickFAST::TemplateDefinitionError("Groupsnot supported for Arca");
}

void
ArcaMessageBuilder::endGroup(
  Messages::FieldIdentityCPtr identity,
  QuickFAST::Messages::MessageBuilder & entry)
{
  throw QuickFAST::TemplateDefinitionError("Groupsnot supported for Arca");
}

bool
ArcaMessageBuilder::consumeBuffer(const unsigned char * buffer, size_t bufferSize)
{
  bool ok = true;
  if(verbose_)
  {
    std::cout << "Incoming Packet:";
    for(size_t nByte = 0; /*nByte < 10 && */ nByte < bufferSize; ++nByte)
    {
      if(nByte % 16 == 0) std::cout << std::endl;
      unsigned short shortByte = static_cast<unsigned short>(buffer[nByte]) & 0xFF;
      std::cout << ' ' << std::hex << std::setw(2)<< std::setfill('0') << shortByte << std::setfill(' ') << std::dec;
    }
    std::cout << std::endl;
  }
  decoder_.reset();
  size_t pos = 0;
  const ArcaFullHeader * header = 0;
  try
  {
    while(pos + sizeof(ArcaCommonHeader) < bufferSize)
    {
      header = reinterpret_cast<const ArcaFullHeader *>(buffer + pos);
      ArcaMsgType messageType = static_cast<ArcaMsgType>(header->MsgType);
      switch(messageType)
      {
        case ArcaMsgType_SEQUENCE_NUMBER_RESET:
        case ArcaMsgType_SYMBOL_INDEX_MAPPING:
        case ArcaMsgType_SYMBOL_CLEAR:
        case ArcaMsgType_FIRM_INDEX_MAPPING:
          {
            pos += sizeof(ArcaCommonHeader);
            if(pos < bufferSize)
            {
              QuickFAST::Codecs::DataSourceBuffer source(buffer + pos, bufferSize - pos);
              if(verbose_)
              {
                source.setEcho(std::cout, QuickFAST::Codecs::DataSource::HEX, false, true);
              }
              size_t entryCount = header->NumBodyEntries;
              for(size_t nEntry = 0; nEntry < entryCount; ++nEntry)
              {
                clear(1);
                if(decoder_.decodeMessage(source, *this))
                {
                  messageHandler_.handleMessage(
                    filtered_,
                    header,
                    &body_);
                  filtered_ = 0;
                }
              }
              pos = bufferSize - source.bytesAvailable();
            }
            break;
          }

        case ArcaMsgType_IMBALANCE_REFRESH:
        case ArcaMsgType_ADD:
        case ArcaMsgType_MODIFY:
        case ArcaMsgType_DELETE:
        case ArcaMsgType_IMBALANCE:
        case ArcaMsgType_GENERIC:
          {
            pos += sizeof(ArcaCommonHeader);
            if(pos < bufferSize)
            {
              QuickFAST::Codecs::DataSourceBuffer source(buffer + pos, bufferSize - pos);
              decoder_.reset();
              if(verbose_)
              {
                source.setEcho(std::cout, QuickFAST::Codecs::DataSource::HEX, false, true);
              }
              unsigned short entryCount = header->NumBodyEntries;
              for(unsigned short nEntry = 0; nEntry < entryCount; ++nEntry)
              {
                clear(1);
                body_.EntryNum = nEntry;
                if(decoder_.decodeMessage(source, *this))
                {
                  if(filter())
                  {
                    messageHandler_.handleMessage(
                      filtered_,
                      header,
                      &body_);
                    filtered_ = 0;
                  }
                  else
                  {
                    ++filtered_;
                  }
                }
              }
              pos = bufferSize - source.bytesAvailable();
            }
            break;
          }

        case ArcaMsgType_BOOK_REFRESH:
          {
            pos += sizeof(ArcaFullHeader);
            if(pos < bufferSize)
            {
              QuickFAST::Codecs::DataSourceBuffer source(buffer + pos, bufferSize - pos);
              decoder_.reset();
              if(verbose_)
              {
                source.setEcho(std::cout, QuickFAST::Codecs::DataSource::HEX, false, true);
              }
              size_t entryCount = header->NumBodyEntries;
              for(size_t nEntry = 0; nEntry < entryCount; ++nEntry)
              {
                clear(1);
                body_.EntryNum = QuickFAST::uint16(nEntry);
                if(decoder_.decodeMessage(source, *this))
                {
                  messageHandler_.handleMessage(
                    filtered_,
                    header,
                    &body_);
                  filtered_ = 0;
                }
              }
              pos = bufferSize - source.bytesAvailable();
            }
            break;
          }

        case ArcaMsgType_HEARTBEAT:
          {
            pos += sizeof(ArcaCommonHeader);
            // no body in a heartbeat message
            clear(1);
            messageHandler_.handleMessage(
              filtered_,
              header,
              &body_);
            filtered_ = 0;
            break;
          }

        case ArcaMsgType_HEARTBEAT_RESPONSE:
          {
            pos += sizeof(ArcaCommonHeader);
            if(pos + sizeof(ArcaHeartbeatResponseBody) < bufferSize)
            {
              clear(1);
              const ArcaHeartbeatResponseBody * body =
                reinterpret_cast<const ArcaHeartbeatResponseBody *>(buffer + pos);
              memcpy(&body_.SourceID, &body->SourceID, sizeof(body_.SourceID));
              messageHandler_.handleMessage(
                filtered_,
                header,
                &body_);
              filtered_ = 0;
              pos += sizeof(ArcaHeartbeatResponseBody);
            }
            break;
          }

        case ArcaMsgType_MESSAGE_UNAVAILABLE:
          {
            pos += sizeof(ArcaCommonHeader);
            if(pos + sizeof(ArcaMessageUnavailableBody) < bufferSize)
            {
              clear(1);
              const ArcaMessageUnavailableBody * body =
                reinterpret_cast<const ArcaMessageUnavailableBody *>(buffer + sizeof(ArcaCommonHeader));
              body_.BeginSeqNum = body->BeginSeqNum;
              body_.EndSeqNum = body->EndSeqNum;
              messageHandler_.handleMessage(
                filtered_,
                header,
                &body_);
              filtered_ = 0;
              pos += sizeof(ArcaMessageUnavailableBody);
            }
            break;
          }
        case ArcaMsgType_RETRANSMISSION_RESPONSE:
          {
            pos += sizeof(ArcaCommonHeader);
            if(pos + sizeof(ArcaRetransmissionResponseBody) < bufferSize)
            {
              clear(1);
              const ArcaRetransmissionResponseBody * body =
                reinterpret_cast<const ArcaRetransmissionResponseBody *>(buffer + sizeof(ArcaCommonHeader));
              body_.RejectReason = body->RejectReason;
              memcpy(&body_.SourceID, &body->SourceID, sizeof(body_.SourceID));
              body_.SourceSeqNum = body->SourceSeqNum;
              body_.Status = body->Status;

              messageHandler_.handleMessage(
                filtered_,
                header,
                &body_);
              filtered_ = 0;
              pos += sizeof(ArcaRetransmissionResponseBody);
            }
            break;
          }

        case ArcaMsgType_RETRANSMISSION_REQUEST:
          {
            pos += sizeof(ArcaCommonHeader);
            if(pos + sizeof(ArcaRetransmissionRequestBody) < bufferSize)
            {
              clear(1);
              const ArcaRetransmissionRequestBody * body =
                reinterpret_cast<const ArcaRetransmissionRequestBody *>(buffer + sizeof(ArcaCommonHeader));
              body_.BeginSeqNum = body->BeginSeqNum;
              body_.BeginSeqNum = body->BeginSeqNum;
              memcpy(&body_.SourceID, &body->SourceID, sizeof(body_.SourceID));
              messageHandler_.handleMessage(
                filtered_,
                header,
                &body_);
              filtered_ = 0;
              pos += sizeof(ArcaRetransmissionRequestBody);
            }
            break;
          }

        case ArcaMsgType_BOOK_REFRESH_REQUEST:
          {
            pos += sizeof(ArcaCommonHeader);
            if(pos + sizeof(ArcaBookRefreshRequestBody) < bufferSize)
            {
              clear(1);
              const ArcaBookRefreshRequestBody * body =
                reinterpret_cast<const ArcaBookRefreshRequestBody *>(buffer + sizeof(ArcaCommonHeader));
              body_.SessionID = body->SessionID;
              memcpy(&body_.SourceID, &body->SourceID, sizeof(body_.SourceID));
              body_.SymbolIndex = body->SymbolIndex;
              messageHandler_.handleMessage(
                filtered_,
                header,
                &body_);
              filtered_ = 0;
              pos += sizeof(ArcaBookRefreshRequestBody);
            }
            break;
          }

        case ArcaMsgType_IMBALANCE_REFRESH_REQUEST:
          {
            pos += sizeof(ArcaCommonHeader);
            if(pos + sizeof(ArcaImbalanceRefreshRequestBody) < bufferSize)
            {
              clear(1);
              const ArcaImbalanceRefreshRequestBody * body =
                reinterpret_cast<const ArcaImbalanceRefreshRequestBody *>(buffer + sizeof(ArcaCommonHeader));
              body_.SessionID = body->SessionID;
              memcpy(&body_.SourceID, &body->SourceID, sizeof(body_.SourceID));
              body_.SymbolIndex = body->SymbolIndex;
              messageHandler_.handleMessage(
                filtered_,
                header,
                &body_);
              filtered_ = 0;
              pos += sizeof(ArcaImbalanceRefreshRequestBody);
            }
            break;
          }

        case ArcaMsgType_SYMBOL_INDEX_MAPPING_REQUEST:
          {
            pos += sizeof(ArcaCommonHeader);
            if(pos + sizeof(ArcaSymbolIndexMappingRequestBody) < bufferSize)
            {
              clear(1);
              const ArcaSymbolIndexMappingRequestBody * body =
                reinterpret_cast<const ArcaSymbolIndexMappingRequestBody *>(buffer + pos);
              body_.RetransmitMethod = body->RetransmitMethod;
              body_.SessionID = body->SessionID;
              memcpy(&body_.SourceID, &body->SourceID, sizeof(body_.SourceID));
              body_.SymbolIndex = body->SymbolIndex;
              messageHandler_.handleMessage(
                filtered_,
                header,
                &body_);
              filtered_ = 0;
              pos += sizeof(ArcaSymbolIndexMappingRequestBody);
            }
            break;
          }

        case ArcaMsgType_FIRM_INDEX_MAPPING_REQUEST:
          {
            pos += sizeof(ArcaCommonHeader);
            if(pos + sizeof(ArcaAttributedQuoteFirmMappingRequestBody) < bufferSize)
            {
              clear(1);
              const ArcaAttributedQuoteFirmMappingRequestBody * body =
                reinterpret_cast<const ArcaAttributedQuoteFirmMappingRequestBody *>(buffer + pos);
              body_.FirmIndex = body->FirmIndex;
              body_.RetransmitMethod = body->RetransmitMethod;
              memcpy(&body_.SourceID, &body->SourceID, sizeof(body_.SourceID));
              messageHandler_.handleMessage(
                filtered_,
                header,
                &body_);
              filtered_ = 0;
              pos += sizeof(ArcaAttributedQuoteFirmMappingRequestBody);
            }
            break;
          }
        default:
          {
            throw EncodingError("Unknown memssage type in incoming ARCA message.");
          }
        }
      }
  }
  catch (const std::exception & ex)
  {
    messageHandler_.reportDecodingError(ex.what(), header, & body_);
    ++discarded_;
  }
  return ok;
}

void
ArcaMessageBuilder::decode(const unsigned char * buffer, size_t bufferSize)
{
}

bool
ArcaMessageBuilder::filter()
{
  size_t filterSize = filters_.size();
  if(filterSize == 0)
  {
    return true;
  }
  unsigned short symbolIndex = body_.SymbolIndex;
  for(size_t nFilter = 0; nFilter < filterSize; ++nFilter)
  {
    if(filters_[nFilter] == symbolIndex)
    {
      return true;
    }
  }
  return false;
}


bool
ArcaMessageBuilder::reportCommunicationError(const std::string & errorBuffer)
{
  return messageHandler_.reportCommunicationError(errorBuffer);
}

void
ArcaMessageBuilder::receiverStarted()
{
  messageHandler_.receiverStarted();
}

void
ArcaMessageBuilder::receiverStopped()
{
  messageHandler_.receiverStopped();
}
