// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#ifndef StringBuffer_H
#define StringBuffer_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>

namespace QuickFAST
{
  template<size_t INTERNAL_CAPACITY = 48>
  class StringBufferT
  {
  public:
    static const size_t npos = size_t(-1);
    /// @brief Construct an empty StringBufferT
    StringBufferT()
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
    {
      internalBuffer_[0] = 0;
    }

    /// @brief Copy constructor
    StringBufferT(const StringBufferT& rhs)
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
    {
      assign(rhs.getBuffer(), rhs.size());
    }

    StringBufferT(const std::string & rhs)
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
    {
      assign(reinterpret_cast<const unsigned char *>(rhs.data()), rhs.size());
    }

    /// @brief Construct from a substring
    StringBufferT(
      const StringBufferT& rhs,
      size_t pos,
      size_t length = npos
      )
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
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

    /// @brief Construct from a C style null terminated string
    explicit StringBufferT(const unsigned char* rhs)
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
    {
      assign(rhs, std::strlen(rhs));
    }

    /// @brief construct from a character buffer
    ///
    /// This method is safe to use with non-null-terminated strings
    StringBufferT(const unsigned char* rhs, size_t length)
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
    {
      assign(rhs, length);
    }

    /// @brief construct a StringBufferT with a given length, filled with the specified character.
    StringBufferT(size_t length, unsigned char c)
      : heapBuffer_(0)
      , size_(0)
      , capacity_(INTERNAL_CAPACITY)
    {
      reserve(length);
      unsigned char * buffer = getBuffer();
      std::memset(buffer, c, length);
      buffer[length] = 0;
      size_ = length;
    }

    /// @brief destructor
    ~StringBufferT()
    {
      delete[] heapBuffer_;
    }

    /// @brief change the actual size of the StringBufferT, shrinking or filling it as necessary
    void resize(size_t length, unsigned char fill = 0)
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

    /// @brief assign a QuickFAST::StringBufferT
    StringBufferT& operator=(const StringBufferT& rhs)
    {
      StringBufferT temp(rhs);
      swap(temp);
      return *this;
    }

    /// @brief assign a C style (null terminated) string.
    StringBufferT& operator=(const unsigned char* rhs)
    {
      StringBufferT temp(rhs);
      swap(temp);
      return *this;
    }

    /// @brief assign a single character.
    StringBufferT& operator=(unsigned char rhs)
    {
      StringBufferT temp(rhs, 1);
      swap(temp);
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

    bool operator==(const unsigned char * pattern)const
    {
      return std::strcmp(
        reinterpret_cast<const char *>(pattern),
        reinterpret_cast<const char *>(data())) == 0;
    }

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
    StringBufferT& operator +=(
      const StringBufferT& rhs
      )
    {
      return append(rhs);
    }

    /// @brief append a C-style null terminated string
    StringBufferT& operator +=(
      const unsigned char* rhs
      )
    {
      return append(rhs);
    }

    /// @brief append a single character
    StringBufferT& operator +=(unsigned char rhs)
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
      return append(rhs, std::strlen(rhs));
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



    /// @brief test for empty StringBufferT
    bool empty() const
    {
      return size() == 0;
    }

    /// @brief discard contents, thereby making this an empty StringBufferT.
    void clear()
    {
      delete [] heapBuffer_;
      heapBuffer_ = 0;
      capacity_ = INTERNAL_CAPACITY;
      internalBuffer_[0] = 0;
      size_ = 0;
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
        swap(heapBuffer_, rhs.heapBuffer_);
        swap(size_, rhs.size_);
        swap(capacity_, rhs.capacity_);
      }
    }

    /// @brief how much space is used
    size_t size() const
    {
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
      {
        if (capacity() >= needed)
        {
          return;
        }
        else if (useHeap(needed))
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
            throw std::bad_alloc("StringBufferT growth failed");
          }
          const unsigned char * oldBuffer = getBuffer();
          std::memcpy(newBuffer, oldBuffer, size_);
          newBuffer[size_] = 0;
          capacity_ = needed;
          delete [] heapBuffer_;
          heapBuffer_ = newBuffer;
        }
        // else the internal buffer can hold the string
        // so there's nothing to do.
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

    operator std::string() const
    {
      return std::string(reinterpret_cast<const char *>(getBuffer()), size());
    }

  private:
    /// @brief find the buffer that's presently in use; const version
    const unsigned char * getBuffer() const
    {
      if (heapBuffer_ == 0)
      {
        return internalBuffer_;
      }
      return heapBuffer_;
    }


    /// @brief find the buffer that's presently in use; mutable version
    unsigned char * getBuffer()
    {
      if (heapBuffer_ == 0)
      {
        return internalBuffer_;
      }
      return heapBuffer_;
    }

    /// @brief should we use the heap?  Make sure the test is always consistent.
    bool useHeap(size_t capacity) const
    {
      return capacity > INTERNAL_CAPACITY;
    }

    void swap(size_t & left, size_t & right)
    {
      size_t temp = left;
      left = right;
      right = temp;
    }
    void swap(unsigned char *& left, unsigned char *& right)
    {
      unsigned char * temp = left;
      left = right;
      right = temp;
    }

  private:
    unsigned char internalBuffer_[INTERNAL_CAPACITY + 1];
    unsigned char * heapBuffer_;
    size_t size_;
    size_t capacity_;
  };

  typedef StringBufferT<48> StringBuffer;

  inline
  std::ostream & operator << (std::ostream & out, const StringBuffer &str)
  {
    out << str.c_str();
    return out;
  }
}
#endif /* StringBuffer_H */
