// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef PCAPFILERECEIVER_H
#define PCAPFILERECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "PCapFileReceiver_fwd.h"
#include <Communication/SynchReceiver.h>
#include <Communication/PCapReader.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// A Receiver that reads input from an istream.
    class PCapFileReceiver
      : public SynchReceiver
    {
    public:
      /// @brief Wrap a PCapFileReader into a Receiver
      ///
      /// @param filename names the PCap file
      /// @param wordSize is the word size of the platform where the file was captured.
      PCapFileReceiver(
        const std::string & filename,
        size_t wordSize = 0
        )
        : SynchReceiver()
        , filename_(filename)
        , wordSize_(wordSize)
      {
      }

      ~PCapFileReceiver()
      {
      }

    private:

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        if(wordSize_ == 32)
        {
          reader_.set32bit(true);
        }
        else if (wordSize_ == 64)
        {
          reader_.set64bit(true);
        }
        reader_.open(filename_.c_str()); // for debugging, dump to->, &std::cout);
        return reader_.good();
      }

      // Implement Receiver method
      bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock& lock)
      {
        const unsigned char * pcapBuffer;
        size_t pcapSize;
        bool result = reader_.read(pcapBuffer, pcapSize);
        if(result)
        {
          if(pcapSize <= buffer->capacity())
          {
            memcpy(buffer->get(), pcapBuffer, pcapSize);
            acceptFullBuffer(buffer, pcapSize, lock);
            result = true;
          }
        }
        return result;
      }

      // Implement Receiver method
      virtual void resetService()
      {
        return;
      }
    private:
      std::string filename_;
      size_t wordSize_;
      PCapReader reader_;
    };
  }
}
#endif // PCAPFILERECEIVER_H
