// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef TEMPLATEREGISTRY_FWD_H
#define TEMPLATEREGISTRY_FWD_H
namespace QuickFAST{
  namespace Codecs{
    class TemplateRegistry;
    /// @brief A smart pointer to a TemplateRegistry.
    typedef boost::shared_ptr<TemplateRegistry> TemplateRegistryPtr;
    /// @brief A smart pointer to a const TemplateRegistry.
    typedef boost::shared_ptr<const TemplateRegistry> TemplateRegistryCPtr;
  }
}
#endif // TEMPLATEREGISTRY_FWD_H
