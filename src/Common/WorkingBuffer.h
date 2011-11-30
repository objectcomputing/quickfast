// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef WORKINGBUFFER_H
#define WORKINGBUFFER_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>


namespace QuickFAST{
  /// @brief A helper buffer for QuickFAST encoding and decoding.
  ///
  /// Some of the FAST encoding/decoding operations need an array
  /// of bytes (unsigned characters) in which to work.   The required size may
  /// vary based on the date to be Xcoded. The buffer manages it's own
  /// size and readily supports forward and reverse access as needed by
  /// the various algorithms.  This simplifies the code in the algorithms.
  ///
  /// Note: QuickFAST keeps a working buffer available for every
  /// stream.   This buffer tends to grow to an appropriate size for the stream
  /// with which it is being used and stay at that size -- thereby avoiding
  /// repeated allocation/deletion/resize operations.
  ///
  class QuickFAST_Export WorkingBuffer
  {
  public:
    WorkingBuffer();
    ~WorkingBuffer();
    /// @brief Copy constructor
    /// @param rhs The buffer to be copied.
    WorkingBuffer(const WorkingBuffer & rhs);
    /// @brief Assignment
    /// @param rhs The buffer to be into this.
    WorkingBuffer & operator =(const WorkingBuffer & rhs);
    /// @brief Swap the contents of two WorkingBuffers
    /// @param rhs receives the contents of this while this receives the contents of rhs.
    void swap(WorkingBuffer & rhs);

    /// @brief Prepare a buffer for use.
    ///
    /// Discards previous contents of the buffer
    /// Sets the direction of access for buffer insertions (push())
    /// Read operations always go forward.
    /// Optionally reserves a minimum amount of space for the buffer.
    /// @param reverse is true for lifo; false for fifo
    /// @param capacity is the minimum size expected to be used.
    void clear(bool reverse, size_t capacity = 0);

    /// @brief add a byte to the buffer
    ///
    /// the reverse parameter is honored so this is either push_back or push_front
    /// @param byte is the data to be added.
    void push(uchar byte);

    /// @brief Support forward iteration from the beginning of the buffer
    /// @returns a uchar* to be used as an iterator
    const uchar * begin()const
    {
      return buffer_.get() + startPos_;
    }

    /// @brief Support forward iteration to the end of the buffer.
    /// @returns a uchar* to be used as an iterator
    const uchar * end()const
    {
      return buffer_.get() + endPos_;
    }

    /// @brief Discard a byte from the front of the buffer (ignores "reverse")
    void pop_front();

    /// @brief How many byte does the buffer presently contain.
    /// @returns the number of bytes that have been pushed since the most recent clear
    size_t size()const
    {
      return endPos_ - startPos_;
    }

    /// @brief Support indexed read access to the buffer.
    ///
    /// Warning: no error checking.
    const uchar & operator[](size_t index)const
    {
      return buffer_[startPos_ + index];
    }

    /// @brief How many bytes can the buffer hold without growing
    size_t capacity()const
    {
      return capacity_;
    }

    ///@brief Apeend one working buffer to another
    ///
    /// if reverse append to the front of this buffer else append to the back
    /// If the values of "reverse" differ, the results may not be as expected.
    ///
    /// @param rhs the buffer to be appended
    void append(const WorkingBuffer & rhs);

    /// @brief A convenience method: copy contents to a std::string
    void toString(std::string & result) const;

    /// @brief Display the contents of the working buffer in hex
    ///
    /// The display always ends with a newline.
    /// @param out is the destination to which the display will be written.
    /// @param wrap is how many bytes to display per line.  Zero means no wrapping
    void hexDisplay(std::ostream & out, size_t wrap = 16) const;

    /// @brief comparison operator
    ///
    /// Compares the size and contents of the buffers for equality
    /// @param rhs the buffer to be compared with
    bool operator ==(const WorkingBuffer & rhs) const;

  private:
    void grow();
    void grow(size_t newCapacity);

  private:
    bool reverse_;
    size_t capacity_;
    size_t startPos_;
    size_t endPos_;
    boost::scoped_array<uchar> buffer_;
  };
}

#endif /* WORKINGBUFFER_H */
