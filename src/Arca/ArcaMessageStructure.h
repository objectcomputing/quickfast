#ifdef _MSC_VER
# pragma once
#endif
#ifndef ARCAMESSAGESTRUCTURE_H
#define ARCAMESSAGESTRUCTURE_H
#include <Common/Types.h>

namespace QuickFASTArca{
  ///@brief Arca Message type
  ///
  /// There are two fields
  enum ArcaMsgType {
    // Values < 100 go in header
    ArcaMsgType_SEQUENCE_NUMBER_RESET = 1,
    ArcaMsgType_HEARTBEAT = 2,
    ArcaMsgType_MESSAGE_UNAVAILABLE = 5,
    ArcaMsgType_RETRANSMISSION_RESPONSE = 10,
    ArcaMsgType_RETRANSMISSION_REQUEST = 20,
    ArcaMsgType_HEARTBEAT_RESPONSE = 24,
    ArcaMsgType_BOOK_REFRESH_REQUEST = 30,
    ArcaMsgType_IMBALANCE_REFRESH_REQUEST = 31,
    ArcaMsgType_BOOK_REFRESH = 32,
    ArcaMsgType_IMBALANCE_REFRESH = 33,
    ArcaMsgType_SYMBOL_INDEX_MAPPING_REQUEST = 34,
    ArcaMsgType_SYMBOL_INDEX_MAPPING = 35,
    ArcaMsgType_SYMBOL_CLEAR = 36,
    ArcaMsgType_FIRM_INDEX_MAPPING = 37,
    ArcaMsgType_FIRM_INDEX_MAPPING_REQUEST = 38,
    ArcaMsgType_GENERIC = 99, // body MsgType is valid

    // MsgTypes >= 100 go in body, not header
    ArcaMsgType_ADD = 100,
    ArcaMsgType_MODIFY = 101,
    ArcaMsgType_DELETE = 102,
    ArcaMsgType_IMBALANCE = 103
  };

#pragma pack(push,1)
  struct ArcaU16
  {
    operator unsigned short()const{return (b[0] << 8) + b[1];}
    operator ArcaMsgType()const{return static_cast<ArcaMsgType>((b[0] << 8) + b[1]);}
  private:
    unsigned char b[2];
  };

  struct ArcaU32
  {
    operator unsigned int()const{return (b[0] << 24) + (b[1] << 16) + (b[2] << 8) + b[3];}
  private:
    unsigned char b[4];
  };



  typedef unsigned char ArcaU8;
  /// @brief common header for all Arca messages
  ///
  /// Followed by 0 or more body entries (defined below)
  /// This is declared with pack 1 to allow direct overlay of
  /// the data within the buffer.
  ///
  /// Beware endian issues.
  ///
  struct ArcaCommonHeader
  {
    ArcaU16 MsgSize;
    ArcaU16 MsgType; // values from ArcaMsgType < 100
    ArcaU32 MsgSeqNum;
    ArcaU32 SendTime;
    ArcaU8 ProductID;
    ArcaU8 RetransFlag;
    ArcaU8 NumBodyEntries;
    ArcaU8 FILLER[1];
  };

  /// @brief special header used only be BookRefresh messages
  struct ArcaFullHeader : public ArcaCommonHeader
  {
    ArcaU8 FILLER2[1];
    ArcaU8 SessionID;
    ArcaU16 SymbolIndex;
    ArcaU16 CurrentRefreshMsgSeq;
    ArcaU16 TotalRefreshMsgSeq;
    ArcaU32 LastSourceSeqNum;
    ArcaU32 LastMsgSeqNum;
    char Symbol[16];
  };

  struct ArcaHeartbeatResponseBody
  {
    char SourceID[20];
  };

  struct ArcaRetransmissionRequestBody
  {
    ArcaU32 BeginSeqNum;
    ArcaU32 EndSeqNum;
    ArcaU8 SourceID[20];
  };

  struct ArcaBookRefreshRequestBody
  {
    ArcaU16 SymbolIndex;
    ArcaU8 SessionID;
    ArcaU8 FILLER1[1];
    ArcaU8 SourceID[20];
  };

  struct ArcaImbalanceRefreshRequestBody
  {
    ArcaU16 SymbolIndex;
    ArcaU8 SessionID;
    ArcaU8 FILLER1[1];
    char SourceID[20];
  };

  struct ArcaSymbolIndexMappingRequestBody
  {
    ArcaU16 SymbolIndex;
    ArcaU8 SessionID;
    ArcaU8 RetransmitMethod;
    char SourceID[20];
  };

  struct ArcaAttributedQuoteFirmMappingRequestBody
  {
    ArcaU16 FirmIndex;
    ArcaU8 FILLER1[1];
    ArcaU8 RetransmitMethod;
    char SourceID[20];
  };

  struct ArcaRetransmissionResponseBody
  {
    ArcaU32 SourceSeqNum;
    char SourceID[20];
    ArcaU8 Status;
    ArcaU8 RejectReason;
    ArcaU8 FILLER2[2];
  };

