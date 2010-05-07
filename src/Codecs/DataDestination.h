// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATADESTINATION_H
#define DATADESTINATION_H
#include "DataDestination_fwd.h"
#include <Messages/FieldIdentity.h>
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>
#include <Common/WorkingBuffer.h>
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
    class /*QuickFAST_Export */ DataDestination{
    public:
      typedef size_t BufferHandle;
      static const BufferHandle NotABuffer = ~0;

      struct iovec
      {
        void * iov_base;
        size_t iov_len;
      };

      typedef iovec * IoVecArray;

      DataDestination()
        : used_(0)
        , active_(NotABuffer)
        , verbose_(false)
        , iovecCapacity_(0)
        , iovecUsed_(0)
      {
      }

      /// @brief Until DataDestinationString is retired, allow for inheritence.
      virtual ~DataDestination()
      {}

      void setVerbose(bool verbose)
      {
        verbose_ = verbose;
      }

      /// @brief start a new buffer at the end of the set.
      ///
      /// The new buffer will be selected for output automatically
      /// @returns a "handle" to the buffer to be used with selectBuffer()
      BufferHandle startBuffer()
      {
        if(used_ == buffers_.size())
        {
          WorkingBuffer empty;
          empty.clear(false);
          buffers_.push_back(empty);
        }
        assert(used_ < buffers_.size());
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
        buffers_[active_].push(byte);
        if(verbose_)
        {
          std::cout << '[' << active_ << ':' << buffers_[active_].size() << ']' << std::hex << std::setw(2) << std::setfill('0') << unsigned short(byte) << std::setfill(' ') << std::dec << ' ';
          static size_t mod = 0;
          if(++mod % 16 == 0) std::cout << std::endl;
        }
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
        for(size_t handle = 0; handle < buffers_.size(); ++handle)
        {
          buffers_[handle].clear(false);
        }
        used_ = 0;
        active_ = 0;
      }

      void startMessage(template_id_t id)
      {
        if(verbose_)
        {
          std::cout << std::endl << "**BEGIN MESSAGE: " << id << std::endl;
        }
      }

      void startField(Messages::FieldIdentityCPtr & identity)
      {
        if(verbose_)
        {
          std::cout << std::endl << "**BEGIN FIELD: " << identity->name() << '[' << identity->id() << ']' << std::endl;
        }
      }

      /// @brief Indicate the message is ready to be sent.
      void endMessage()
      {
        if(verbose_)
        {
          std::cout << std::endl << "**END MESSAGE" << std::endl;
        }
      }

      /// @brief access the data written to the destination as a string
      void getIOVector(IoVecArray & iovector, size_t count)const
      {
        size_t size = buffers_.size();
        if( size >= iovecCapacity_)
        {
          iovector_.reset(new iovec[size]);
          iovecCapacity_ = size;
        }
        for(iovecUsed_ = 0; iovecUsed_ < size; ++iovecUsed_)
        {
          iovector_[iovecUsed_].iov_base = const_cast<uchar *>(buffers_[iovecUsed_].begin());
          iovector_[iovecUsed_].iov_len = buffers_[iovecUsed_].size();
        }
        iovector = iovector_.get();
        count = iovecUsed_;
      }

      /// @brief Convert results to string: for testing NOT production.
      void toString(std::string & result)const
      {
        size_t size = 0;
        for(size_t pos = 0; pos < buffers_.size(); ++pos)
        {
          size += buffers_[pos].size();
        }
        result.reserve(size);
        for(size_t pos = 0; pos < buffers_.size(); ++pos)
        {
          result.append(reinterpret_cast<const char *>(buffers_[pos].begin()),
            buffers_[pos].size());
        }
      }

    private:
      size_t used_;
      size_t active_;
      bool verbose_;

      /// @brief A type to store the buffers in vectors
      typedef std::vector<WorkingBuffer> BufferVector;
      BufferVector buffers_;
      mutable boost::scoped_array<iovec> iovector_;
      mutable size_t iovecCapacity_;
      mutable size_t iovecUsed_;

    };
  }
}
#endif // DATADESTINATION_H
