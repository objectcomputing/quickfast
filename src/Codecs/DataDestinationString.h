// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATADESTINATIONSTRING_H
#define DATADESTINATIONSTRING_H
#include <Codecs/DataDestination.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief A data destination that collects the output into a string.
    class QuickFAST_Export DataDestinationString : public DataDestination
    {
      DataDestinationString();
    public:
      /// @brief Construct an empty destination
      /// @brief a typical virtual destructor.
      virtual ~DataDestinationString();
      virtual void endMessage();
      /// @brief access the data written to the destination as a string
      const std::string & getValue()const;
    protected:
      virtual void allocateBuffer();
      virtual void putByte(BufferHandle handle, uchar byte);

      virtual void clear();

    private:
      /// @brief A type to store the buffers in vectors
      typedef std::vector<std::string> BufferVector;
      BufferVector buffers_;
      std::string value_;
    };
  }
}
#endif // DATADESTINATIONSTRING_H
