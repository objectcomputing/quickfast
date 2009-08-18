// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
# pragma warning(disable:4251) // Disable VC warning about dll linkage required (for private members?)
# pragma warning(disable:4275) // disable warning about non dll-interface base class.
# pragma warning(disable:4996) // Disable VC warning that std library may be unsafe
# pragma warning(disable:4290) // C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
# pragma warning(disable:4396) // Disable 'boost::operator !=' : the inline specifier cannot be used when a friend declaration refers to a specialization of a function template
                               // boost::unordered_set triggers this.  I think it's a bug somewhere, but it doesn't
                               // cause any problems because the code never compares boost::unordered sets

#endif
#ifndef QuickFAST_H
#define QuickFAST_H

#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
# define NOMINMAX                 // Do not define min & max a macros: l'histoire anciene
# include <windows.h>
#endif // _WIN32

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <stdexcept>
#include <math.h>
#include <iostream>
#include <iomanip>

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/operators.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>


////////////////////////
// Doxygen documentation

/// @mainpage
/// This project is an implementation of the FAST Protcol.
///  http://www.fixprotocol.org/fast
///
/// <h3>FAST</h3>
/// FAST is a standard for serializing an <b><i>application data type</i></b>
/// into a <b><i>message</i></b>.
/// The goal is to minimize the size of the encoded message thereby minimizing the
/// bandwidth required to transmit the information.  Ideally this should be done
/// without requiring excessive CPU useage.
///
/// This is achieved by creating a custom encoding/decoding compression strategy
/// for each type of messsage using a set of basic codec instructions.  This strategy
/// is called a <b><i>template</i></b>.   More than one template can be (and usually
/// is) defined.
/// There is a many-to-many correspondence between application data types and templates.
/// The encoder is free to choose the most effect template to apply to each message that
/// it encodes.
///
/// Templates are most commonly represented as XML documents.  See the specification
/// available from the URL above for details about the XML schema.
///
/// The template file is shared between both counterparties via out-of-band
/// communication. It is vital that both counterparties be using the same set of templates.
///
/// <h3>Using QuickFAST in an application that receives FAST data -- low level approach.</h3>
/// To receive FAST encoded data, a typical application would:<ul>
/// <li>Use a QuickFAST::Codecs::XMLTemplateParser to read a template file and produce
/// a QuickFAST::Codecs::TemplateRegistry.</li>
/// <li>Create a QuickFAST::Codecs::Decoder and supply it with the template registry to use.</li>
/// <li>Create an object that implements the QuickFAST::Codecs::DataSource interface to accept the
/// incoming stream of data and supply it to the decoder.</li>
/// <li>Each time the data source has a complete message available<ul>
///  <li>Create an empty QuickFAST::Messages::Message</li>
///  <li>Pass the data source and the message to the decoder's decodeMessage() method.</li>
///  <li>Process the message which has now been filled by the decoder.</li></ul>
/// </li></ul>
///
/// The application should catch exceptions thrown by the decoder.  The data source should have
/// a method for resynchronizing the input after an exception has disrupted the decoding of an
/// incoming message.
///
/// <h3>"Prepackaged" decoders.</h3>
/// The QuickFAST::Codecs::SynchronousDecoder and QuickFAST::Codecs::MulticastDecoder
/// objects support simpler approach to writing a decoder application.  To use either
/// of these objects, you need an application object that can handle the incoming messages.
/// This object should implement the QuickFAST::Codecs::MessageConsumer interface.
///
/// The SynchronousDecoder or MulticastDecoder handles the details of receiving and decoding
/// incoming messages.  Each time a complete messages has been decoded, the consumeMessage()
/// method of your MessageConsumer object will be called.
///
/// The SynchronousDecoder can be used to receive messages from a DataSource that either has
/// all incoming data available (reading from a file, for example.) or that blocks waiting
/// for incoming data to arrive when necessary.
///
/// The MulticastDecoder using nonblocking boost::asio to dispatch incoming data to the
/// decoder out of an event loop run by a boost::io_service object.  Multiple input channels
/// can be supported--each with it's own independent decoder.  Multiple threads can be used.
/// be used, to allow parallel processing, although each decoder will only be executing in one
/// thread at a time.  See the boost::asio documentation for details.
///
/// <h3>Using QuickFAST in an application that sends FAST data.</h3>
/// To send FAST encoded data, a typical application would:<ul>
/// <li>Use a QuickFAST::Codecs::XMLTemplateParser to read a template file and produce
/// a QuickFAST::Codecs::TemplateRegistry.</li>
/// <li>Create a QuickFAST::Codecs::Encoder and supply it with the template registry to use.</li>
/// <li>Create an object that implements the QuickFAST::Codecs::DataDestination interface to handle
/// the FAST encoded data that will be generated.</li>
/// <li>Whenever data is available to be sent:<ul>
///  <li>Create an empty QuickFAST::Messages::Message</li>
///  <li>Add fields to the message by <ul>
///   <li>creating a QuickFAST::Messages::FieldIdentity to identify the field, and</li>
///   <li>creating the appropriate implementation of QuickFAST::Messages::Field to represent the field value.</li>
///   <li>adding the field to the message using the addField() method.</li></ul></li>
///  <li>Pass the populated message and the data destination to the encoders encodeMessage() method.</li>
/// </ul></ul>
///
/// The encoder will send the encoded data to the DataDestination, then call the DataDestination::endMessage()
/// method.  This call is a signal for the destination to actually transmit or otherwise process the encoded
/// message.
///
/// The application should catch exceptions thrown by the encoder and tell the the data destination
/// to discard any partially encoded information.
///
/// <h4>
/// For examples of how QuickFAST can be used in an application </h4>
/// @see QuickFAST::InterpretFAST
/// @see QuickFAST::MulticastInterpreter
/// @see QuickFAST::PerformanceTest
///
/// These examples use some support programs
/// @see QuickFAST::FileToTCP
/// @see QuickFAST::FileToMulticast
///
/// @todo: Discuss threading issues.
/// @todo: Discuss message buffering issues.
///
/// This page was generated from comments in src/common/QuickFASTPCH.h

