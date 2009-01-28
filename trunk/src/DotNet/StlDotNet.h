// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#pragma once

#include <streambuf>
#include <string>
#include <vector>
#include <vcclr.h>

using namespace System;

namespace StlDotNet {


  template<typename StdStringType>
  void MarshalString( System::String^ stringIn, StdStringType& stdStringOut ) {
     using namespace Runtime::InteropServices;
     typedef typename StdStringType::value_type FTChar;

     FTChar* chars = reinterpret_cast<FTChar*>((Marshal::StringToHGlobalAnsi(stringIn)).ToPointer());
     stdStringOut = chars;
     Marshal::FreeHGlobal(IntPtr((void*)chars));
  }

  template<typename StdStringType>
  StdStringType string_cast(System::String^ stringIn)
  {
    StdStringType stdStrOut;
    MarshalString(stringIn, stdStrOut);
    return stdStrOut;
  }

  template<typename StdString>
  System::String^ string_cast(const StdString& stdString)
  {
    return gcnew System::String(stdString.c_str());
  }

  /// Stream buffer that operates on a System.IO.Stream as the underlying data source.
  public class StreamBuf :public std::streambuf
  {
    public:
        StreamBuf(System::IO::Stream^ managedStream):
            managedStream_(managedStream)
        {
            setg(0, 0, 0);
        }


    private:
        gcroot<System::IO::Stream^> managedStream_;




        virtual pos_type seekoff(off_type _Off, std::ios_base::seekdir _Way, std::ios_base::openmode _Which)
        {
          typedef System::IO::SeekOrigin FTSeekOrigin;
          FTSeekOrigin seekOrigin;

          if(!managedStream_->CanSeek)
            return std::streampos(std::_BADOFF);

          // change position by offset, according to way and mode
          switch(_Way)
          {
            case std::ios_base::beg:
              seekOrigin = FTSeekOrigin::Begin;
              break;
            case std::ios_base::cur:
              seekOrigin = FTSeekOrigin::Current;
              break;
            case std::ios_base::end:
              seekOrigin = FTSeekOrigin::End;
              break;
          };

          try
          {
            //May have a loss of data here depending on the size of streamoff.  Seek returns a long long.
            std::streamoff currPosition = static_cast<std::streamoff>(managedStream_->Seek(_Off, seekOrigin));
            return (std::streampos(currPosition));
          }
          catch(...)
          {
            return (std::streampos(std::_BADOFF));
          }
        }

        virtual pos_type seekpos(pos_type,
          std::ios_base::openmode = std::ios_base::in)
        {	// change to specified position, according to mode
          return (std::streampos(std::_BADOFF));
        }

        virtual int_type uflow()
        {	// get a character from stream, point past it
          int_type retVal = managedStream_->ReadByte();

          return (retVal != -1)? retVal : traits_type::eof();
        }

        virtual int_type underflow()
        {
          //We should not get here.
          throw gcnew System::NotImplementedException();
        }

        // copy ctor and assignment not implemented;
        // copying not allowed
        StreamBuf(const StreamBuf &);
        StreamBuf &operator= (const StreamBuf &);
  };

  template <typename T>
  struct IteratorHolder
  {
    typedef T iterator;
    IteratorHolder(iterator beginIt, iterator endIt): it(beginIt), end(endIt){}

    T it;
    T end;
  };

}
