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
      /// @brief return -1 if end of data (forever);
      ///        otherwise return n >= 0 for the number of bytes readily available
      virtual int bytesAvailable();

      /// @brief Get the next byte.
      ///
      /// Honors "lookAhead" then calls readByte if necessary
      /// See messageAvailable() for a discussion of blocking in getByte().
      ///
      /// @param[out] byte where to store the byte.
      /// @returns true if successful; false if end of data
      virtual bool getByte(uchar & byte);

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
        RAW,    /// Binary data as read
        HEX,    /// Two hex characters per byte
        NONE    /// No data echoed (only field/message boundaries)
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

    protected:
      /// @brief Get the next byte.
      ///
      /// This is the method that should be implemented by
      /// derived classes so that look-ahead will work correctly.
      /// @param[out] byte where to store the byte.
      /// @returns true if successful; false if end of data
      virtual bool readByte(uchar & byte) = 0;

    protected:
      /// @brief Store a byte that was read before it was needed.
      uchar lookAhead_;
      /// @brief True if lookAhead_ contains a valid byte.
      bool lookedAhead_;
    private:
      std::ostream * echo_;
      bool verboseMessages_;
      bool verboseFields_;
      bool raw_;
      bool hex_;
      size_t byteCount_;
    };
  }
}
#endif // DATASOURCE_H
