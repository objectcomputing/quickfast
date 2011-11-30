// Copyright (c) 2009, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef TEMPLATE_FWD_H
#define TEMPLATE_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

namespace QuickFAST{
  namespace Codecs{
    class Template;
    /// @brief A smart pointer to a Template.
    typedef boost::shared_ptr<Template> TemplatePtr;
    /// @brief A smart pointer to a const Template.
    typedef boost::shared_ptr<const Template> TemplateCPtr;
  }
}
#endif // TEMPLATE_FWD_H
