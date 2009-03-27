// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

namespace QuickFASTDotNet{

  namespace Codecs{

    class DotNetDataSource: public QuickFAST::Codecs::DataSource
    {
    public:
      DotNetDataSource(System::IO::Stream^ inStream);

      virtual bool readByte(QuickFAST::uchar & byte);

    protected:
      gcroot<System::IO::Stream^> inStream_;
    };
  }
}