  struct ArcaMessageUnavailableBody
  {
    ArcaU32 BeginSeqNum;
    ArcaU32 EndSeqNum;
  };


#pragma pack(pop)

  ///@brief merge all Arca body fields into a single structure
  struct ArcaMessageStructure
  {
    QuickFAST::uint16 EntryNum;            // Synthesized: zero based entry index.
    QuickFAST::uint16 MsgType;             //  00 AB_MSG_TYPE
    QuickFAST::uint16 SymbolIndex;         //  10 AB_STOCK_IDX
    QuickFAST::uint32 SourceSeqNum;        //  20 AB_SEQUENCE
    QuickFAST::uint32 SourceTime;          //  30 AB_TIME
    QuickFAST::uint32 OrderID;             //  40 AB_ORDER_ID
    QuickFAST::uint32 TotalImbalance;      //  41 AB_IMBALANCE
    QuickFAST::int32 Volume;               //  50 AB_VOLUME
    QuickFAST::int32 PriceNumerator;       //  60 AB_PRICE
    QuickFAST::uchar PriceScaleCode;       //  70 AB_PRICE_SCALE
    QuickFAST::uchar Side;                 //  80 AB_BUY_SELL
    QuickFAST::uchar AuctionType;          //  81 AB_AUCTION_TYPE
    QuickFAST::uchar ExchangeID;           //  90 AB_EXCH_ID
    QuickFAST::uchar SecurityType;         // 100 AB_SECURITY_TYPE
    QuickFAST::uint16 FirmIndex;           // 110 AB_FIRM_ID
    QuickFAST::uint16 AuctionTime;         // 111 AB_AUCTION_TIME
    QuickFAST::uchar SessionID;            // 120 AB_SESSION_ID
    QuickFAST::int32 MarketImbalance;      // 130 AB_MKT_IMBALANCE
    QuickFAST::uchar Symbol[16];           // 131  AB_SYMBOL_STRING
    QuickFAST::uchar Firm[5];              // 132  AB_FIRM_STRING

    QuickFAST::uint32 BeginSeqNum;         // 990 / not a compressed field
    QuickFAST::uint32 EndSeqNum;           // 991 / not a compressed field
    QuickFAST::uchar RetransmitMethod;     // 992 / not a compressed field
    QuickFAST::uchar Status;               // 993 / not a compressed field
    QuickFAST::uchar RejectReason;         // 994 / not a compressed field
    QuickFAST::uchar SourceID[20];          // 995 / not a compressed field
  };

  /// @brief Assign a tag number to each field in the ArcaMessageStructure.
  ///
  /// Tag numbers must match the tags used in the Arca
  /// XML template, so they are explicly assigned here.
  /// The comments give the name of the field from Appendix C, table 1 of the
  /// ArcaBook Multicast Specification v2.4
  /// Tag numbers = 10 * field number from that table + offset to avoid duplicates.
  /// Some fields are defined in messages that do not get compressed.  These
  /// fields are given 900 tag numbers.
  enum ArcaFieldTag
  {
    ARCATAG_MsgType = 0,              //  00 AB_MSG_TYPE
    ARCATAG_SymbolIndex = 10,         //  10 AB_STOCK_IDX
    ARCATAG_SourceSeqNum = 20,        //  20 AB_SEQUENCE
    ARCATAG_SourceTime = 30,          //  30 AB_TIME
    ARCATAG_OrderID = 40,             //  40 AB_ORDER_ID
    ARCATAG_TotalImbalance = 41,      //  41 AB_IMBALANCE
    ARCATAG_Volume = 50,              //  50 AB_VOLUME
    ARCATAG_PriceNumerator = 60,      //  60 AB_PRICE
    ARCATAG_PriceScaleCode = 70,      //  70 AB_PRICE_SCALE
    ARCATAG_Side = 80,                //  80 AB_BUY_SELL
    ARCATAG_AuctionType = 81,         //  81 AB_AUCTION_TYPE
    ARCATAG_ExchangeID = 90,          //  90 AB_EXCH_ID
    ARCATAG_SecurityType = 100,       // 100 AB_SECURITY_TYPE
    ARCATAG_FirmIndex = 110,          // 110 AB_FIRM_ID
    ARCATAG_AuctionTime = 111,        // 111 AB_AUCTION_TIME
    ARCATAG_SessionID = 120,          // 120 AB_SESSION_ID
    ARCATAG_MarketImbalance = 130,    // 130 AB_MKT_IMBALANCE
    ARCATAG_Symbol = 131,             // 131  AB_SYMBOL
    ARCATAG_Firm = 132,               // 132  AB_FIRM_STRING

    ARCATAG_BeginSeqNum = 990,        // 990 / not a compressed field
    ARCATAG_EndSeqNum = 991,          // 991 / not a compressed field
    ARCATAG_RetransmitMethod = 992,   // 992 / not a compressed field
    ARCATAG_Status = 993,             // 993 / not a compressed field
    ARCATAG_RejectReason = 994,       // 994 / not a compressed field
    ARCATAG_SourceID = 995            // 995 / not a compressed field
  };

}
#endif /* ARCAMESSAGESTRUCTURE_H */
