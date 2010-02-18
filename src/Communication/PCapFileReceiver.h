// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef PCAPFILERECEIVER_H
#define PCAPFILERECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "PCapFileReceiver_fwd.h"
#include <Communication/Receiver.h>
#include <Communication/PCapReader.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// A Receiver that reads input from an istream.
    class PCapFileReceiver
      : public Receiver
    {
    public:
    /// @brief Wrap a standard istream into a Receiver
    ///
    /// The input stream should be opened in binary mode
    /// if that option is available/needed on your operating
    /// system and stream type. (i.e. specify std::ios::binary
    /// when you open a ofstream on Windows.)
    /// @param stream supplies the data
      PCapFileReceiver(
        const std::string & filename
        )
        : Receiver()
        , filename_(filename)
      {
      }

      ~PCapFileReceiver()
      {
      }

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        reader_.open(filename_.c_str());
        return reader_.good();
      }

    private:

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

      virtual void run()
      {
        while(!stopping_ && reader_.good())
        {
          tryServiceQueue();
        }
      }

    private:
      std::string filename_;
      PCapReader reader_;
    };
  }
}
#endif // PCAPFILERECEIVER_H
