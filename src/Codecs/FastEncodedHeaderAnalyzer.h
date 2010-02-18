// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FASTENCODEDHEADERANALYZER_H
#define FASTENCODEDHEADERANALYZER_H
#include <Common/QuickFAST_Export.h>
#include <Codecs/HeaderAnalyzer.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief Handle a FAST encoded block header
    class QuickFAST_Export FastEncodedHeaderAnalyzer
      : public HeaderAnalyzer
    {
    public:

      /// @brief Construct
      ///
      /// Ignore: prefix FAST fields (looking for stop bits)
      /// Interpret: the next FAST field as block size if hasBlockSize is true
      /// Ignore: suffix FAST fields (looking for stop bits)
      ///
      /// @param prefix is the number of fields before the block size
      /// @param suffix is the number of fields after the block size
      /// @param hasBlockSize is true if the blocksize is present in the header
      explicit FastEncodedHeaderAnalyzer(
        size_t prefix = 0,
        size_t suffix = 0,
        bool hasBlockSize = false
        );

      /// @brief Typical virtual destructor
      virtual ~FastEncodedHeaderAnalyzer();

      ////////////////////////
      // Implement HeaderAnalyzer
      virtual bool analyzeHeader(DataSource & source, size_t & blockSize, bool & skip);
    private:
      size_t prefixCount_;
      size_t suffixCount_;
      enum
      {
        ParsingIdle,
        ParsingPrefix,
        ParsingBlockSize,
        ParsingSuffix,
        ParsingComplete
      } state_;
      bool hasBlockSize_;
      size_t blockSize_;
      size_t fieldCount_;
    };
  }
}
#endif // FASTENCODEDHEADERANALYZER_H
