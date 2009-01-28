// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef PRESENCEMAP_H
#define PRESENCEMAP_H
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
    public:
      /// @brief Construct a presence map that may contain up to bitCount fields.
      /// @param bitCount how many fields can be represented in the presence map.
      PresenceMap(size_t bitCount);

      /// @brief comparison operator requires bit-for-bit match
      bool operator == (const PresenceMap &  rhs)const;

      /// @brief Read a presence map from a data source.
      /// @param source provides the data.
      /// @returns true if a presence map was found.  False normally means EOF.
      bool decode(DataSource & source);


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

    private:
      uchar bitMask_;
      size_t bytePosition_;
      // IMPORTANT: byteLength_ must appear before bits_ in the header file
      // to insure proper initialization
      size_t byteLength_;
      boost::scoped_array<uchar> bits_;
      std::ostream * vout_;
    };
  }
}
#endif // PRESENCEMAP_H
