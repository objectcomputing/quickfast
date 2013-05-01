// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "Types.h"

using namespace QuickFAST;

const std::string &
ValueType::typeName(ValueType::Type type)
{
  switch(type)
  {
  case INT8:
    {
      static const std::string name("int8");
      return name;
    }
  case UINT8:
    {
      static const std::string name("uInt8");
      return name;
    }
  case INT16:
    {
      static const std::string name("int16");
      return name;
    }
  case UINT16:
    {
      static const std::string name("uInt16");
      return name;
    }
  case INT32:
    {
      static const std::string name("int32");
      return name;
    }
  case UINT32:
    {
      static const std::string name("uInt32");
      return name;
    }
  case INT64:
    {
      static const std::string name("int64");
      return name;
    }
  case UINT64:
    {
      static const std::string name("uInt64");
      return name;
    }
  case DECIMAL:
    {
      static const std::string name("decimal");
      return name;
    }
  case EXPONENT:
    {
      static const std::string name("exponent");
      return name;
    }
  case MANTISSA:
    {
      static const std::string name("mantissa");
      return name;
    }
  case ASCII:
    {
      static const std::string name("ascii");
      return name;
    }
  case UTF8:
    {
      static const std::string name("utf8");
      return name;
    }
  case BYTEVECTOR:
    {
      static const std::string name("byteVector");
      return name;
    }
  case SEQUENCE:
    {
      static const std::string name("sequence");
      return name;
    }
  case LENGTH:
    {
      static const std::string name("length");
      return name;
    }
  case GROUP:
    {
      static const std::string name("group");
      return name;
    }
  case TEMPLATEREF:
    {
      static const std::string name("templateRef");
      return name;
    }

  case TYPEREF:
    {
      static const std::string name("typeRef");
      return name;
    }

  default:
    {
      static const std::string name("Unknown");
      return name;
    }
  }
}

