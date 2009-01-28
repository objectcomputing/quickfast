// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef TEMPLATE_FWD_H
#define TEMPLATE_FWD_H
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
