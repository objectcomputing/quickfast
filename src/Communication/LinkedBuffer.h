// Copyright (c) 2009, 2010, 2011, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef LINKEDBUFFER_H
#define LINKEDBUFFER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "LinkedBuffer_fwd.h"
#include <Common/Types.h>

namespace QuickFAST
{
  namespace Communication
  {
    ///@brief A buffer of unsigned chars to be stored in a linked list.
    ///
    /// The data can be in an internal buffer "owned" by this LinkedBuffer or
    /// it can live in an external buffer which implies that this LinkedBuffer
    /// is not concerned with the lifetime of the memory space.
    ///
    /// An "extra" void *field in each LinkedBuffer can be used by the external application
    /// to remember information about the external buffer (or for other purposes as needed.)
    ///
    /// A LinkedBuffer also has a flags field containing 32 uncommitted flags that may be
    /// used for whatever purpose is needed.
    class LinkedBuffer
    {
    public:
      /// @brief Construct with a given size
      /// @param capacity is how many bytes to allocate
      LinkedBuffer(size_t capacity)
        : link_(0)
        , buffer_(new unsigned char [capacity])
        , capacity_(capacity)
        , used_(0)
        , extra_(0)
        , flags_(0)
      {
      }

      /// @brief Construct an empty buffer
      ///
      /// This can be used either as a list head in which case it will never contain data
      /// or it can be used to link to external data.
      LinkedBuffer()
        : link_(0)
        , buffer_(0)
        , capacity_(0)
        , used_(0)
        , extra_(0)
      {
      }

      /// @brief Construct: wrap around an external buffer.
      /// @param externalBuffer: where the data is stored
      /// @param used How much data is present.
      /// @param extra opaque information for the actual owner of the buffer
      LinkedBuffer(const unsigned char * externalBuffer, size_t used, void * extra = 0)
        : link_(0)
        , buffer_(const_cast<unsigned char *>(externalBuffer))
        , capacity_(0)
        , used_(used)
        , extra_(extra)
      {
      }

      ~LinkedBuffer()
      {
        if(capacity_ != 0)
        {
          delete[] buffer_;
        }
      }

      /// @brief Access the raw buffer
      /// @returns a pointer to the raw buffer
      unsigned char * get()
      {
        return buffer_;
      }

      /// @brief Constant access to the raw buffer
      /// @returns a const pointer to the raw buffer
      const unsigned char * get() const
      {
        return buffer_;
      }

      /// @brief Support indexing
      unsigned char & operator[](size_t index)
      {
        if((capacity_ == 0 && index >= used_) || index >= capacity_)
        {
          throw std::range_error("LinkedBuffer: Index out of bounds.");
        }
        return buffer_[index];
      }

      /// @brief Support indexing constant
      const unsigned char & operator[](size_t index) const
      {
        if(index >= used_)
        {
          throw std::range_error("LinkedBuffer (const): Index out of bounds.");
        }
        return buffer_[index];
      }

      /// @brief Access the allocated size.
      /// @returns the buffer's capacity
      size_t capacity() const
      {
        return capacity_;
      }

      /// @brief set external buffer
      ///
      void setExternal(const unsigned char * externalBuffer, size_t used, void * extra = 0)
      {
        if(capacity_ != 0)
        {
          delete[] buffer_;
          capacity_ = 0;
        }
        buffer_ = const_cast<unsigned char *>(externalBuffer);
        used_ = used;
        extra_ = extra;
      }

      /// @brief Set the number of bytes used in this buffer
      /// @param used byte count
      void setUsed(size_t used)
      {
        used_ = used;
      }

      /// @brief Access the number of bytes used in this buffer
      /// @returns used byte count
      size_t used()const
      {
        return used_;
      }

      /// @brief Linked List support: Set the link
      /// @param link pointer to buffer to be linked <b>after</b> this one.
      void link(LinkedBuffer * link)
      {
        link_ = link;
      }

      /// @brief Linked List support: access the link.
      /// @returns the buffer <b>after</b> this one.  Zero if none
      LinkedBuffer * link() const
      {
        return link_;
      }

      /// @brief capture extra info when used with external data
      void setExtra(void *extra)
      {
        extra_ = extra;
      }

      /// @brief return extra info when used with external data
      void * extra() const
      {
        return extra_;
      }

      /// @brief Set flag bit(s) in this buffer.
      /// @param mask is a mask of the bit(s) to be set
      void setFlag(uint32 mask)
      {
        flags_ |= mask;
      }

      /// @brief Clear flag bit(s) in this buffer.
      /// @param mask is a mask of the bits(s) to be cleared
      void clearFlag(uint32 mask)
      {
        flags_ &= ~mask;
      }

      /// @brief Check to see if any specified flag bits are set.
      /// @param mask is a mask of the bits(s) to be checked
      /// @returns true if ANY bits are set both in mask and in this buffer.
      bool checkAnyFlag(uint32 mask)const
      {
        return ((flags_ & mask) != 0);
      }

      /// @brief Check to see if any specified bits is set.
      /// @param mask is a mask of the bits(s) to be checked
      /// @returns true if ALL bits set in the mask are set in this buffer in this buffer.
      ///          zero bits in the mask are ignored.
      bool checkAllFlag(uint32 mask)const
      {
        return ((flags_ & mask) == mask);
      }

      /// @brief Retrieve all flag bits in case neither of the check* cases apply.
      /// @returns the entire set of flag bits.
      uint32 getFlags()const
      {
        return flags_;
      }

    private:
      LinkedBuffer * link_;
      unsigned char * buffer_;
      size_t capacity_;
      size_t used_;
      void * extra_;
      uint32 flags_;
    };

  }
}

//TEMPORARY
// For forward compatibility include SingleServerBufferQueue which includes BufferQueue
// They used to be part of this header file.
#include "SingleServerBufferQueue.h"

#endif // LINKEDBUFFER_H
