// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATADESTINATIONSTRING_H
#define DATADESTINATIONSTRING_H
#include <Codecs/DataDestination.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief A data buffer that collects the output into a string.
    class QuickFAST_Export DestinationBufferString : public DestinationBuffer
    {
    public:
      /// @brief a typical virtual destructor.
      virtual ~DestinationBufferString();

      virtual void putByte(uchar byte);
      virtual void reserve(size_t size);

      /// @brief access the resulting value.
      const std::string & value() const;

    private:
      std::string buffer_;

    };

    /// @brief A data destination that collects the output into a string.
    class QuickFAST_Export DataDestinationString : public DataDestination
    {
    public:
      /// @brief Construct an empty destination
      DataDestinationString();
      /// @brief a typical virtual destructor.
      virtual ~DataDestinationString();
      virtual void endMessage();
      /// @brief access the data written to the destination as a string
      const std::string & getValue()const;
    protected:
      virtual DestinationBufferPtr allocateBuffer();

    private:
      std::string value_;
    };
  }
}
#endif // DATADESTINATIONSTRING_H
