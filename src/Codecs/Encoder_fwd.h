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
    /// Right now this is just a placeholder for Encoder
    /// And someplace for me to jot down my my ideas on how
    /// the encoder will work.
    ///
    /// The goal of the encoder is to create a Codecs::Template
    /// object.  A Template object is primarily an ordered
    /// collection of FieldInstructions.
    class Encoder;
    /// @brief A smart pointer to a Encoder.
    //typedef boost::shared_ptr<Encoder> EncoderPtr;
    /// @brief A smart pointer to a const Encoder.
    //typedef boost::shared_ptr<const Encoder> EncoderCPtr;
  }
}
#endif // ENCODER_FWD_H
