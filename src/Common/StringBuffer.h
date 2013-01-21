// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#ifdef _MSC_VER
# pragma once
#endif
#ifndef STRINGBUFFER_H
#define STRINGBUFFER_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>

namespace QuickFAST
{

  ///@brief A buffer to hold a string
  ///
  /// std::string would work -- this class actually replaces std::string.
  /// However in order to optimize performance, we want to control
  /// how large the string can be without requiring a heap allocation
  /// the INTERNAL_CAPACITY parameter does that.
  /// We also want to know how many heap allocations were required.
  /// The growCount() method provides that information.
  ///
  /// Secondary usage.  In addition to it's std::string like behavior, a StringBufferT
  /// can also be used as a "facade" for a real std::string.  This is not intended as
  /// a general purpose solution but rather as a crutch to help the MessageAccessor return
  /// a const reference to an StringBuffer when what it really has is an std::string.
  /// To use the StringBuffer this way, use the constructor that takes const std::string *
  /// [note the pointer, not the reference].  The pointed to string must continue to exist
  /// linger than the StringBufferT does, or BAD THINGS[tm] happen.
  template<size_t INTERNAL_CAPACITY = 48>
  class StringBufferT
  {
  public:
    /// @brief define a not-a-valid-position constant
    static const size_t npos = size_t(-1);
    /// @brief Construct an empty StringBufferT
    StringBufferT()
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
      , growCount_(0)
      , delegateString_(0)
    {
      internalBuffer_[0] = 0;
    }

    /// @brief Copy constructor
    /// @param rhs is the data to initialize the string buffer.
    StringBufferT(const StringBufferT& rhs)
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
      , growCount_(0)
      , delegateString_(0)
    {
      assign(rhs.getBuffer(), rhs.size());
    }

    /// @brief copy construct from a standard string
    /// @param rhs is the data to initialize the string buffer.
    StringBufferT(const std::string & rhs)
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
      , growCount_(0)
      , delegateString_(0)
    {
      assign(reinterpret_cast<const unsigned char *>(rhs.data()), rhs.size());
    }

    /// @brief Construct from a substring
    ///
    /// @param rhs the string from which to extract a substring
    /// @param pos is the starting position of the substring
    /// @param length is how long the substring is.  npos means to end-of-string.
    StringBufferT(
      const StringBufferT& rhs,
      size_t pos,
      size_t length = npos
      )
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
      , growCount_(0)
      , delegateString_(0)
    {
      if(pos > rhs.size())
      {
        pos = rhs.size();
      }
      if((length == size_t(npos)) || ((pos + length) > rhs.size()))
      {
        length = rhs.size() - pos;
      }
      const unsigned char * rhsBuffer = rhs.getBuffer();
      assign(rhsBuffer + pos, length);
    }

    /// @brief Construct from an unsigned C style null terminated string
    StringBufferT(const unsigned char* rhs)
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
      , growCount_(0)
      , delegateString_(0)
    {
      assign(rhs, std::strlen(reinterpret_cast<const char *>(rhs)));
    }

    /// @brief Construct from a C style null terminated string
    StringBufferT(const char* rhs)
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
      , growCount_(0)
      , delegateString_(0)
    {
      assign(
        reinterpret_cast<const unsigned char *>(rhs),
        std::strlen(rhs));
    }

    /// @brief construct from a character buffer
    ///
    /// This method is safe to use with non-null-terminated strings
    StringBufferT(const unsigned char* rhs, size_t length)
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
      , growCount_(0)
      , delegateString_(0)
    {
      assign(rhs, length);
    }

    /// @brief construct a StringBufferT with a given length, filled with the specified character.
    StringBufferT(size_t length, unsigned char c)
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
      , growCount_(0)
      , delegateString_(0)
    {
      reserve(length);
      unsigned char * buffer = getBuffer();
      std::memset(buffer, c, length);
      buffer[length] = 0;
      size_ = length;
    }

