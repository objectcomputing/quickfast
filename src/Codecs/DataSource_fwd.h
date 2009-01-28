// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATASOURCE_FWD_H
#define DATASOURCE_FWD_H
namespace QuickFAST{
  namespace Codecs{
    class DataSource;
    /// @brief A smart pointer to a DataSource.
    typedef boost::shared_ptr<DataSource> DataSourcePtr;
  }
}
#endif // DATASOURCE_FWD_H
