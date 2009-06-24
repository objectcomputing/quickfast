#ifdef _MSC_VER
# pragma once
#endif
#ifndef QUICKFASTARCA_EXPORT_H
#define QUICKFASTARCA_EXPORT_H

// Compile time controls for library generation.  Define with /D or #define
// To produce or use a static library: #define QUICKFASTARCA_HAS_DLL=0
//   Default is to produce/use a DLL
// While building the QUICKFASTARCA_ library: #define QUICKFASTARCA_BUILD_DLL
//   Default is to export symbols from a pre-built QUICKFASTARCA DLL
//
// Within QUICKFASTARCA use the QUICKFASTARCA_Export macro where a __declspec is needed.

#if defined (_WIN32)

#  if !defined (QUICKFASTARCA_HAS_DLL)
#    define QUICKFASTARCA_HAS_DLL 1
#  endif /* ! QUICKFASTARCA_HAS_DLL */

#  if defined (QUICKFASTARCA_HAS_DLL) && (QUICKFASTARCA_HAS_DLL == 1)
#    if defined (QUICKFASTARCA_BUILD_DLL)
#      define QuickFASTArca_Export __declspec(dllexport)
#    else /* QUICKFASTARCA_BUILD_DLL */
#      define QuickFASTArca_Export __declspec(dllimport)
#    endif /* QUICKFASTARCA_BUILD_DLL */
#  else /* QUICKFASTARCA_HAS_DLL == 1 */
#    define QuickFASTArca_Export
#  endif /* QUICKFASTARCA_HAS_DLL == 1 */

#  else /* !_WIN32 */

#    define QuickFASTArca_Export
#  endif /* _WIN32 */
#endif /* QUICKFASTARCA_EXPORT_H */

