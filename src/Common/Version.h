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
    "1.3 "
    "[prerelease] "
    "\n"
    "Copyright (c) 2009, 2010 Object Computing, Inc.\n"
    "All Rights Reserved\n"
    "See the file license.txt for licensing information.\n";
  /// @brief version number
  const long QuickFAST_Version = 0x00010003; // MMMM.mmmm {M=Major, m=minor)
}
#endif // VERSION_H
