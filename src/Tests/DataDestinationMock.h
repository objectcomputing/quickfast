// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATADESTINATIONMOCK_H
#define DATADESTINATIONMOCK_H
#include <Codecs/DataDestination.h>

namespace QuickFAST{
  namespace Tests{
    /// Mock Field instruction that counts function calls
    class DataDestinationMock : public Codecs::DataDestination
    {
    public:
      DataDestinationMock()
      {
      }
      virtual ~DataDestinationMock()
      {
      }

      virtual void endMessage()
      {
      }

      virtual DestinationBufferPtr allocateBuffer()
      {
        DestinationBufferPtr result;
        return result;
      }

    };

  }
}

#endif /* DATADESTINATIONMOCK_H */
