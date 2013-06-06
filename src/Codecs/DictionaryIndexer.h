// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DICTIONARYINDEXER_H
#define DICTIONARYINDEXER_H
#include <Common/QuickFAST_Export.h>
#include "DictionaryIndexer_fwd.h"

namespace QuickFAST{
  namespace Codecs{
    /// @brief Helps build an index of Dictionary Items
    ///
    /// Allows finding and updating by index rather than by search
    /// Note that this simply assigns index values before encoding/decoding begins.
    /// The actual array of dictionary entries will be created by the Context
    /// when the Encoder or Decoder is created.
    class QuickFAST_Export DictionaryIndexer
    {
    public:
      DictionaryIndexer();
      ~DictionaryIndexer();
      /// @brief tell the indexer when you start a new template.
      void newTemplate();

      /// @brief Get the index for a field
      ///
      /// This will either find an existing index, or create a new one.
      ///
      /// @param dictionaryName identifies the dictionary to be used for this field.
      ///        Special values "", "global", "template", and "type" will be recognized.
      ///        Any other value is a user defined dictionary.
      /// @param typeName names the application type in case dictionary name is "type"
      /// @param typeNamespace namespace to qualifytypeName
      /// @param key is the key to identify the element in the dictionary
      /// @param keyNamespace qualifies the key name.
      size_t getIndex(
        const std::string & dictionaryName,
        const std::string & typeName,
        const std::string & typeNamespace,
        const std::string & key,
        const std::string & keyNamespace);

      /// @brief How many dictionary entries are needed.
      /// @returns a count of dictionary entries.
      size_t size()const;

    private:
      typedef std::map<std::string, size_t> NameToIndex;
      size_t getDictionaryIndex(NameToIndex & nameToIndex, const std::string & key);

      NameToIndex globalNames_;
      NameToIndex templateNames_;
      NameToIndex typeNames_;
      NameToIndex qualifiedNames_;
      size_t index_;
    };
  }
}
#endif // DICTIONARYINDEXER_H
