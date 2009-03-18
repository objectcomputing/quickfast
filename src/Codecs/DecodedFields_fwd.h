// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DECODEDFIELDS_FWD_H
#define DECODEDFIELDS_FWD_H
namespace QuickFAST{
  namespace Messages{
    class DecodedFields;
    /// @brief Smart pointer to a FieldSet.
    typedef boost::shared_ptr<DecodedFields> DecodedFieldsPtr;
    /// @brief Smart pointer to a const FieldSet.
    typedef boost::shared_ptr<const DecodedFields> DecodedFieldsCPtr;
  }
}
#endif // DECODEDFIELDS_FWD_H
