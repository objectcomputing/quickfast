// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef PRESENCEMAP_H
#define PRESENCEMAP_H
#include "PresenceMap_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>
#include <Codecs/DataSource_fwd.h>
#include <Codecs/DataDestination_fwd.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief The internal representation of a FAST Presence Map.
    ///
    /// One bit per conditionally present field.  See the FAST
    /// protocol documentation available from:
    /// http://www.fixprotocol.org/fast
    /// for details on when a presence map bit is used for a field.
    class QuickFAST_Export PresenceMap{
      /// How many bytes can be stored in this object without allocating additional memory
      /// Consider ways to optimize this after parsing templates.
      const static size_t defaultByteCapacity_ = 20;
    public:
      /// @brief Construct a presence map that may contain up to bitCount fields.
      /// @param bitCount how many fields can be represented in the presence map.
      PresenceMap(size_t bitCount);

      /// @brief comparison operator requires bit-for-bit match
      bool operator == (const PresenceMap &  rhs)const;

      /// @brief Read a presence map from a data source.
      /// @param source provides the data.
      void decode(DataSource & source);

      /// @brief Decode directly from a buffer which must be complete in memory.
      ///
      /// @param buffer points to a fast encoded buffer;
      /// @param[in,out] pos is the position in the buffer. It will be updated to point beyond the presence map
      void decode(const unsigned char * buffer, size_t &pos);


      /// @brief Return the number of bytes needed to encode this PMAP
      ///
      /// Zero means no pmap bits were used.
      size_t encodeBytesNeeded()const;

      /// @brief Encode this presence map into a data destination.
      ///
      /// @param destination where the data is written
      void encode(DataDestination & destination);

      /// @brief Stuff a raw representation of the presence map into this object.
      ///
      /// Intended for testing/debugging.  Avoid using this in production code.
      /// @param buffer points to a presence map in "wire" format.
      /// @param byteLength counts the BYTES in the buffer.
      void setRaw(const uchar * buffer, size_t byteLength);

      /// @brief Peek at the raw representation of the presence map in this object.
      ///
      /// Intended for testing/debugging.  Avoid using this in production code.
      /// @param[out] buffer will be set to point to a presence map in "wire" format
      /// @param[out] byteLength will be set to the count of the BYTES in the buffer.
      void getRaw(const uchar *& buffer, size_t &byteLength)const;

      /// @brief Reset the presence map to the first field with changing the actual map.
      void rewind();

      /// @brief Check to see if the next field is present.
      ///
      /// Note this "consumes" a bit in the presence map.
      /// Do not mix calls to setNextField() and checkNextField() without
      /// an intervening rewind() or reset().
      /// @returns true if the field is present.
      bool checkNextField();

      /// @brief Special support for Arca FAST-like (ish) encoding
      ///
      /// Check a specific bit. Do not change PMAP position
      /// @param bit is the bit to be checked.
      /// @returns true if the bit is set
      bool checkSpecificField(size_t bit);

      /// @brief Reinitialize the presence map to be empty with room for bitCount fields.
      /// @param bitCount how many fields can be represented in the presence map.
      void reset(size_t bitCount = 0);

      /// @brief indicate whether the next field is present.
      ///
      /// Note this "consumes" a position in the presence map.
      /// Do not mix calls to setNextField() and checkNextField() without
      /// an intervening rewind() or reset().
      void setNextField(bool present);

      /// @brief Provide an ostream to which verbose output can be written for debug/analysis
      /// @param vout is the address of the output stream (zero disables verbosity)
      void setVerbose(std::ostream * vout)
      {
        vout_ = vout;
      }

    private:
      void appendByte(size_t & pos, uchar byte);
      void grow();
      void verboseSetNext(bool present);
      void verboseCheckNextField(bool result);
      void verboseCheckSpecificField(size_t bit, size_t byte, uchar bitMask, bool result);

      /// @brief convert byte to bit# in stop bit encoded pmap
      /// slow-- not for production use
      size_t maskToBitNumber(uchar bitMask);
      size_t bitNumber(size_t byteOffset, uchar bitMask)
      {
        return byteOffset * 7 + maskToBitNumber(bitMask);
      }

    private:
      static const uchar startByteMask = '\x40';
      uchar bitMask_;
      size_t bytePosition_;
      size_t byteCapacity_;
      uchar internalBuffer_[defaultByteCapacity_];
      boost::scoped_array<uchar> externalBuffer_;
      uchar * bits_;
      std::ostream * vout_;
    };

    inline
    void
    PresenceMap::setNextField(bool present)
    {
      if(bytePosition_ >= byteCapacity_)
      {
        grow();
      }
      if(present)
      {
        bits_[bytePosition_] |= bitMask_;
      }
      else
      {
        bits_[bytePosition_] &= ~bitMask_;
      }
      if(vout_)
      {
        verboseSetNext(present);
      }
      bitMask_ >>= 1;
      if(bitMask_ == 0)
      {
        bitMask_ = startByteMask;
        bytePosition_ += 1;
      }
    }

    inline
    bool
    PresenceMap::checkNextField()
    {
      if(bytePosition_ >= byteCapacity_)
      {
        if(vout_)(*vout_) << "pmap:at end [" << bytePosition_ << "]" << std::endl;
        return false;
      }
      bool result = (bits_[bytePosition_] & bitMask_) != 0;
      if(vout_)
      {
        verboseCheckNextField(result);
      }
      bitMask_ >>= 1;
      if(bitMask_ == 0)
      {
        bitMask_ = startByteMask;
        bytePosition_ += 1;
      }
      return result;
    }

    inline
    bool
    PresenceMap::checkSpecificField(size_t bit)
    {
      size_t byte = bit / 7;
      if(byte >= byteCapacity_)
      {
        return false;
      }
      size_t bitNum = bit % 7;
      unsigned char bitmask = startByteMask >> bitNum;
      bool result = ((bits_[byte] & bitmask) != 0);
      if(vout_)
      {
        verboseCheckSpecificField(bit, byte, bitmask, result);
      }
      return result;
    }

  }
}
#endif // PRESENCEMAP_H
