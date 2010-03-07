// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#include <DotNet/DNSequence.h>
#include <DotNet/DNDecimal.h>
#include <DotNet/DNField.h>

namespace QuickFAST
{
  namespace DotNet
  {
    class ImplFieldSet;

    // Design decsion: Rather than defining DNField, I chose to provide access to field content and identity via indexing the field set.
    public ref class DNFieldSet
    {
    public:
      DNFieldSet(ImplFieldSet & impl);
      ~DNFieldSet();

      /// How many fields are defined
      size_t size();

      /// @brief find a field with the given local name (ignores namespace)
      int findIndexByName(System::String ^ fieldName);

      DNField ^ getField(size_t   index);

      DNField ^ findFieldByName(System::String ^ fieldName);

      void clear();


      /// @brief for internal use
//      ImplFieldSet * getImpl();

    private:
      ImplFieldSet * impl_;
      bool owned_;
    };
  }
}
