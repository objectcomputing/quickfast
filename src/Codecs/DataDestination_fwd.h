// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DATADESTINATION_FWD_H
#define DATADESTINATION_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

namespace QuickFAST{
  namespace Codecs{
    class DestinationBuffer;
    /// @brief A smart pointer to a DestinationBuffer.
    typedef boost::shared_ptr<DestinationBuffer> DestinationBufferPtr;

    class DataDestination;
    /// @brief A smart pointer to a DataDestination.
    typedef boost::shared_ptr<DataDestination> DataDestinationPtr;
  }
}
#endif // DATADESTINATION_FWD_H
