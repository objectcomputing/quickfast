// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif

#ifndef XMLTEMPLATEPARSER_FWD_H
#define XMLTEMPLATEPARSER_FWD_H

#include <boost/shared_ptr.hpp>

namespace QuickFAST
{
  namespace Util
  {
    class XMLTemplateParser;
    /// @brief A smart pointer to a XMLTemplateParser.
    typedef boost::shared_ptr<XMLTemplateParser> XMLTemplateParserPtr;
    /// @brief A smart pointer to a const XMLTemplateParser.
    typedef boost::shared_ptr<const XMLTemplateParser> XMLTemplateParserCPtr;
  }
}
#endif // XMLTEMPLATEPARSER_FWD_H
