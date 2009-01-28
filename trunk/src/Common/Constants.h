// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <Common/Types.h>

namespace QuickFAST{
  static const uchar stopBit = '\x80';
  static const uchar dataBits = '\x7F';
  static const uchar signBit = '\x40';
  static const unsigned int dataShift = 7;
  static const unsigned int byteSize = 8;

  static const uchar nullInteger = '\x80';
  static const uchar nullAscii = '\x80';
  static const uchar nullBlob = '\x80';
  static const uchar nullDecimal = '\x80';

  static const uchar nullableStringPreamble = '\0';
  static const uchar emptyString = '\x80';
  static const uchar leadingZeroBytePreamble = '\0';

}
#endif // CONSTANTS_H
