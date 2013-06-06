// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATASOURCESTRING_H
#define DATASOURCESTRING_H
#include <Common/QuickFAST_Export.h>
#include <Codecs/DataSource.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief A DataSource that reads input from a text string.
    class QuickFAST_Export DataSourceString : public DataSource
    {
    public:
      /// @brief Construct given a c-style string
      /// @param cstring contains the data to be delivered by the source
      explicit DataSourceString(const char * cstring);
      /// @brief Construct given a c++-style string
      /// @param string contains the data to be delivered by the source
      explicit DataSourceString(const std::string & string);
      /// @brief a typical virtual destructor.
      virtual ~DataSourceString();

      virtual bool getBuffer(const uchar *& buffer, size_t & size);
//      virtual bool readByte(uchar & byte);

    private:
      std::string string_;
      bool first_;
    };
  }
}
#endif // DATASOURCESTRING_H
