// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATASOURCESTREAM_H
#define DATASOURCESTREAM_H
#include "DataSource.h"
#include <Common/QuickFAST_Export.h>
namespace QuickFAST{
  namespace Codecs{
    /// A data source that reads input from an istream.
    class QuickFAST_Export DataSourceStream : public DataSource
    {
    public:
      /// Default size for the buffer
      const static size_t defaultBufferSize = 1024;

      /// @brief Wrap a standard istream into a DataSource
      ///
      /// The input stream should be opened in binary mode
      /// if that option is available/needed on your operating
      /// system and stream type. (i.e. specify std::ios::binary
      /// when you open a ofstream on Windows.)
      /// @param stream supplies the data
      /// @param bufferSize specifies how large a buffer to allocate to read the data
      explicit DataSourceStream(std::istream & stream, size_t bufferSize = defaultBufferSize);

      /// @brief a typical virtual destructor.
      virtual ~DataSourceStream();

      ///////////////////////
      // Implement DataSource
      virtual bool getBuffer(const uchar *& buffer, size_t & size);

    private:
      DataSourceStream();
      DataSourceStream(const DataSourceStream & );
      DataSourceStream & operator =(const DataSourceStream & );
    private:
      std::istream & stream_;
      size_t pos_; // position within file
      size_t end_; // end of file

      boost::scoped_array<uchar> buffer_;
      size_t capacity_; // size of buffer
    };
  }
}
#endif // DATASOURCESTREAM_H
