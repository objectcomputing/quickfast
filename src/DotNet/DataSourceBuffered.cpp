// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include "DataSourceBuffered.h"

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;


namespace QuickFASTDotNet{
  namespace Codecs{


    DataSourceBuffered::DataSourceBuffered(System::IO::Stream^ inStream)
      : DotNetDataSource(inStream)
      , pos_(0)
      , end_(0)
    {
      fillBuffer();
    }

#pragma unmanaged
    bool DataSourceBuffered::readByte(QuickFAST::uchar & byte)
    {
      if(pos_ < end_)
      {
        byte = buffer_[pos_++];
        return true;
      }
      // do we want to recheck the stream for data
      //else if (fillBuffer())
      //{
      //  return readByte (byte)
      //}
      return false;
    }
#pragma managed


    bool DataSourceBuffered::fillBuffer (void)
    {
      bool filledBuffer = false;
      long length = long(inStream_->Length - inStream_->Position);
      if (length > 0)
      {
        pos_ = 0;
        end_ = length;

        buffer_.reset(new unsigned char[length]);
        unsigned char* buf = buffer_.get();
        int read = 0;
        long cnt = 0;
        for(cnt = 0; (cnt < length) && (read != -1); cnt++)
        {
          read = inStream_->ReadByte();
          buf[cnt] = (unsigned char) read;
        }
        if (read == -1)
          end_ = cnt;

        //buffer_.reset(new unsigned char[length]);
        //if (managedBuffer_ == nullptr || managedBuffer_->Length < length)
        //{
        //  managedBuffer_ = gcnew cli::array<unsigned char>(length);
        //}
        //int result = inStream_->Read(managedBuffer_, 0, length);

        //if (result > 0) // read number of bytes
        //{
        //  IntPtr unmgd(buffer_.get());
        //  Marshal::Copy(managedBuffer_, 0, unmgd, result);
        //  if (result < length)
        //  {
        //    // should we warn that we for some reason were unable to read all of the buffer?
        //    end_ = result;
        //  }
        //  filledBuffer = true;
        //}
        //else // error while reading
        //{
        //  pos_ = 0;
        //  end_ = 0;
        //}
      }

      return filledBuffer;
    }

  }
}
