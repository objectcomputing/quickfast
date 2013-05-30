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
  static const uchar stopBit = uchar('\x80');
  static const uchar dataBits = uchar('\x7F');
  static const uchar signBit = uchar('\x40');
  static const unsigned int dataShift(7u);
  static const unsigned int byteSize(8u);

  static const uchar zeroIntegerNonnullable = uchar('\x80');
  static const uchar zeroIntegerNullable = uchar('\x81');

  static const uchar nullInteger = uchar('\x80');
  static const uchar nullAscii = uchar('\x80');
  static const uchar nullBlob = uchar('\x80');
  static const uchar nullDecimal = uchar('\x80');

  static const uchar nullableStringPreamble = uchar('\0');
  static const uchar emptyString = uchar('\x80');
  static const uchar leadingZeroBytePreamble = uchar('\0');

  static const uchar ASCII_SOH = '\x01';
  static const uchar ASCII_ETX = '\x03';
  static const uchar ASCII_US = '\x1F';

}
#endif // CONSTANTS_H
