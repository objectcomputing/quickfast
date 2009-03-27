// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include "DataSource.h"
namespace QuickFASTDotNet{

  namespace Codecs{


    class DataSourceBuffered : public QuickFASTDotNet::Codecs::DotNetDataSource
    {
    public:
      DataSourceBuffered(System::IO::Stream^ inStream);

      virtual bool readByte(QuickFAST::uchar & byte);

    protected:
      bool fillBuffer (void);

      boost::scoped_array<unsigned char> buffer_;
      gcroot<cli::array<unsigned char>^> managedBuffer_;
//      size_t pos_;
//      size_t end_;
      long pos_;
      long end_;

    };


  }
}
