// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DICTIONARYFWD_H
#define DICTIONARYFWD_H
namespace QuickFAST{
  namespace Codecs{
    class Dictionary;
    /// @brief A smart pointer to a Dictionary
    typedef boost::shared_ptr<Dictionary> DictionaryPtr;
    /// @brief A smart pointer to a const Dictionary
    typedef boost::shared_ptr<const Dictionary> DictionaryCPtr;
  }
}

#endif // DICTIONARYFWD_H
