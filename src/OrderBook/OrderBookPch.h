// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
# pragma warning(disable:4251) // Disable VC warning about dll linkage required (for private members?)
# pragma warning(disable:4275) // disable warning about non dll-interface base class.
# pragma warning(disable:4996) // Disable VC warning that std library may be unsafe
# pragma warning(disable:4396) // Disable 'boost::operator !=' : the inline specifier cannot be used when a friend declaration refers to a specialization of a function template
                               // boost::unordered_set triggers this.  I think it's a bug somewhere, but it doesn't
                               // cause any problems because the code never compares boost::unordered sets
#endif
#ifndef ORDERBOOKPCH_H
#define ORDERBOOKPCH_H

#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
# define NOMINMAX                 // Do not define min & max a macros: l'histoire anciene
# include <windows.h>
#endif // _WIN32

#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <stdexcept>
#include <math.h>
#include <iomanip>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/operators.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <iostream>

#endif // ORDERBOOKPCH_H
