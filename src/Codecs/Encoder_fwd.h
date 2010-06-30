// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ENCODER_FWD_H
#define ENCODER_FWD_H
namespace QuickFAST{
  namespace Codecs{
    class Encoder;
    /// @brief A smart pointer to a Encoder.
    typedef boost::shared_ptr<Encoder> EncoderPtr;
  }
}
#endif // ENCODER_FWD_H
