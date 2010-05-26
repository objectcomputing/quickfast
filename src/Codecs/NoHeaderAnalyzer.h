// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef NOHEADERANALYZER_H
#define NOHEADERANALYZER_H
#include <Codecs/HeaderAnalyzer.h>
namespace QuickFAST{
  namespace Codecs{
    /// A Header Analyzer for messsages that have no headers.
    class NoHeaderAnalyzer
      : public HeaderAnalyzer
    {
    public:
      NoHeaderAnalyzer()
        : testSkip_(0)
        , headersParsed_(0)
      {
      }
      /// @brief Typical virtual destructor
      virtual ~NoHeaderAnalyzer()
      {
      }
      virtual bool analyzeHeader(
        DataSource & source,
        size_t & blockSize,
        bool & skip);

      /// @brief For debuging.  Force this to skip every "testSkip"th record.
      /// @param testSkip is how often to skip a record.  Zero means never skip.
      void setTestSkip(size_t testSkip)
      {
        testSkip_ = testSkip;
      }

    private:
      size_t testSkip_;
      size_t headersParsed_;
    };

    inline
    bool NoHeaderAnalyzer::analyzeHeader(
      DataSource & /*source*/,
      size_t & blockSize,
      bool & skip)
    {
      blockSize = 0;
      skip = false;
      if(testSkip_ != 0 && (++headersParsed_ % testSkip_ == 0))
      {
        std::cout << "SKIPPING HEADER " << headersParsed_ << std::endl;
        skip = true;
      }

      return true;
    }

  }
}
#endif // NOHEADERANALYZER_H
