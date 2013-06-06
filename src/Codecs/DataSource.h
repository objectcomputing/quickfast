// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATASOURCE_H
#define DATASOURCE_H
#include "DataSource_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>
#include <Common/StringBuffer.h>
#include <Common/Exceptions.h>
#include <Application/DecoderConfiguration_fwd.h>
namespace QuickFAST{
  namespace Codecs{
    /// An inteface for data sources to be used by a Decoder.
    class QuickFAST_Export DataSource{
    public:
      /// @brief Construct
      DataSource();

      /// @brief Typical virtual destructor
      virtual ~DataSource();

      /// @brief Is a complete message available?
      ///
      /// This method can be used to avoid beginning the decode process until
      /// a complete message is available for decoding.
      ///
      /// For synchronous decoding:
      ///  -  If getByte() is designed to block this message can just
      ///     return 1 meaning "it *will* be available when needed."
      ///  -  If getByte() is nonblocking then this method should block
      ///     until a complete has arrived, then return 1.
      ///  -  For synchronous operation this method should never return 0.
      ///
      /// For asynchronous decoding:
      ///  -  This method should be called in an event loop or reactor.
      ///     It should not block; just return 0 until there is a
      ///     complete message available, then return 1.
      ///     For asynchronous operation, getByte should
      ///     never block.
      ///
      /// @returns -1 if end of data; 0 if no message available now; 1 if a message should be decoded
      virtual int messageAvailable();

      /// @brief Check for incoming data
      ///
      /// The actual value returned is not critical.  The decoder checks
      ///  >= 0 to detect EOF.
      ///
      /// @return -1 if end of data (forever);
      ///        otherwise return n >= 0 for the number of bytes readily available
      virtual int bytesAvailable();

      /// @brief How many bytes remain in current buffer?
      ///
      /// Never updates the buffers.
      ///
      /// @return number of bytes in current buffer.
      inline
      size_t currentBytesAvailable()
      {
          return size_ - position_;
      }

      /// @brief Check for contiguous bytes in current buffer
      /// @param needed is the number of contiguous bytes needed
      /// @param[out] buffer points to beginning of contiguous area if return is true
      /// @returns true if contiguous data is available
      inline
      bool hasContiguous(size_t needed, const uchar *& buffer)
      {
        buffer = buffer_ + position_;
        return (position_ + needed <= size_);
      }

      /// @brief Skip over contiguous bytes
      ///
      /// presumably after calling hasContiguous to find the contiguous bytes
      /// @param used how many to skip. must be <= needed in the hasContiguous() call.
      inline
      void skipContiguous(size_t used)
      {
        if(echo_)
        {
          size_t end = position_ + used;
          while (position_ < end)
          {
            doEcho(position_ < size_, buffer_[position_]);
            ++position_;
          }
        }
        else
        {
          position_ += used;
        }
        if(position_ > size_)
        {
          throw InternalError("[ERR I90]", "Too many contiguous bytes read from DataSource.");
        }
      }

      /// @brief Get the next byte.
      ///
      /// @param[out] byte where to store the byte.
      /// @returns true if successful; false if end of data
      inline
      bool getByte(uchar & byte)
      {
        bool ok = true;
        if(position_ < size_)
        {
          byte = buffer_[position_++];
        }
        else if(getBuffer(buffer_, size_))
        {
          position_ = 0;
          byte = buffer_[position_++];
        }
        else
        {
          ok = false;
        }

        if(echo_)
        {
          doEcho(ok, byte);
        }
        return ok;
      }

      /// @brief A FYI from the decoder to tell the DataSource about a message boundary.
      /// No action is required but some data sources can do interesting things with
      /// the information
      virtual void beginMessage();

      /// @brief A FYI from the decoder to tell the DataSource about a field boundary.
      /// No action is required but some data sources can do interesting things with
      /// the information
      virtual void beginField(const std::string & name);

      /// @brief select mode to echo data
      enum EchoType
      {
        RAW = Application::DecoderConfigurationEnums::RAW,    /// Binary data as read
        HEX = Application::DecoderConfigurationEnums::HEX,    /// Two hex characters per byte
        NONE = Application::DecoderConfigurationEnums::NONE   /// No data echoed (only field/message boundaries)
      };

      /// @brief Control Echo for debugging/diagnostics
      ///
      /// As an aid to debugging and or diagnosing problems, the DataSource
      /// can echo every input byte to an "echo" stream, either in binary or hex.
      /// Either HEX, RAW, or NONE should be specified.
      /// If NONE is selected, then the message and field info can
      /// be used to find message and field boundaries in the input data stream.
      /// @param echo output stream to receive datal
      /// @param echoType Either RAW, HEX, or NONE to dump raw or "human readable" data to output.
      /// @param verboseMessages if true, echo message boundaries.
      /// @param verboseFields if true, echo field boundaries.
      void setEcho(
        std::ostream & echo,
        const EchoType& echoType = HEX,
        bool verboseMessages = true,
        bool verboseFields = false);

      /// @brief get the echo stream so others can share it.
      ///
      /// @returns a pointer to the echo stream; 0 means no echo
      std::ostream * getEcho()const
      {
        return echo_;
      }

      /// @brief Discard any remaining contents and prepare for new data.
      void reset()
      {
        size_ = 0;
        position_ = 0;
        buffer_ = 0;
      }

    protected:
      /// @brief Honor the echo parameters
      /// @param ok the result about to be returned from getByte
      /// @param byte the byte found by getByte
      void doEcho(bool ok, uchar byte);

      /// @brief get a new buffer (releasing the old one)
      ///
      /// Free the previous buffer and start a new one.
      /// @param[out] buffer should be set to point to a bufferfull of data
      /// @param[out] size should be set to the size of the buffer. If return is true, size must be > 0!
      /// @returns true if more data is available;  false means EOF and stops the decoder
      virtual bool getBuffer(const uchar *& buffer, size_t & size) = 0;

    private:
      /// current buffer being delivered to decoder
      const uchar * buffer_;
      /// size of current buffer
      size_t size_;
      /// position within current buffer
      size_t position_;
    protected:
      /// Where echo output gets written
      std::ostream * echo_;
      /// Echo message boundaries
      bool verboseMessages_;
      /// echo field boundaries
      bool verboseFields_;
      /// echo as raw binary
      bool raw_;
      /// echo in hex
      bool hex_;
      
      /// Position in stream to be displayed by echo
      size_t byteCount_;
      /// used to build the echo message
      StringBuffer echoString_;
      /// Are we decoding a pmap?
      bool isPmap_;
    };
  }
}
#endif // DATASOURCE_H
