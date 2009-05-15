#ifndef ORDERBOOK_EXPORT_H
#define ORDERBOOK_EXPORT_H

#if !defined(ORDERBOOK_HAS_DLL)
# if defined(ORDERBOOK_AS_STATIC_LIBS)
#   define ORDERBOOK_HAS_DLL 0
# else
#   define ORDERBOOK_HAS_DLL 1
# endif
#endif

#if (ORDERBOOK_HAS_DLL == 1)
#  if defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x550)
#    if defined(ORDERBOOK_BUILD_DLL)
#      define ORDERBOOK_Export __symbolic
#    else
#      define ORDERBOOK_Export __global
#    endif
#  elif defined(WIN32) || defined(UNDER_CE) || defined(__CYGWIN__)
#    if defined(ORDERBOOK_BUILD_DLL)
#      define ORDERBOOK_Export __declspec(dllexport)
#    else
#      define ORDERBOOK_Export __declspec(dllimport)
#    endif
#  elif (defined(__GNUC__) && (__GNUC__ >= 4))
#    if defined(ORDERBOOK_BUILD_DLL)
#      define ORDERBOOK_Export __attribute__((visibility("default")))
#    else
#      define ORDERBOOK_Export
#    endif
#  else
#    define ORDERBOOK_Export
#  endif
#else
#  define ORDERBOOK_Export
#endif

#endif