    /// @brief Special consructor to wrap a StringBufferT around a "real" std::string
    ///
    /// Not intended for general purpose use. I was solving a particular problem.
    /// @param delegateString points to the real string to be wrapped.
    ///        It must exist longer than the StringBufferT.
    StringBufferT(const std::string * delegateString)
      : heapBuffer_(0)
      , size_(0)
      , capacity_(0)
      , growCount_(0)
      , delegateString_(delegateString)
    {
    }



    /// @brief destructor
    ~StringBufferT()
    {
      delete[] heapBuffer_;
    }

    /// @brief change the actual size of the StringBufferT, shrinking or filling it as necessary
    void resize(size_t length, unsigned char fill = 0)
    {
      if(delegateString_ != 0)
      {
        if (size() < length)
        {
          append(length - size(), fill);
        }
        else if (size() > length)
        {
          getBuffer()[length] = 0;
          size_ = length;
        }
      }
    }

    /// @brief assign a QuickFAST::StringBufferT
    StringBufferT<INTERNAL_CAPACITY>& operator=(const StringBufferT& rhs)
    {
      StringBufferT temp(rhs);
      swap(temp);
      return *this;
    }

    /// @brief assign a C style (null terminated) string.
    StringBufferT<INTERNAL_CAPACITY>& operator=(const unsigned char* rhs)
    {
      StringBufferT temp(rhs);
      swap(temp);
      return *this;
    }

    /// @brief assign a single character.
    StringBufferT<INTERNAL_CAPACITY>& operator=(unsigned char rhs)
    {
      StringBufferT temp(rhs, 1);
      swap(temp);
      return *this;
    }

    /// @brief assign from standard string
    /// @param rhs is the data to assign to the string buffer.
    StringBufferT<INTERNAL_CAPACITY> & operator =(const std::string & rhs)
    {
      assign(reinterpret_cast<const unsigned char *>(rhs.data()), rhs.size());
      return *this;
    }

    /// @brief equality comparison
    bool operator==(const StringBufferT& rhs) const
    {
      return (size() == rhs.size() && memcmp(rhs.getBuffer(), getBuffer(), size()) == 0);
    }

    /// @brief inequality comparison
    bool operator!=(const StringBufferT& rhs) const
    {
      return !(*this == rhs);
    }

    /// @brief compare to a C-style (unsigned) string.
    bool operator==(const unsigned char * pattern)const
    {
      return std::strcmp(
        reinterpret_cast<const char *>(pattern),
        reinterpret_cast<const char *>(data())) == 0;
    }

    /// @brief compare to a C-style string.
    bool operator==(const char * pattern)const
    {
      return std::strcmp(
        pattern, reinterpret_cast<const char *>(data())) == 0;
    }

    /// @brief access element of the StringBufferT (read only)
    const unsigned char& operator[](size_t pos) const
    {
      // If pos == size(), the const version returns 0.
      if(pos <= size())
        return *(getBuffer() + pos);
      throw std::range_error("String access out of range.");
    }

    /// @brief access an element of the StringBufferT for possible update
    unsigned char& operator[](size_t pos)
    {
      // If pos == size(), the non-const behavior is undefined.
      if(pos < size())
      {
        return *(getBuffer() + pos);
      }
      throw std::range_error("String access out of range.");
    }

    /// @brief append a QuickFAST::StringBufferT
    StringBufferT<INTERNAL_CAPACITY>& operator +=(
      const StringBufferT& rhs
      )
    {
      return append(rhs);
    }

    /// @brief append an unsigned C-style null terminated string
    StringBufferT<INTERNAL_CAPACITY>& operator +=(
      const unsigned char * rhs
      )
    {
      return append(rhs);
    }

    /// @brief append a C-style null terminated string
    StringBufferT<INTERNAL_CAPACITY>& operator +=(
      const char * rhs
      )
    {
      return append(rhs);
    }

