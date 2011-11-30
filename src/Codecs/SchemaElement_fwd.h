// Copyright (c) 2009, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

#ifdef _MSC_VER
# pragma once
#endif
#ifndef SCHEMAELEMENT_FWD_H
#define SCHEMAELEMENT_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

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
