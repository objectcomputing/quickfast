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
      typedef size_t BufferHandle;
      static const size_t NotABuffer = ~0;

      DataDestination()
        : used_(0)
        , capacity_(0)
        , active_(NotABuffer)
      {
      }

      /// @brief a typical virtual destructor.
      virtual ~DataDestination()
      {}

      /// @brief start a new buffer at the end of the set.
      ///
      /// The new buffer will be selected for output automatically
      /// @returns a "handle" to the buffer to be used with selectBuffer()
      BufferHandle startBuffer()
      {
        if(used_ == capacity_)
        {
          allocateBuffer();
        }
        assert(used_ < capacity_);
        active_ = used_;
        used_++;
        return active_;
      }

      /// @brief Append a byte to the end of the currently selected buffer.
      /// @param byte is the datum to be appended.
      void putByte(uchar byte)
      {
        if(active_ == NotABuffer)
        {
          startBuffer();
        }
        putByte_i(active_, byte);
      }

      /// @brief Get the currently selected buffer
      /// @returns a handle to the buffer that can be used with selectBuffer()
      BufferHandle getBuffer()const
      {
        return active_;
      }

      /// @brief Set the target for subsequent bytes
      /// @param buffer is the handle as returned by startBuffer() or getBuffer()
      void selectBuffer(BufferHandle handle)
      {
        active_ = handle;
      }

      void clear()
      {
        for(size_t pos = 0; pos < capacity_; ++pos)
        {
          clear_i(pos);
        }
        used_ = 0;
        active_ = 0;
      }

      /// @brief Indicate the message is ready to be sent.
      virtual void endMessage() = 0;

    protected:
      /// @brief implementation specfic buffer allocator
      /// @returns a pointer to the newly allocated buffer
      virtual void allocateBuffer() = 0;
      virtual void putByte_i(BufferHandle handle, uchar byte) = 0;
      /// @brief Clear the contents of the destination to prepare for reuse
      virtual void clear_i(BufferHandle handle) = 0;

    protected:
      size_t used_;
      size_t capacity_;
      size_t active_;
    };
  }
}
#endif // DATADESTINATION_H