    /// @brief append a single unsigned character
    StringBufferT<INTERNAL_CAPACITY>& operator +=(unsigned char rhs)
    {
      return append(&rhs, 1);
    }

    /// @brief append a single character
    StringBufferT<INTERNAL_CAPACITY>& operator +=(char rhs)
    {
      return append(&rhs, 1);
    }

    /// @brief append a substring from a QuickFAST::StringBufferT
    StringBufferT& append(
      const StringBufferT& rhs,
      size_t pos = 0,
      size_t length = size_t(npos)
      )
    {
      size_t rhsSize = rhs.size();
      if(pos > rhsSize)
      {
        pos = rhsSize;
      }
      if((length == size_t(npos)) || ((pos + length) > rhsSize))
      {
        length = rhsSize - pos;
      }
      return append(rhs.getBuffer() + pos, length);
    }

    /// @brief append a C-style string
    StringBufferT& append(
      const unsigned char* rhs
      )
    {
      return append(
        rhs,
        std::strlen(reinterpret_cast<const char *>(rhs)));
    }

    /// @brief append a C-style string
    StringBufferT& append(
      const char* rhs
      )
    {
      return append(
        rhs,
        std::strlen(rhs));
    }

    /// @brief append from a character buffer.
    StringBufferT& append(
      const unsigned char* rhs,
      size_t length
      )
    {
      if (length > 0)
      {
        reserve(size() + length);
        unsigned char* buf = getBuffer() + size();
        std::memcpy(buf, rhs, length);
        buf[length] = 0;
        size_ += length;
      }
      return *this;
    }

    /// @brief append from a character buffer.
    StringBufferT& append(
      const char* rhs,
      size_t length
      )
    {
      if (length > 0)
      {
        reserve(size() + length);
        unsigned char* buf = getBuffer() + size();
        std::memcpy(buf, rhs, length);
        buf[length] = 0;
        size_ += length;
      }
      return *this;
    }

    /// @brief extend and fill the StringBufferT with the given character
    StringBufferT& append(
      size_t length,
      unsigned char c
      )
    {
      if (length > 0)
      {
        reserve(size() + length);
        unsigned char *buf = getBuffer() + size();
        memset(buf, c, length);
        buf[length] = 0;
        size_ += length;
      }
      return *this;
    }

    /// @brief extend and fill the StringBufferT with the given character
    StringBufferT& append(
      size_t length,
      char c
      )
    {
      if (length > 0)
      {
        reserve(size() + length);
        unsigned char *buf = getBuffer() + size();
        memset(buf, c, length);
        buf[length] = 0;
        size_ += length;
      }
      return *this;
    }

    /// @brief test for empty StringBufferT
    bool empty() const
    {
      return size() == 0;
    }

    /// @brief discard value without discarding buffer
    void erase()
    {
      size_ = 0;
      delegateString_ = 0;
    }

    /// @brief discard contents, thereby making this an empty StringBufferT.
    void clear()
    {
      delete [] heapBuffer_;
      heapBuffer_ = 0;
      capacity_ = INTERNAL_CAPACITY;
      internalBuffer_[0] = 0;
      size_ = 0;
      delegateString_ = 0;
    }

    /// @brief swap the contents of two StringBuffers
    void swap(StringBufferT& rhs)
    {
      if(this != &rhs)
      {
        // if either StringBufferT is using the internal buffer, swap them
        if(heapBuffer_ == 0 || rhs.heapBuffer_ == 0)
        {
          unsigned char origStackBuf[sizeof(internalBuffer_)];
          std::memcpy(origStackBuf, internalBuffer_, sizeof(internalBuffer_));
          std::memcpy(internalBuffer_, rhs.internalBuffer_, sizeof(internalBuffer_));
          std::memcpy(rhs.internalBuffer_, origStackBuf, sizeof(internalBuffer_));
        }
        std::swap(heapBuffer_, rhs.heapBuffer_);
        std::swap(size_, rhs.size_);
        std::swap(capacity_, rhs.capacity_);
        std::swap(growCount_, rhs.growCount_);
        std::swap(delegateString_, rhs.delegateString_);
      }
    }

