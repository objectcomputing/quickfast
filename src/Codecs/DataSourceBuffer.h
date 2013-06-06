// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATASOURCEBUFFER_H
#define DATASOURCEBUFFER_H
#include <Common/QuickFAST_Export.h>
#include <Codecs/DataSource.h>
namespace QuickFAST{
  namespace Codecs{
    /// A data source that uses an existing buffer.
    class QuickFAST_Export DataSourceBuffer : public DataSource
    {
    public:
      /// @brief Deliver data from an existing buffer.
      ///
      /// Does not take ownership or otherwise manipulate the buffer.
      /// @param buffer contains the data
      /// @param length is the number of bytes in the data
      explicit DataSourceBuffer(const unsigned char * buffer, size_t length);

      /// @brief a typical virtual destructor.
      virtual ~DataSourceBuffer();

      virtual bool getBuffer(const uchar *& buffer, size_t & size);

    private:
      const unsigned char * buffer_;
      size_t end_;
      bool first_;
    };
  }
}
#endif // DATASOURCEBUFFER_H
