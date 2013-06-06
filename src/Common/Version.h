// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef VERSION_H
#define VERSION_H
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>
namespace QuickFAST
{
  /// @brief Product information about QuickFAST
  const char QuickFAST_Product[] = "QuickFAST Version "
    "1.5.0"
    "\n"
    "Copyright (c) 2009, 2010, 2011, 2012, 2013 Object Computing, Inc.\n"
    "All Rights Reserved\n"
    "See the file license.txt for licensing information.\n";
  /// @brief version number
  const long QuickFAST_Version = 0x00010500; // MMMM.mmpp {M=Major, m=minor, p=patch)
}
#endif // VERSION_H
