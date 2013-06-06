// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATASOURCEBUFFEREDSTREAM_H
#define DATASOURCEBUFFEREDSTREAM_H
#include <Common/QuickFAST_Export.h>
#include <Codecs/DataSource.h>
namespace QuickFAST{
  namespace Codecs{
    /// A data source that reads input from an istream into a big buffer then
    /// delivers the data from there.
    /// Intended for performance testing, not production use.
    class QuickFAST_Export DataSourceBufferedStream : public DataSource
    {
    public:
      /// @brief Buffer standard istream into a DataSource
      ///
      /// The input stream should be opened in binary mode
      /// if that option is available/needed on your operating
      /// system and stream type. (i.e. specify std::ios::binary
      /// when you open a ofstream on Windows.)
      /// @param stream supplies the data
      explicit DataSourceBufferedStream(std::istream & stream);

      /// @brief a typical virtual destructor.
      virtual ~DataSourceBufferedStream();

      ///////////////////////
      // Implement DataSource
      virtual bool getBuffer(const uchar *& buffer, size_t & size);

    private:
      bool first_;
      boost::scoped_array<unsigned char> buffer_;
      size_t pos_;
      size_t end_;
    };
  }
}
#endif // DATASOURCEBUFFEREDSTREAM_H
