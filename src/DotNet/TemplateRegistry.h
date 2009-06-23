// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include <DotNet/BoostPtr.h>
#include <Codecs/TemplateRegistry.h>

namespace QuickFASTDotNet {
  namespace Codecs{

    /// @brief A registry of known templates.
    ///
    /// Normally the Template Registry will be initialized by reading an
    /// XML templates file. This is done by a QuickFAST::Util::XMLTemplateParser object.
    public ref class TemplateRegistry
    {
    public:
      static TemplateRegistry^ Parse(System::IO::Stream^ xmlStream);

      /// @brief The number of templates in the registry.
      property unsigned int Size
      {
        unsigned int get() { return unsigned int(templateRegistry_->size()); }
      }

      /// @brief Returns the maximum number of fields that will be produced by any template in the registry.
      ///
      /// Does not include "nested" fields -- unmerged groups and sequences count as one each.
      property unsigned int MaxFieldCount
      {
        unsigned int get() { return unsigned int(templateRegistry_->maxFieldCount()); }
      }

    internal:
      TemplateRegistry(QuickFAST::Codecs::TemplateRegistryPtr templateRegistry):
            templateRegistry_(templateRegistry)
      {
      }

      /// Returns a QuickFAST::Codecs::TemplateRegistryPtr shared_ptr
      property QuickFAST::Codecs::TemplateRegistryPtr NativeTemplateRegistry
      {
        QuickFAST::Codecs::TemplateRegistryPtr get() { return templateRegistry_.GetBoostPtr(); }
      }

    private:
      BoostPtrHolder<QuickFAST::Codecs::TemplateRegistryPtr> templateRegistry_;
    };
  }
}
