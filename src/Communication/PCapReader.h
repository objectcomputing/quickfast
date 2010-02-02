// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef PCAPREADER_H
#define PCAPREADER_H
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// @brief a helper function object to access data that may need to be byte swapped.
    class ByteSwapper
    {
    public:
      /// @brief construct and enable or disable swapping
      ///
      /// @param swap if true swapping occurs.
      explicit ByteSwapper(bool swap = false)
        : swap_(swap)
      {
      }

      /// @brief change the state of swap after-the-fact
      ///
      /// @param swap if true swapping occurs.
      void setSwap(bool swap)
      {
        swap_ = swap;
      }

      /// @brief conditionally swap an unsigned 16 bit integer
      ///
      /// @param v the value to be swapped
      /// @returns the swapped value
      uint16 operator()(uint16 v) const
      {
        if(swap_)
        {
          return (v >> 8) | (v << 8);
        }
        return v;
      }


      /// @brief conditionally swap a signed 16 bit integer
      ///
      /// @param v the value to be swapped
      /// @returns the swapped value
      int16 operator()(int16 v) const
      {
        if(swap_)
        {
          return ((v >> 8) & 0xFF) | (v << 8);
        }
        return v;
      }


      /// @brief conditionally swap an unsigned 32 bit integer
      ///
      /// @param v the value to be swapped
      /// @returns the swapped value
      uint32 operator()(uint32 v) const
      {
        if(swap_)
        {
          return ((v >> 24) & 0x000000FF)
               | ((v >> 8)  & 0x0000FF00)
               | ((v << 8)  & 0x00FF0000)
               | ((v << 24) & 0xFF000000)
               ;
        }
        return v;
      }


      /// @brief conditionally swap a signed 32 bit integer
      ///
      /// @param v the value to be swapped
      /// @returns the swapped value
      int32 operator()(int32 v) const
      {
        if(swap_)
        {
          return ((v >> 24) & 0x000000FF)
               | ((v >> 8)  & 0x0000FF00)
               | ((v << 8)  & 0x00FF0000)
               | ((v << 24) & 0xFF000000)
               ;
        }
        return v;
      }

    private:
      bool swap_;
    };

    /// @brief Read a PCap file containing UDP packets
    ///
    /// A simple file reader that handles only UDP (and multicast) packets.
    /// For more power, see tcpdump and/or winpcap open source projects.
    ///
    /// PCap is the format used by many communication utility data capture packages
    /// including Wireshark (aka Ethereal) and tcpdump.
    class QuickFAST_Export PCapReader
    {
    public:
      PCapReader();
      /// @brief open a PCap formatted file
      ///
      /// @param filename names the file
      /// @returns true if the open was successful
      bool open(const char * filename);

      /// @brief enable noisy operation for debugging purposes
      ///
      /// @param verbose true turns on the noise.
      void setVerbose(bool verbose);

      /// @brief Start over with the first record in the file.
      ///
      /// @returns true if everything went ok.
      bool rewind();

      /// @brief Check the state of the file.
      ///
      /// @returns true if no errors and not at end of file.
      bool good()const;

      /// @brief Read the next record in the file.
      ///
      /// @param[out] buffer end up pointing to the user data in the packet (headers are bypassed)
      /// @param[out] size contains the number of bytes of user data in the packet (zero is possible and legal!)
      /// @returns true if the read was successful.  False usually means end of data
      bool read(const unsigned char *& buffer, size_t & size);

      /// @brief DEBUG ONLY.  Seek to a particular address.
      ///
      /// since there is no tell() method the address probably came from a verbose display.
      /// not intended for general use.  I needed it while debugging and saw no reason to throw it away.
      /// @param address raw seek address of the beginning of a packet
      void seek(size_t address);

      /// @brief force the reader to expect 64 bit headers even on a 32 bit system.
      ///
      /// Only one of 64bit and 32bit should be set.
      /// @param state turns the 64bit state on or off (default is off)
      void set64bit(bool state = true)
      {
        usetv64_ = state;
      }

      /// @brief force the reader to expect 32 bit headers even on a 64 bit system.
      ///
      /// Only one of 64bit and 32bit should be set.
      /// @param state turns the 32bit state on or off (default is off)
      void set32bit(bool state = true)
      {
        usetv32_ = state;
      }

    private:
      boost::scoped_array<unsigned char> buffer_;
      size_t fileSize_;
      size_t pos_;
      bool ok_;
      bool usetv32_;  // true forces 32 bit header on 64 bit platform
      bool usetv64_;  // true forces 64 bit header on 32 bit platform
                      // neither usetv32_ nor usetv64_ means use native
                      // both is an (undetected) error.
      ByteSwapper swap;
      bool verbose_;
    };
  }
  }
#endif // PCAPREADER_H
