// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ENCODER_FWD_H
#define ENCODER_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

namespace QuickFAST{
  namespace Codecs{
    class Encoder;
    /// @brief A smart pointer to a Encoder.
    typedef boost::shared_ptr<Encoder> EncoderPtr;
  }
}
#endif // ENCODER_FWD_H
