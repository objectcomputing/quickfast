// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATADESTINATION_H
#define DATADESTINATION_H
#include "DataDestination_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief An interface for a buffer to hold data for a DataDestination
    class QuickFAST_Export DestinationBuffer
    {
    public:
      /// @brief a typical virtual destructor.
      virtual ~DestinationBuffer()
      {
      }

      /// @brief Put the next byte into the buffer
      /// @param[in] byte to put.
      virtual void putByte(uchar byte) = 0;

      /// @brief Reserve space
      /// @param size the number of bytes expected into this buffer
      virtual void reserve(size_t size) = 0;

    };

    /// @brief An inteface for data destinations to be used by an Encoder.
    ///
    /// A DataDestination holds an ordered set of buffers.  Data to be sent
    /// is added to the end of a buffer, but there is no restriction that the
    /// buffers have to be filled in order.  In particular the presence map and
    /// template ID information may not be known until after subsequent fields are
    /// encoded so the buffer holding them may be filled in after the buffer holding
    /// the fields even though it will appear first in the output stream.
    ///
    /// Different implementations of DataDestination may use scatter/gather I/O; may
    /// send the individual buffers separately; or may assemble them into a common
    /// buffer when endMessage() is called.
    class QuickFAST_Export DataDestination{
    public:
      DataDestination()
      {
      }

      /// @brief a typical virtual destructor.
      virtual ~DataDestination()
      {}

      /// @brief start a new buffer at the end of the set.
      ///
      /// The new buffer will be selected for output automatically
      /// @returns a "handle" to the buffer to be used with selectBuffer()
      DestinationBufferPtr startBuffer()
      {
        activeBuffer_ = allocateBuffer();
        buffers_.push_back(activeBuffer_);
        return activeBuffer_;
      }

      /// @brief Append a byte to the end of the currently selected buffer.
      /// @param byte is the datum to be appended.
      void putByte(uchar byte)
      {
        if(!activeBuffer_)
        {
          startBuffer();
        }
        activeBuffer_->putByte(byte);
      }

      /// @brief Get the currently selected buffer
      /// @returns a handle to the buffer that can be used with selectBuffer()
      const DestinationBufferPtr & getBuffer()const
      {
        return activeBuffer_;
      }

      /// @brief Set the target for subsequent bytes
      /// @param buffer is the handle as returned by startBuffer() or getBuffer()
      void selectBuffer(DestinationBufferPtr buffer)
      {
        activeBuffer_ = buffer;
      }

      /// @brief Indicate the message is ready to be sent.
      virtual void endMessage() = 0;
    protected:
      /// @brief implementation specfic buffer allocator
      /// @returns a pointer to the newly allocated buffer
      virtual DestinationBufferPtr allocateBuffer() = 0;

    protected:
      /// @brief A type to store the buffers in vectors
      typedef std::vector<DestinationBufferPtr> BufferVector;
      /// @brief the currently defined buffers
      BufferVector buffers_;
      /// @brief the currently active buffer
      DestinationBufferPtr activeBuffer_;
    };
  }
}
#endif // DATADESTINATION_H