/// @brief General utility/overhead classes used throughout the rest of the
/// system are in the ::QuickFAST namespace.
///
/// Source files for elements in this namespace are in the src/Common directory.
///
/// <i>This page was generated from comments in src/common/QuickFASTPCH.h</i>
namespace QuickFAST{

  /// @brief A FAST encoder and decoder.
  ///
  /// This namespace focuses on templates, codecs, and the encoding/decoding process, not on
  /// the use of the application data.
  ///
  /// QuickFAST::Codecs::Encoder is the encoder. <br>
  /// QuickFAST::Codecs::Decoder is the decoder. <br>
  /// QuickFAST::Codecs::XMLTemplateParser parses the templates.
  ///
  ///  @see XMLTemplateParser for more detailed information about parsing XML into templates.
  ///
  /// Source files for elements in this namespace are in the src/Codecs directory.
  ///
  /// <i>This page was generated from comments in src/common/QuickFASTPCH.h</i>
  namespace Codecs{}
  /// @brief Classes for managing FAST application data: messages and fields.
  ///
  /// This namespace focuses on using the application data.  It has no knowledge
  /// of the encoding/decoding process.  In particular nothing in this namespace
  /// should know anything about the ::QuickFAST::Codecs namespace.
  ///
  /// Application data is sent to and from QuickFAST via QuickFAST::Messages::Message
  /// objects.  A Message is a collection of QuickFAST::Messages::Field objects.
  /// Each field within the message is identified by a QuickFAST::Messages::FieldIdentity.
  ///
  /// Source files for elements in this namespace are in src/Messages directory.
  ///
  /// <i>This page was generated from comments in src/common/QuickFASTPCH.h</i>
  namespace Messages{}
}

#endif // QuickFAST_H
