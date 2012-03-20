// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef QUICKFAST_EXPORT_H
#define QUICKFAST_EXPORT_H

// Compile time controls for library generation.  Define with /D or #define
// To produce or use a static library: #define QUICKFAST_HAS_DLL=0
//   Default is to produce/use a DLL
// While building the QUICKFAST_ library: #define QUICKFAST_BUILD_DLL
//   Default is to export symbols from a pre-built QUICKFAST DLL
//
// Within QUICKFAST use the QuickFAST_Export macro where a __declspec is needed.

#if defined (_WIN32)

#  if !defined (QUICKFAST_HAS_DLL)
#    define QUICKFAST_HAS_DLL 1
#  endif /* ! QUICKFAST_HAS_DLL */

#  if defined (QUICKFAST_HAS_DLL) && (QUICKFAST_HAS_DLL == 1)
#    if defined (QUICKFAST_BUILD_DLL)
#      define QuickFAST_Export __declspec(dllexport)
#    else /* QUICKFAST_BUILD_DLL */
#      define QuickFAST_Export __declspec(dllimport)
#    endif /* QUICKFAST_BUILD_DLL */
#  else /* QUICKFAST_HAS_DLL == 1 */
#    define QuickFAST_Export
#  endif /* QUICKFAST_HAS_DLL == 1 */

#  else /* !_WIN32 */

#    define QuickFAST_Export
#  endif /* _WIN32 */
#endif /* QUICKFAST_EXPORT_H */

