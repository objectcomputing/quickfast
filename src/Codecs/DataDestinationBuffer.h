// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATADESTINATIONBUFFER_H
#define DATADESTINATIONBUFFER_H
#include <Codecs/DataDestination.h>
#include <Common/WorkingBuffer.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief A data destination that collects the output into a string.
    class QuickFAST_Export DataDestinationBuffer : public DataDestination
    {
    public:
      struct iovec
      {
        void * iov_base;
        size_t iov_len;
      };

      typedef iovec * IoVecArray;

      /// @brief Construct an empty destination
      DataDestinationBuffer();
      /// @brief a typical virtual destructor.
      virtual ~DataDestinationBuffer();

      virtual void endMessage();

      /// @brief access the data written to the destination as a string
      void getValue(IoVecArray & iovector, size_t count)const;

      /// @brief Convert results to string: for testing NOT production.
      std::string toString()const;

    protected:
      virtual void allocateBuffer();
      virtual void putByte_i(BufferHandle handle, uchar byte);
      virtual void clear_i(BufferHandle handle);

    private:
      /// @brief A type to store the buffers in vectors
      typedef std::vector<WorkingBuffer> BufferVector;
      BufferVector buffers_;
      mutable boost::scoped_array<iovec> iovector_;
      mutable size_t iovecCapacity_;
      mutable size_t iovecUsed_;
    };
  }
}
#endif // DATADESTINATIONBUFFER_H
