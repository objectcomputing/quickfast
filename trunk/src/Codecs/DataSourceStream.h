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
      /// @brief Wrap a standard istream into a DataSource
      ///
      /// The input stream should be opened in binary mode
      /// if that option is available/needed on your operating
      /// system and stream type. (i.e. specify std::ios::binary
      /// when you open a ofstream on Windows.)
      /// @param stream supplies the data
      explicit DataSourceStream(std::istream & stream);

      /// @brief a typical virtual destructor.
      virtual ~DataSourceStream();

      virtual bool readByte(uchar & byte);

    private:
      std::istream & stream_;
    };
  }
}
#endif // DATASOURCESTREAM_H
