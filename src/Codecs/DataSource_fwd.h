// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATASOURCE_FWD_H
#define DATASOURCE_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

namespace QuickFAST{
  namespace Codecs{
    class DataSource;
    /// @brief A smart pointer to a DataSource.
    typedef boost::shared_ptr<DataSource> DataSourcePtr;
  }
}
#endif // DATASOURCE_FWD_H
