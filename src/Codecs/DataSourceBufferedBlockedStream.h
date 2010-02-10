// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATASOURCEBUFFEREDBLOCKEDSTREAM_H
#define DATASOURCEBUFFEREDBLOCKEDSTREAM_H
#include "DataSource.h"
#include <Common/QuickFAST_Export.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief A data source that reads input from an istream containing block headers.
    class QuickFAST_Export DataSourceBufferedBlockedStream : public DataSource
    {
    public:
      /// @brief Wrap a standard istream into a DataSource
      ///
      /// The input stream should be opened in binary mode
      /// if that option is available/needed on your operating
      /// system and stream type. (i.e. specify std::ios::binary
      /// when you open a ofstream on Windows.)
      /// @param stream supplies the data
      explicit DataSourceBufferedBlockedStream(std::istream & stream);

      /// @brief a typical virtual destructor.
      virtual ~DataSourceBufferedBlockedStream();

      ///////////////////////
      // Implement DataSource
      virtual bool getBuffer(const uchar *& buffer, size_t & size);

    private:
      DataSourceBufferedBlockedStream();
      DataSourceBufferedBlockedStream(const DataSourceBufferedBlockedStream & );
      DataSourceBufferedBlockedStream & operator =(const DataSourceBufferedBlockedStream & );
    private:
      boost::scoped_array<uchar> buffer_;
      size_t bufferCapacity_;
      size_t bufferPosition_;
      size_t blockRemaining_;
    };
  }
}
#endif // DATASOURCEBUFFEREDBLOCKEDSTREAM_H
