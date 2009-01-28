// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#ifdef _MSC_VER
# pragma once
#endif

#ifndef SCHEMAELEMENT_FWD_H
#define SCHEMAELEMENT_FWD_H
namespace QuickFAST
{
  namespace Codecs
  {
    class SchemaElement;
    /// A smart pointer to a SchemaElement
    typedef boost::shared_ptr<SchemaElement> SchemaElementPtr;
  }
}

#endif /* SCHEMAELEMENT_FWD_H */
