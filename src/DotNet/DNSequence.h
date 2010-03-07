// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

namespace QuickFAST
{
  namespace DotNet
  {
    ref class DNMessageBuilder;
    ref class DNFieldSet;

    class ImplSequence;

    public ref class DNSequence
    {
    public:
      DNSequence(ImplSequence & impl);
      ~DNSequence();

      // todo: properties not methods!
      System::String ^ lengthName();
      System::String ^ lengthNamespace();
      System::String ^ lengthId();


      /// How many entries are defined
      size_t size();

      DNFieldSet ^ entry(size_t index);

    private:
      ImplSequence * impl_;
    };
  }
}
