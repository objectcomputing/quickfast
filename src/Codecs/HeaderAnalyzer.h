// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef HEADERANALYZER_H
#define HEADERANALYZER_H
#include "HeaderAnalyzer_fwd.h"
#include <Codecs/DataSource_fwd.h>
#include <Common/Types.h>
namespace QuickFAST{
  namespace Codecs{
    /// An interface to be used to adapt to various styles of block or message header
    class HeaderAnalyzer
    {
    public:
      /// @brief Typical virtual destructor
      virtual ~HeaderAnalyzer()
      {
      }

      /// @brief Analyze the header.
      ///
      /// This method must read the entire header (if any) from the DataSource
      /// leaving it positioned to read the first byte of the actual FAST-encoded
      /// message.
      ///
      /// analyzeHeader should get header bytes by calling source.getByte().  This call
      /// may return false meaning no more bytes are present right now.   In that case
      /// analyzeHeader should save its state and return false -- expecting to be called
      /// again when additional data becomes available.
      ///
      /// If the header contains a block size, the blockSize parameter should be
      /// set to be the number of bytes in the block AFTER THIS HEADER.  Otherwise
      /// blockSize should be set to zero or left unchanged.
      ///
      /// Based on the contents of the header (or the phase of the moon) it may
      /// decide that it does not need the contents of this message and set the
      /// skip parameter accordingly.   The decoder can make of that what it will.
      ///
      /// In particular, even if analyzeHeader returns skip as true, the application may still
      /// see the decoded contents of the block due to restrictions in the protocol.
      ///
      /// @param[in] source for the bytes that make up the header.
      /// @param[out] blockSize (if any) from the header.  zero means unspecified.
      /// @param[out] skip true if this message should ignored if possible.
      /// @returns true if header is complete; false if more data is needed
      virtual bool analyzeHeader(DataSource & source, size_t & blockSize, bool & skip) = 0;

      /// @brief does this header analyzer support sequence numbers
      virtual bool supportsSequenceNumber()const
      {
        return false;
      }
      /// @brief Extract a sequence number from a header
      ///
      /// This message assumes the entire header is available in memory.
      /// This method does NOT consume or modify any data.  It may be called
      /// repeatedly and will produce the same result each time.
      /// @param buffer points to the header
      /// @returns the sequence number;
      virtual uint32 getSequenceNumber(const uchar * buffer) const
      {
        return 0;
      }

      /// @brief reset the header analyzer -- called when something went wrong while analyzing previous header
      virtual void reset()
      {
      }
    };
  }
}
#endif // HEADERANALYZER_H
