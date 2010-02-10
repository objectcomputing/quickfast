// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef NOBLOCKHEADER_H
#define NOBLOCKHEADER_H
#include <Codecs/HeaderAnalyzer.h>
namespace QuickFAST{
  namespace Codecs{
    /// A Header Analyzer for messsages that have no headers.
    class NoHeaderAnalyzer
      : public HeaderAnalyzer
    {
    public:
      /// @brief Typical virtual destructor
      virtual ~NoHeaderAnalyzer()
      {
      }
      virtual bool analyzeHeader(
        DataSource & source,
        size_t & blockSize,
        bool & skip);
    };

    inline
    bool NoHeaderAnalyzer::analyzeHeader(
      DataSource & /*source*/,
      size_t & blockSize,
      bool & skip)
    {
      blockSize = 0;
      return true;
    }
  }
}
#endif // NOBLOCKHEADER_H
