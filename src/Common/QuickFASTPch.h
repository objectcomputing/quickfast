// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
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
#pragma warning(disable:4820)  // 'n' bytes padding added after data member
#pragma warning(disable:4127)  // Conditonal expression is constant (particularly in templates)
#pragma warning(disable:4100)  // Disable: unreferenced formal parameter (/W4 warning: common case for virtual methods)

#endif
#ifndef QUICKFASTPCH_H
#define QUICKFASTPCH_H
// If this symbol is not defined the user included a QuickFAST header without
// using one of the standard precompiled header files.
#define QUICKFAST_HEADERS

#ifdef _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
# endif
# ifndef NOMINMAX
#   define NOMINMAX                 // Do not define min & max a macros: l'histoire anciene
# endif
# include <windows.h>
#endif // _WIN32

#include <limits.h>
#include <stdio.h>
#include <string.h>

// If building for .NET, must link boost threads dynamically
#define BOOST_THREAD_USE_DLL
// This reports at compile time which boost libraries will be used
// #define BOOST_LIB_DIAGNOSTIC

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <stdexcept>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>

#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/operators.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/cstdint.hpp>
#include <boost/bind.hpp>

////////////////////////
// Doxygen documentation

/// @mainpage
/// This project is an implementation of the FAST Protcol.
///  http://www.fixprotocol.org/fast
///
/// The home page for this project is
///  http://quickfast.org/
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
/// <h3>Using QuickFAST in an application that receives FAST data.</h3>
///
/// Two example applications that are bundled with QuickFAST will help you to get started
/// quickly using QuickFAST to decode incoming FAST data for your application:<ul>
///  <li><b>TutorialApplication</b> is a simple application that shows QuickFAST configured
///         for one particular use-case.  Source for this application is in
///          .../src/Examples/TutorialApplication<br>
///         There is  <a href="http://code.google.com/p/quickfast/wiki/TutorialApplication">
///         a more detailed discription of this application on the QuickFAST web site.</a></li>
///  <li><b>InterpretApplication</b> is an application that uses command line options to configure
///         the QuickFAST for a variety of situations.  It supports all of the options available in
///         QuickFAST except those requiring custom code.  Source for this application is in
///         .../src/Examples/InterpretApplication.</li>
/// </ul>
///
/// <h4>Other decoding techiniques</h4>
/// Earlier versions of QuickFAST simpler, less flexiable techinques to configuring QuickFAST.
/// These techinques are still supported; however, it is suggested that future development
/// be done by starting with one of the applications described above.
///
/// For details on this lower level support see: <ul>
/// <li>QuickFAST::Codecs::SynchronousDecoder, and</li>
/// <li>QuickFAST::Codecs::MulticastDecoder.</li>
/// </ul>
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
/// The Examples directory also includes some support applications that do not explictly use QuickFAST but
/// that can be helpful in testing QuickFAST-based application:
/// @see QuickFAST::FileToTCP
/// @see QuickFAST::FileToMulticast
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

  /// @brief Classes involved in passing data to/from the Decoder/Encoder from/to the outside world.
  ///
  /// Classes in this namespace address the issues involved in:
  /// <ul>
  /// <li>reading data from communication sockets</li>
  /// <li>reading data from data files</li>
  /// <li>handling any framing or other non-FAST information in the incoming data.</li>
  /// <li>delivering the incoming data to a decoder</li>
  /// <li>accepting outgoing data from an encoder</li>
  /// <li>adding any framing or other non-FAST information to the outgoing data.</li>
  /// <li>writing outgoing data to communication sockets</li>
  /// <li>writing outgoing data to files</li>
  /// </ul>
  /// Source files for elements in this namespace are in src/Communication directory.
  namespace Communication{}

  /// @ brief Wrapper classes that provide high-level support to Applications using QuickFAST for decoding.
  ///
  /// This namespace contains: <dl>
  ///
  /// <dt>DecoderConfiguration</dt>
  ///   <dd>Contains all the information necessary to configure a DecoderConnection. </dd>
  /// <dt>DecoderConnection</dt>
  ///  <dd>Supports a single source of FAST encoded input.</dd>
  /// </dl>
  namespace Application{}

}
#endif // QUICKFASTPCH_H
