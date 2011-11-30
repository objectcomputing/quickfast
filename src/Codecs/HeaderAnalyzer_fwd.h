// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef HEADERANALYZER_FWD_H
#define HEADERANALYZER_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

namespace QuickFAST{
  namespace Codecs{
    /// An interface to be used to adapt to various styles of block header
    class HeaderAnalyzer;
  }
}
#endif // HEADERANALYZER_H
