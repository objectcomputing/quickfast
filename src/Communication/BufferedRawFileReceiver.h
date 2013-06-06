// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef BUFFEREDRAWFILERECEIVER_H
#define BUFFEREDRAWFILERECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "BufferedRawFileReceiver_fwd.h"
#include <Communication/SynchReceiver.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// A Receiver that reads input from an istream.
    class BufferedRawFileReceiver
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
      BufferedRawFileReceiver(std::istream & stream)
      {
        std::streamoff startPosition = stream.tellg();
        stream.seekg(0, std::ios_base::end);
        std::streamoff endPosition = stream.tellg();
        size_ = (size_t)(endPosition - startPosition);
        if(size_ > 0)
        {
          buffer_.reset(new unsigned char[(unsigned int)(endPosition - startPosition)]);
          stream.seekg(startPosition);
          stream.read(reinterpret_cast<char *>(buffer_.get()), size_);
          size_t read = (size_t) stream.gcount();
          if(read != size_)
          {
            size_ = 0;
            buffer_.reset();
          }
        }
      }

      ~BufferedRawFileReceiver()
      {
      }

    private:

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        return size_ > 0;
      }

      // Implement Receiver method
      bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock& lock)
      {
        bool result = size_ > 0;
        if(result)
        {
          buffer->setExternal(buffer_.get(), size_);
          (void) acceptFullBuffer(buffer, size_, lock);
          size_ = 0;
        }
        return result;
      }

      // Implement Receiver method
      virtual void resetService()
      {
        ;
      }

    private:
      size_t size_;
      boost::scoped_array<unsigned char> buffer_;
    };
  }
}
#endif // BUFFEREDRAWFILERECEIVER_H
