// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include "DataSource.h"

using namespace System;
using namespace System::IO;


namespace QuickFASTDotNet{
  namespace Codecs{


    DotNetDataSource::DotNetDataSource(System::IO::Stream^ inStream)
      :inStream_(inStream)
    {
    }


    bool DotNetDataSource::readByte(QuickFAST::uchar & byte)
    {
      int read = inStream_->ReadByte();
      byte = static_cast<QuickFAST::uchar>(read);

      return read != -1;
    }

  }
}
