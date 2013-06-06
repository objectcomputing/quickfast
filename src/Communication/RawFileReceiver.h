// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef RAWFILERECEIVER_H
#define RAWFILERECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "RawFileReceiver_fwd.h"
#include <Communication/SynchReceiver.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// A Receiver that reads input from an istream.
    class RawFileReceiver
      : public SynchReceiver
    {
    public:
      /// @brief Wrap a standard istream into a Receiver
      ///
      /// The input stream should be opened in binary mode
      /// if that option is available/needed on your operating
      /// system and stream type. (i.e. specify std::ios::binary
      /// when you open a ofstream on Windows.)
      /// @param stream supplies the data
      RawFileReceiver(
        std::istream & stream
        )
        : stream_(stream)
      {
      }

      ~RawFileReceiver()
      {
      }

    private:

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        return stream_.good() && !stream_.eof();
      }

      // Implement Receiver method
      bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock& lock)
      {
        bool filling = false;
        if(!stopping_ && stream_.good() && !stream_.eof())
        {
          stream_.read(reinterpret_cast<char *>(buffer->get()), buffer->capacity());
          size_t size = (size_t) stream_.gcount();
          if(acceptFullBuffer(buffer, size, lock))
          {
            needService_ = true;
          }
          filling = true;
        }
        return filling;
      }

      // Implement Receiver method
      virtual void resetService()
      {
        return;
      }

    private:
      std::istream & stream_;
      bool needService_;
    };
  }
}
#endif // RAWFILERECEIVER_H