    /// @brief how much space is used
    size_t size() const
    {
      if(delegateString_)
      {
        return delegateString_->size();
      }
      return size_;
    }

    /// @brief an alias for size
    size_t length() const
    {
      return size();
    }

    /// @brief how much can the current buffer hold?
    size_t capacity() const
    {
      return capacity_;
    }

    /// @brief expose the buffer as constant a C-style string
    /// note that we keep the buffer null terminated
    const unsigned char* c_str() const
    {
      return getBuffer();
    }

    /// @brief expose the buffer as a character buffer not guaranteed to be null terminated.
    const unsigned char* data() const
    {
      return getBuffer();
    }

    /// @brief be sure the StringBufferT can hold at least "needed" bytes.
    void reserve(size_t needed)
    {
      if(delegateString_)
      {
        throw std::logic_error("StringBufferT cannot grow delegated string.");
      }
      if (capacity() < needed)
      {
        // Note: the following line determines the growth factor for the external buffer
        // it can be adjusted to be more or less agressive based on experience.
        size_t newCapacity = capacity() * 2;
        if (needed < newCapacity)
        {
          needed = newCapacity;
        }
        if(needed < size())
        {
          throw std::logic_error("StringBufferT growth calculation incorrect");
        }
        unsigned char * newBuffer(new unsigned char[needed + 1]);
        if(newBuffer == 0)
        {
          throw std::bad_alloc();
        }
        const unsigned char * oldBuffer = getBuffer();
        std::memcpy(newBuffer, oldBuffer, size_);
        newBuffer[size_] = 0;
        capacity_ = needed;
        delete [] heapBuffer_;
        heapBuffer_ = newBuffer;
        growCount_ += 1;
      }
    }

    /// @brief replace the current contents with data from the character buffer
    void assign(
      const unsigned char * source,
      size_t length
      )
    {
      reserve(length);
      unsigned char* buffer = getBuffer();
      std::memcpy(buffer, source, length);
      buffer[length] = 0;
      size_ = length;
    }

    /// @brief cast to a standard string.
    operator std::string() const
    {
      return std::string(reinterpret_cast<const char *>(getBuffer()), size());
    }

    /// @brief How many times did the string grow?
    ///
    /// for testing and performance tuning.
    size_t growCount()const
    {
      return growCount_;
    }

  private:
    /// @brief find the buffer that's presently in use; const version
    const unsigned char * getBuffer() const
    {
      if (heapBuffer_ != 0)
      {
        return heapBuffer_;
      }
      else if(delegateString_ != 0)
      {
        return reinterpret_cast<const unsigned char *>(delegateString_->c_str());
      }
      return internalBuffer_;
    }


    /// @brief find the buffer that's presently in use; mutable version
    unsigned char * getBuffer()
    {
      if (heapBuffer_ != 0)
      {
        return heapBuffer_;
      }
      else if(delegateString_ != 0)
      {
        throw std::logic_error("StringBufferT: Cannot write to delegated string.");
      }
      return internalBuffer_;
    }

  private:
    unsigned char internalBuffer_[INTERNAL_CAPACITY + 1];
    unsigned char * heapBuffer_;
    size_t size_;
    size_t capacity_;
    size_t growCount_;

    const std::string * delegateString_;
  };

  ///@brief typedef the most common use of StringBufferT
  typedef StringBufferT<48> StringBuffer;

  ///@brief support writing a string buffer to an ostream.
  inline
  std::ostream & operator << (std::ostream & out, const StringBuffer &str)
  {
    out << str.c_str();
    return out;
  }
}
#endif /* STRINGBUFFER_H */
