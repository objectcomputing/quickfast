// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIXEDSIZEHEADERANALYZER_H
#define FIXEDSIZEHEADERANALYZER_H
#include <Common/QuickFAST_Export.h>
#include "HeaderAnalyzer.h"
#include <Common/ByteSwapper.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief Handle a fixed Size block header
    class QuickFAST_Export FixedSizeHeaderAnalyzer
      : public HeaderAnalyzer
    {
    public:
      /// @brief Construct
      ///
      /// Ignore: prefixBytes
      /// Interpret: sizeBytes as a "bigEndian" number
      /// Ignore:suffixBytes
      ///
      /// Note sequence # support was added after-the-fact.  It clashes a bit with
      /// block size support, but they _can_ be used together if you are careful.
      ///
      /// @param sizeBytes is the number of bytes in the block size field.  Zero means no block size.
      /// @param bigEndian if true means the data on the wire is big endian (network order)
      /// @param prefixBytes is the number if bytes in the header before the block size.
      /// @param suffixBytes is the number if bytes in the header after the block size.
      /// @param sequenceOffset is the position of the sequenceNumber field in the header
      /// @param sequenceLength is the number of bytes in the sequenceNumber field
      explicit FixedSizeHeaderAnalyzer(
        size_t sizeBytes,
        bool bigEndian = false,
        size_t prefixBytes = 0,
        size_t suffixBytes = 0,
        size_t sequenceOffset = 0,
        size_t sequenceLength = 4
        );

      /// @brief Typical virtual destructor
      virtual ~FixedSizeHeaderAnalyzer();

      /// @brief For debuging.  Force this to skip every "testSkip"th record.
      /// @param testSkip is how often to skip a record.  Zero means never skip.
      void setTestSkip(size_t testSkip)
      {
        testSkip_ = testSkip;
      }

      ////////////////////////
      // Implement HeaderAnalyzer
      virtual bool analyzeHeader(DataSource & source, size_t & blockSize, bool & skip);
      virtual bool supportsSequenceNumber()const;
      virtual uint32 getSequenceNumber(const uchar * buffer) const;
      virtual void reset();
    private:
      size_t prefixBytes_;
      size_t sizeBytes_;
      size_t suffixBytes_;
      size_t sequenceOffset_;
      size_t sequenceLength_;
      bool bigEndian_;
      bool swapNeeded_;

      enum
      {
        ParsingIdle,
        ParsingPrefix,
        ParsingBlockSize,
        ParsingSuffix,
        ParsingComplete
      } state_;
      size_t blockSize_;
      size_t byteCount_;

      size_t testSkip_;
      size_t headersParsed_;
    };
  }
}
#endif // FIXEDSIZEHEADERANALYZER_H
