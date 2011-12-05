// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#define QUICKFAST_HEADERS
# pragma warning(disable:4251) // Disable VC warning about dll linkage required (for private members?)
# pragma warning(disable:4275) // disable warning about non dll-interface base class.
# pragma warning(disable:4996) // Disable VC warning that std library may be unsafe
# pragma warning(disable:4290) // C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
# pragma warning(disable:4396) // Disable 'boost::operator !=' : the inline specifier cannot be used when a friend declaration refers to a specialization of a function template
                               // boost::unordered_set triggers this.  I think it's a bug somewhere, but it doesn't
                               // cause any problems because the code never compares boost::unordered sets
#pragma warning(disable:4820)  // 'n' bytes padding added after data member
#pragma warning(disable:4127)  // Conditonal expression is constant (particularly in templates)
// Force BOOST to link dynamically in the .NET environment
#define BOOST_THREAD_USE_DLL
#define BOOST_LIB_DIAGNOSTIC

#include <vcclr.h>

#pragma unmanaged

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

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/operators.hpp>
#include <boost/cstdint.hpp>

#pragma managed

namespace QuickFAST
{
  /// @brief The API to allow QuickFAST to be used from .NET
  ///
  /// Note the API methods all begin with the prefix DN
  /// The classes that are named Impl* are implementation details that should not
  /// be exposed to the .NET world.
  ///
  /// To Decode incoming fast messages:
  ///   Create a DNDecoderConnection
  ///   Cofigure it by setting properties.
  ///   Create a class that implements DNMessageBuilder.  Note that DNMessageDeliverer is a good candidate for this role.
  ///   Call the DNDecoderConnection::run passing the builder and threading information.
  namespace DotNet
  {
  }
}

