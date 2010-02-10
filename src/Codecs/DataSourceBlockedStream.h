// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATASOURCEBLOCKEDSTREAM_H
#define DATASOURCEBLOCKEDSTREAM_H
#include "DataSource.h"
#include <Common/QuickFAST_Export.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief A data source that reads input from an istream containing block headers.
    class QuickFAST_Export DataSourceBlockedStream : public DataSource
    {
    public:
      /// @brief Wrap a standard istream into a DataSource
      ///
      /// The input stream should be opened in binary mode
      /// if that option is available/needed on your operating
      /// system and stream type. (i.e. specify std::ios::binary
      /// when you open a ofstream on Windows.)
      /// @param stream supplies the data
      explicit DataSourceBlockedStream(std::istream & stream);

      /// @brief a typical virtual destructor.
      virtual ~DataSourceBlockedStream();

      virtual bool getBuffer(const uchar *& buffer, size_t & size);

    private:
      DataSourceBlockedStream();
      DataSourceBlockedStream(const DataSourceBlockedStream & );
      DataSourceBlockedStream & operator =(const DataSourceBlockedStream & );
    private:
      std::istream & stream_;
      boost::scoped_array<uchar> buffer_;
      size_t bufferCapacity_;
    };
  }
}
#endif // DATASOURCEBLOCKEDSTREAM_H
