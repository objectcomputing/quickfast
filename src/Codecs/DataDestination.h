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
#include <boost/asio.hpp>
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
    class /*QuickFAST_Export */ DataDestination
    {
    public:
      /// @brief a type for an opaque handle to a buffer within the DataDestination
      typedef size_t BufferHandle;
      /// @brief special handle for a buffer that doesn't exist
      static const BufferHandle NotABuffer = ~0U;

      DataDestination()
        : used_(0)
        , active_(NotABuffer)
        , verboseOut_(0)
      {
      }

      /// @brief Until DataDestinationString is retired, allow for inheritence.
      virtual ~DataDestination()
      {
      }

      /// @brief Enable verbose output as information is added to the data destination
      ///
      /// @param verbose is the stream where the verbose/debug output will be written.
      void setVerbose(std::ostream & verbose)
      {
        verboseOut_ = & verbose;
      }

      /// @brief Turn verbose output off.
      void disableVerbose()
      {
        verboseOut_ = 0;
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
          buffers_.push_back(empty);
        }
        assert(used_ < buffers_.size());
        active_ = used_++;
        buffers_[active_].clear(false);
        return active_;
      }

      /// @brief Append a byte to the end of the currently selected buffer.
      /// @param byte is the datum to be appended.
      void putByte(uchar byte)
      {
        if(active_ == NotABuffer)
        {
          (void)startBuffer();
        }
        buffers_[active_].push(byte);
        if(verboseOut_)
        {
          (*verboseOut_)
            << '[' << active_ << ':' << buffers_[active_].size() << ']'
            << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<unsigned short>(byte)
            << std::setfill(' ') << std::dec << ' ';
          static size_t mod = 0; // keep the lines from getting too long
          if(++mod % 16 == 0) (*verboseOut_) << std::endl;
        }
      }

      /// @brief Get the currently selected buffer
      /// @returns a handle to the buffer that can be used with selectBuffer()
      BufferHandle getBuffer()const
      {
        if(active_ == NotABuffer)
        {
          (void) const_cast<DataDestination *>(this)->startBuffer();
        }
        return active_;
      }

      /// @brief Set the target for subsequent bytes
      /// @param handle is the handle as returned by startBuffer() or getBuffer()
      void selectBuffer(BufferHandle handle)
      {
        active_ = handle;
      }

      /// @brief Discard all buffered data.  Ready to start a new encoding cycle.
      void clear()
      {
        for(size_t handle = 0; handle < buffers_.size(); ++handle)
        {
          buffers_[handle].clear(false);
        }
        used_ = 0;
        active_ = NotABuffer;
      }

      /// @brief Notificaiton that we're starting a new message
      /// @param id identifies the template that will be used.
      void startMessage(template_id_t id)
      {
        if(verboseOut_)
        {
          (*verboseOut_) << std::endl << "**BEGIN MESSAGE: " << id << std::endl;
        }
      }

      /// @brief Notification that we're starting a new field.
      /// @param identity identifies the field.
      void startField(const Messages::FieldIdentity identity)
      {
        if(verboseOut_)
        {
          (*verboseOut_) << std::endl << "**BEGIN FIELD: " << identity.name() << '[' << identity.id() << ']' << std::endl;
        }
      }
      /// @brief Notification that a field has been completely encoded
      /// @param identity identifies the field.
      void endField(const Messages::FieldIdentity identity)
      {
        if(verboseOut_)
        {
          (*verboseOut_) << std::endl << "**END FIELD: " << identity.name() << '[' << identity.id() << ']' << std::endl;
        }
      }

      /// @brief Indicate the message is ready to be sent.
      void endMessage()
      {
        if(verboseOut_)
        {
          (*verboseOut_) << std::endl << "**END MESSAGE" << std::endl;
        }
      }

      /// @brief Convert results to string.
      ///
      /// For best performance, use toWorkingBuffer rather than this method
      /// or else use gather/write treating this object as an asio::ConstBufferSequence
      /// @param result is the Strubg into which the data will be copied.
      void toString(std::string & result)const
      {
        size_t size = 0;
        for(size_t pos = 0; pos < buffers_.size(); ++pos)
        {
          size += buffers_[pos].size();
        }
        result.clear();
        result.reserve(size);
        for(size_t pos = 0; pos < buffers_.size(); ++pos)
        {
          result.append(reinterpret_cast<const char *>(buffers_[pos].begin()),
            buffers_[pos].size());
        }
      }

      /// @brief return the accumulated data in a single buffer
      ///
      /// @param result is the WorkingBuffer into which the data will be copied.
      void toWorkingBuffer(WorkingBuffer & result) const
      {
        size_t size = 0;
        for(size_t pos = 0; pos < buffers_.size(); ++pos)
        {
          size += buffers_[pos].size();
        }
        result.clear(false, size);
        for(size_t pos = 0; pos < buffers_.size(); ++pos)
        {
          result.append(buffers_[pos]);
        }
      }

      /// @brief Support for asio gather writes: forward iterator through buffers
      ///
      /// The intent is for DataDestination to conform to the asio::ConstBufferSequence concept.
      /// This would allow it to be passed directly to the asio::write(v)
      /// Note the implication that if asynch writes are used the DataDestination must
      /// remain intact until the write completes.
      class const_iterator
      {
      public:
        /// @brief construct an iterator pointing into a DataDestinatoin
        /// @param destination is the buffer-container
        /// @param position is the starting position for the iterator.
        const_iterator(const DataDestination & destination, size_t position)
          : destination_(destination)
          , position_(position)
        {
        }

        /// @brief Point iterator to next buffer (preincrement)
        const_iterator & operator ++()
        {
          if(position_ < destination_.size())
          {
            ++position_;
          }
          return *this;
        }

        /// @brief Point iterator to next buffer (postincrement)
        const_iterator operator ++(int)
        {
          const_iterator result(*this);
          if(position_ < destination_.size())
          {
            ++position_;
          }
          return result;
        }

        /// @brief dereference the iterator to find the actual buffer
        boost::asio::const_buffer operator * () const
        {
          const WorkingBuffer & buffer(destination_[position_]);
          return boost::asio::const_buffer(buffer.begin(), buffer.size());
        }

        /// @brief dereference the iterator to find the actual buffer
        boost::asio::const_buffer operator -> () const
        {
          const WorkingBuffer & buffer(destination_[position_]);
          return boost::asio::const_buffer(buffer.begin(), buffer.size());
        }

        /// @brief compare iterators.
        /// @param rhs is the iterator to which this should be compared.
        bool operator == (const_iterator & rhs) const
        {
          return position_ == rhs.position_;
        }

        /// @brief compare iterators.
        /// @param rhs is the iterator to which this should be compared.
        bool operator != (const_iterator & rhs) const
        {
          return position_ != rhs.position_;
        }
      private:
        const_iterator & operator=(const_iterator &); // no autogenerated assignment

      private:
        const DataDestination & destination_;
        size_t position_;
      };

      /// @brief return iterator pointing to the first buffer.
      const_iterator begin()const
      {
        return const_iterator(*this, 0);
      }

      /// @brief return iterator pointing past the last buffer
      const_iterator end() const
      {
        return const_iterator(*this, used_);
      }

      /// @brief return a count of the buffers containing data
      size_t size() const
      {
        return used_;
      }

      /// @brief indexed access to a buffer in the set.
      /// @param index should be < size()
      const WorkingBuffer & operator[](size_t index)const
      {
        return buffers_[index];
      }

    private:
      DataDestination & operator = (const DataDestination &); // forbidden assignment
      DataDestination(const DataDestination &); // forbidden copy constructor

    private:
      /// @brief how many buffers contain data.
      size_t used_;
      /// @brief which buffer will receive pushes
      size_t active_;
      /// @brief A type to store the buffers in vectors
      typedef std::vector<WorkingBuffer> BufferVector;
      BufferVector buffers_;
      /// @brief Where to write noisy/debug output.
      std::ostream * verboseOut_;

    };
  }
}
#endif // DATADESTINATION_H
