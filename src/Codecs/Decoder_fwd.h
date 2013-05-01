// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DECODER_FWD_H
#define DECODER_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

namespace QuickFAST{
  namespace Codecs{
    class Decoder;
    /// @brief A smart pointer to a Decoder.
//    typedef boost::shared_ptr<Decoder> DecoderPtr;
    /// @brief A smart pointer to a const Decoder.
//    typedef boost::shared_ptr<const Decoder> DecoderCPtr;
  }
}
#endif // DECODER_FWD_H
